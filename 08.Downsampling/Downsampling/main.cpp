#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat downSampling(Mat input, int scaleFactor) {
	Mat output = Mat(Size(input.cols / scaleFactor, input.rows / scaleFactor), CV_8UC1);
	
	for (int row = 0; row < output.rows; row++) {
		for (int col = 0; col < output.cols; col++) {
			output.at<uchar>(row, col) = input.at<uchar>(row * scaleFactor, col * scaleFactor);
		}
	}
	return output;
}


int main() {
	Mat original = imread("lena.tif", IMREAD_GRAYSCALE);
	Mat gaussian;

	GaussianBlur(original, gaussian, Size(11, 11), 2.0);

	Mat withoutFilter = downSampling(original, 2);
	Mat withGaussianFilter = downSampling(gaussian, 2);

	imwrite("original.bmp", original);
	imwrite("withoutFilter.bmp", withoutFilter);
	imwrite("withGaussian.bmp", withGaussianFilter);
	/****************************************/
	Mat reducedImage; 
	Mat increasedImage;

	pyrDown(original, reducedImage); 
	pyrUp(original, increasedImage);

	imwrite("pyrDown.bmp", reducedImage);
	imwrite("pyrUp.bmp", increasedImage);
	/****************************************/
	Mat resizedImage;
	Mat resizedImage2;
	Mat resizedImage3;

	resize(original, resizedImage, Size(3000, 3000));
	resize(original, resizedImage2, Size(), 0.5, 0.5);
	resize(original, resizedImage3, Size(), 0.5, 0.5, INTER_NEAREST);

	imwrite("resizedImage.bmp", resizedImage);
	imwrite("resizedImage2.bmp", resizedImage2);
	imwrite("resizedImage3.bmp", resizedImage3);
}