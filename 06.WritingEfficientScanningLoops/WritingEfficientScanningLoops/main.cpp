#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void colorReduce_original(Mat image, int div = 64) {

	int nl = image.rows;	// number of iines
	// total number of elements per line
	int nc = image.cols * image.channels();

	for (int j = 0; j < nl; j++) {
		// get the address of row j
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++) {
			// process each pixel
			data[i] = data[i] / div * div + div / 2;
			// end of pixel processing
		}
	}
}

void colorReduce_bitWise(cv::Mat image, int div = 64) {
	int nl = image.rows; // number of lines
	// total number of elements per line
	int nc = image.cols * image.channels();
	
	int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5);
	// mask used to round the pixel value
	uchar mask = 0xFF << n; // e.g. for div=16, mask= 0xF0
	uchar div2 = div >> 1; // div2 = div/2
	// this loop is executed only once
	// in case of continuous images
	for (int j = 0; j < nl; j++) {
		uchar* data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {
			*data &= mask;
			*data++ += div2;
		} // end of line
	}
}

void colorReduce_1D(cv::Mat image, int div = 64) {
	int nl = image.rows; // number of lines
	// total number of elements per line
	int nc = image.cols * image.channels();
	if (image.isContinuous()) {
		// then no padded pixels
		nc = nc * nl;
		nl = 1; // it is now a 1D array
	}

	int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5);
	// mask used to round the pixel value
	uchar mask = 0xFF << n; // e.g. for div=16, mask= 0xF0
	uchar div2 = div >> 1; // div2 = div/2
	// this loop is executed only once
	// in case of continuous images
	for (int j = 0; j < nl; j++) {
		uchar* data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {
			*data &= mask;
			*data++ += div2;
		} // end of line
	}
}


int main() {
	// read the image
	Mat image = imread("dog.jpg");
	Mat image2 = imread("dog2.jpg");
	Mat image3 = imread("dog3.jpg");

	int64 start_original = getTickCount();
	colorReduce_original(image, 64);
	int64 end_original = getTickCount();
	double time_original = (end_original - start_original) / getTickFrequency();
	cout << "original time : " << time_original << endl;

	int64 start_bitWise = getTickCount();
	colorReduce_bitWise(image2, 64);
	int64 end_bitWise = getTickCount();
	double time_bitWise = (end_bitWise - start_bitWise) / getTickFrequency();
	cout << "bitWise time : " << time_bitWise << endl;

	int64 start_1D = getTickCount();
	colorReduce_1D(image3, 64);
	int64 end_1D = getTickCount();
	double time_1D = (end_1D - start_1D) / getTickFrequency();
	cout << "1D time : " << time_1D << endl;

	system("pause");
}