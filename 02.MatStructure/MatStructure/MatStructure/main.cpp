#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

// test function that creates an image
Mat getGrayImage() {
	// create image 
	Mat image(Size(500, 500), CV_8U, 50);

	// fucntion instruction
	/** @overload
	@param rows Number of rows in a 2D array.
	@param cols Number of columns in a 2D array.
	@param type Array type. Use CV_8UC1, ..., CV_64FC4 to create 1-4 channel matrices, or
	CV_8UC(n), ..., CV_64FC(n) to create multi-channel (up to CV_CN_MAX channels) matrices.
	@param s An optional value to initialize each matrix element with. To set all the matrix elements to
	the particular value after the construction, use the assignment operator
	Mat::operator=(const Scalar& value) .
	*/
	// Mat(int rows, int cols, int type, const Scalar& s);
	
	// return it
	return image;
}

int main() {
	// create a new image mad of 50 rows and 320 columns
	Mat image(50, 320, CV_8U, 50);
	namedWindow("image");
	imshow("image", image); // show the image
	waitKey(0);	// wait for a key pressed

	// re-allocate a new image
	image.create(Size(200, 200), CV_8U);
	image = 255;
	namedWindow("image_allocated");
	imshow("image_allocated", image);	// show the image
	waitKey(0);	// wait for a key pressed

	//create a red color image
	// channel order is BGR
	Mat image_red(Size(240, 320), CV_8UC3, Scalar(0, 0, 255));
	// or
	// Mat image_red(Size(240, 320), CV_8UC3);
	// image_red = Scalar(0, 0, 255);
	namedWindow("image_red");
	imshow("image_red", image_red);
	waitKey(0);

	// read an image
	Mat lena = imread("lena.tif");
	namedWindow("lena");
	imshow("lena", lena);
	waitKey(0);

	// all these images point to the same data block
	Mat copiedLena(lena);
	image = lena;

	// these images are new copies of the source image
	Mat newCopiedLena;
	lena.copyTo(newCopiedLena);
	Mat newCopiedLena2;
	newCopiedLena2 = lena.clone();

	// transform the image for testing
	flip(lena, lena, -1);
	
	// check which images have been affected by the processing
	imshow("lena", lena);
	imshow("copiedLena", copiedLena);
	imshow("image", image);
	imshow("newCopiedLena", newCopiedLena);
	imshow("newCopiedLena2", newCopiedLena2);
	waitKey(0);

	// get a gray-level image from a function
	Mat gray = getGrayImage();
	imshow("grayImage", gray);	
	waitKey(0);

	// read the image in gray scale
	Mat gray_image = imread("lena.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat gray_image_float;
	gray_image.convertTo(gray_image_float, CV_32F, 1 / 255.0, 0.0);	// CV_32F -> data range : 0.0 ~ 1.0 

	imshow("gray_image_float", gray_image_float);	
	waitKey(0);

	system("pause");
}
