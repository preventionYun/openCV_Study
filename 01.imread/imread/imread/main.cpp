#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	// Mat imread( const String& filename, int flags = IMREAD_COLOR );

	// original
	Mat original = imread("dog.jpg");
	if (!original.empty()) {
		cout << "original channel : " << original.channels() << endl;
		namedWindow("original");
		imshow("original", original);
		waitKey(0);
	}
	else
		cout << "original image read fail";

	// color -> gray scale read
	Mat grayScaleRead = imread("dog.jpg", IMREAD_GRAYSCALE);
	if (!grayScaleRead.empty()) {
		cout << "grayScaleRead channel : " << grayScaleRead.channels() << endl;
		namedWindow("grayScaleRead");
		imshow("grayScaleRead", grayScaleRead);
		waitKey(0);
	}
	else
		cout << "grayScaleRead image read fail";

	// original gray image, without flag
	Mat grayDefault = imread("gray_dog.jpg");
	if (!grayDefault.empty()) {
		cout << "grayDefault channel : " << grayDefault.channels() << endl;
		namedWindow("grayDefault");
		imshow("grayDefault", grayDefault);
		waitKey(0);
	}
	else
		cout << "gray_dog image read fail";

	// original gray image, with flag
	Mat grayUnchanged = imread("gray_dog.jpg", IMREAD_UNCHANGED);
	if (!grayUnchanged.empty()) {
		cout << "grayUnchanged channel : " << grayUnchanged.channels() << endl;
		namedWindow("grayUnchanged");
		imshow("grayUnchanged", grayUnchanged);
		waitKey(0);
	}
	else
		cout << "gray_dog image read fail";
}