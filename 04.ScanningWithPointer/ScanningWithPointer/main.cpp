#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void colorReduce(Mat image, int div = 64) {
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

int main() {
	// read the image
	Mat image = imread("castle.jpg");
	colorReduce(image, 16);
	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);
	imwrite("colorReduce_16.jpg", image);
}