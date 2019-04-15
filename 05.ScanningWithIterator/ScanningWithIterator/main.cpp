#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void colorReduction(Mat image, int div = 64) {
	// div must be a power of 2
	int n = static_cast<int>(
		log(static_cast<double>(div)) / log(2.0) + 0.5);
	// mask used to round the pixel value
	uchar mask = 0xFF << n;	// e.g. for div = 16, mask = 0xF0
	uchar div2 = div >> 1;	// div2 = div/2

	// get iterators
	Mat_<Vec3b>::iterator it = image.begin<Vec3b>();
	Mat_<Vec3b>::iterator itend = image.end<Vec3b>();

	// scan all pixels
	for (; it != itend; ++it) {
		(*it)[0] &= mask;
		(*it)[0] += div2;
		(*it)[1] &= mask;
		(*it)[1] += div2;
		(*it)[2] &= mask;
		(*it)[2] += div2;
	}
}

int main() {
	Mat lena = imread("lena.tif", IMREAD_COLOR);
	namedWindow("original");
	namedWindow("after");

	imshow("original", lena);
	colorReduction(lena, 64);
	imshow("after", lena);
	waitKey(0);
}