#include "VideoProcessor.h"

using std::cin;
using std::cout;
using std::hex;

int main()
{
	uint32_t colorCode;
	cout << "Enter color code to recognize:";
	cin >> hex >> colorCode;

	string path;
	cout << "Enter path to video file:";
	cin >> path;

	VideoProcessor vp(path, colorCode);
	return 0;
}
