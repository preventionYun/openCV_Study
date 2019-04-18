#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat original = imread("lena.tif", IMREAD_GRAYSCALE);
	Mat blurResult;
	Mat gaussianBlurResult;
	
	blur(original, blurResult, Size(5, 5));
	GaussianBlur(original, gaussianBlurResult, Size(5, 5), 1.5);

	cv::Mat gauss = cv::getGaussianKernel(3, 1.5, CV_32F);
	cout << gauss << endl;

	namedWindow("original");
	namedWindow("blurResult");
	namedWindow("GaussianBlurResult");
	imshow("original", original);
	imshow("blurResult", blurResult);
	imshow("gaussianBlurResult", gaussianBlurResult);

	waitKey(0);
}

