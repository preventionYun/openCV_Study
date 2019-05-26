#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat cow_binary = imread("cow_binary.bmp", IMREAD_GRAYSCALE);
	Mat castle_grayScale = imread("castle_original.jpg", IMREAD_GRAYSCALE);
	Mat book_grayScale = imread("book_original.jpg", IMREAD_GRAYSCALE);

	Mat eroded, dilated, opened, closed;
	Mat gradient, black_top_hat;
	Mat structuringElement_5x5 = Mat(5, 5, CV_8U, Scalar(1));
	Mat structuringElement_7x7 = Mat(7, 7, CV_8U, Scalar(1));

	// erode
	erode(cow_binary, eroded, structuringElement_5x5);
	// dilate
	dilate(cow_binary, dilated, structuringElement_5x5);
	// closing
	morphologyEx(cow_binary, closed, MORPH_CLOSE, structuringElement_5x5);
	// opening
	morphologyEx(cow_binary, opened, MORPH_OPEN, structuringElement_5x5);
	// gradient
	morphologyEx(castle_grayScale, gradient, MORPH_GRADIENT, Mat());
	gradient.convertTo(gradient, CV_8UC1, -1, 255);
	// black-top-hat
	morphologyEx(book_grayScale, black_top_hat, MORPH_BLACKHAT, structuringElement_7x7);
	black_top_hat.convertTo(black_top_hat, CV_8UC1, -1, 255);

	imwrite("eroded.bmp", eroded);
	imwrite("dilated.bmp", dilated);
	imwrite("closed.bmp", closed);
	imwrite("opened.bmp", opened);
	imwrite("castle_grayScale.bmp", castle_grayScale);
	imwrite("gradient.bmp", gradient);
	imwrite("book_grayScale.bmp", book_grayScale);
	imwrite("black_top_hat.bmp", black_top_hat);
}