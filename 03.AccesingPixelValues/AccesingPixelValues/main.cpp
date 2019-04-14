#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

void salt(Mat image, int n) {
	default_random_engine generator;
	uniform_int_distribution<int> randomRow(0, image.rows - 1);
	uniform_int_distribution<int> randomCol(0, image.cols - 1);

	int i, j;
	for (int k = 0; k < n; k++) {
		// random image coordinate
		i = randomCol(generator);
		j = randomRow(generator);

		if (image.type() == CV_8UC1) { // gray-level image 
			// single-channel 8-bit image
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3) { // color image
			// 3-channel image
			image.at<Vec3b>(j, i)[0] = 255;
			image.at<Vec3b>(j, i)[1] = 255;
			image.at<Vec3b>(j, i)[2] = 255;
		}
	}
}

int main() {
	Mat image = imread("lena.tif", IMREAD_COLOR);

	salt(image, 3000);

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);
}