#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

using std::string;
using std::tuple;

class VideoProcessor {
private:
	size_t m_frameCount;
	string m_working_file;
	uint32_t m_color_code;
public:
	VideoProcessor(string fileName, uint32_t colorCode);

	tuple<uint8_t, uint8_t, uint8_t> colorCodeToRgb(uint32_t color_code);

	void showWindow(int window_height, int window_width, uint8_t* windowData);
};