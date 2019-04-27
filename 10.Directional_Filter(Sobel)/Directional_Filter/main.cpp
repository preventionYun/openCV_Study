#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat original = imread("lena.tif", IMREAD_GRAYSCALE);
	Mat sobel_X, sobel_Y;
	Mat sobel;
	Mat scharr_X, scharr_Y;
	Mat scharr;
	Mat sobelThresholded;
	
	// apply low pass filter (optional)
	GaussianBlur(original, original, Size(3,3), 1.5);

	/*
	void Sobel( InputArray src, OutputArray dst, int ddepth,
                         int dx, int dy, int ksize = 3,
                         double scale = 1, double delta = 0,
                         int borderType = BORDER_DEFAULT );
	*/
	Sobel(original, sobel_X, CV_8UC1, 1, 0, 3, 0.4, 128);	
	Sobel(original, sobel_Y, CV_8UC1, 0, 1, 3, 0.4, 128);
	sobel = abs(sobel_X) + abs(sobel_Y);
	
	/*
	double threshold( InputArray src, OutputArray dst, double thresh, double maxval, int type );
	*/
	threshold(sobel, sobelThresholded, 240, 255, cv::THRESH_BINARY);

	imwrite("grayScale.bmp", original);
	imwrite("sobel_X.bmp", sobel_X);
	imwrite("sobel_Y.bmp", sobel_Y);
	imwrite("sobel.bmp", sobel);
	imwrite("sobelThresholded.bmp", sobelThresholded);
}

