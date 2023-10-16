#include "VideoProcessor.h"
#include<vector>
#include <thread>

using std::cout;
using std::vector;
using std::endl;
using std::to_string;
using std::get;
using cv::imshow;
using cv::waitKey;
using cv::countNonZero;
using cv::imwrite;
using cv::inRange;
using cv::Mat;
using std::exception;
using cv::VideoCapture;
using cv::Rect;
using cv::Vec3b;
using cv::Scalar;
using cv::CAP_PROP_POS_MSEC;
using cv::CAP_PROP_FPS;

VideoProcessor::VideoProcessor(string fileName, uint32_t cCode) : m_frameCount(0), m_working_file(fileName), m_color_code(cCode)
{
	VideoCapture cap(m_working_file);

	if (!cap.isOpened()) {
		throw exception("Error opening video file\n");
	}

	Mat frame;
	int x = 0;
	int y = 300;
	int width = 400;
	int height = 240;

	double fps = cap.get(CAP_PROP_FPS);
	int interval = static_cast<int>(fps);
	double delay = 1000.0 / fps;
	int num_frames = cap.get(cv::CAP_PROP_FRAME_COUNT);
	int frameCount = 0;

	tuple<uint8_t, uint8_t, uint8_t> color = colorCodeToRgb(m_color_code);
	vector<Mat> frames(num_frames);
	while (frameCount != num_frames)
	{
		frameCount++;
		if (frameCount % interval != 0)
			continue;

		cap.read(frames[frameCount++]);
	}


	Scalar lower_color(119, 124, 53);
	Scalar middle_yellow(228, 229, 90);
	Scalar upper_color(get<0>(color), get<1>(color), get<2>(color));

	// Set the number of threads to use
	int numThreads = std::thread::hardware_concurrency();
	std::mutex indexMutex;

	cv::parallel_for_(cv::Range(0, num_frames), [&](const cv::Range& range)
		{
			for (int i = range.start; i < range.end; i++)
			{
				if (i % interval != 0)
					continue;

				Rect window_frame(x, frames[i].rows - y, width, height);

				Mat cropped = frames[i](window_frame);

				for (int y = 0; y < frame.rows; y++) {
					for (int x = 0; x < frame.cols; x++) {
						Scalar pixel = frame.at<Vec3b>(y, x);
						if ((pixel[0] == lower_color[0] && pixel[1] == lower_color[1] && pixel[2] == lower_color[2]) ||
							(pixel[0] == upper_color[0] && pixel[1] == upper_color[1] && pixel[2] == upper_color[2]) ||
							(pixel[0] == middle_yellow[0] && pixel[1] == middle_yellow[1] && pixel[2] == middle_yellow[2])) {
							double timestamp_sec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0;
							cout << "Color (" << lower_color[0] << "," << lower_color[1] << "," << lower_color[2] << ") found at timestamp: " << timestamp_sec << " seconds" << endl;

							// Save the target window as image 
							/*const string filePath = ".\\frame" + to_string(frameCount) + ".jpg";
							imwrite(filePath, cropped);*/
							break;
						}
					}
				}
			}
		});

	//while (cap.read(frame)) {
	//	frameCount++;
	//	if (frameCount % interval != 0)
	//		continue;

	//	Rect window_frame(x, frame.rows - y, width, height);

	//	Mat cropped = frame(window_frame);

	//	for (int y = 0; y < frame.rows; y++) {
	//		for (int x = 0; x < frame.cols; x++) {
	//			Scalar pixel = frame.at<Vec3b>(y, x);
	//			if ((pixel[0] == lower_color[0] && pixel[1] == lower_color[1] && pixel[2] == lower_color[2]) ||
	//				(pixel[0] == upper_color[0] && pixel[1] == upper_color[1] && pixel[2] == upper_color[2]) ||
	//				(pixel[0] == middle_yellow[0] && pixel[1] == middle_yellow[1] && pixel[2] == middle_yellow[2])) {
	//				double timestamp_sec = cap.get(CAP_PROP_POS_MSEC) / 1000.0;
	//				cout << "Color (" << lower_color[0] << "," << lower_color[1] << "," << lower_color[2] << ") found at timestamp: " << timestamp_sec << " seconds" << endl;

	//				// Save the target window as image 
	//				const string filePath = ".\\frame" + to_string(frameCount) + ".jpg";
	//				imwrite(filePath, cropped);
	//				break;
	//			}
	//		}
	//	}
	//}

	m_frameCount = frameCount;
	cap.release();
}

tuple<uint8_t, uint8_t, uint8_t> VideoProcessor::colorCodeToRgb(uint32_t color_code) {
	uint8_t r = (color_code >> 16) & 0xFF;
	uint8_t g = (color_code >> 8) & 0xFF;
	uint8_t b = color_code & 0xFF;
	return std::make_tuple(r, g, b);
}

void VideoProcessor::showWindow(int window_height, int window_width, uint8_t* windowData)
{
	Mat windowMat(window_height, window_width, CV_8UC3, windowData);

	imshow("Window", windowMat);
	waitKey(0);
}