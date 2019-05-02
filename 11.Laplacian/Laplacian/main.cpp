#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class myLaplacian {
private:
	// laplacian
	Mat laplace;
	// size of the laplacian kernel
	int kernelSize;

public:
	myLaplacian() : kernelSize(3) {}

	// Set size of the kernel
	void setKernelSize(int inputAperture) {
		kernelSize = inputAperture;
	}

	// computer the floating point Laplacian
	Mat computeLaplacian(const Mat& inputImage) {
		// Compute Laplacian
		Laplacian(inputImage, laplace, CV_32F, kernelSize);
		return laplace;
	}

	/*
	Get the Laplacian result in 8-bit image.
	zero corresponds to gray level 128.
	if no scale is provided, then the max value will be scaled to intensity 255
	You must call computeLaplacian before calling this
	*/
	Mat convertDataRange(double scale = -1.0) {
		if (scale < 0) {
			double laplacianMin, laplacianMax;
			// get min and max laplacian values
			minMaxLoc(laplace, &laplacianMin, &laplacianMax);
			// scale the laplacian to 127
			scale = 127 / max(-laplacianMin, laplacianMax);
		}

		// produce gray-level image
		Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);
		return laplaceImage;
	}

	/*
	Get a binary image of the zero-crossings
	laplacian image should be CV_32F
	negative values in black
	positive values in white
	*/
	Mat getZeroCrossings(Mat laplace) {
		// threshold at 0
		Mat signImage;
		threshold(laplace, signImage, 0, 255, THRESH_BINARY);
		// convert the +/- image into CV_8U
		Mat binary;
		signImage.convertTo(binary, CV_8U);
		// dilate the binary image of +/- regions
		Mat dilated;
		dilate(binary, dilated, Mat());

		// return the zero-crossing contours
		return dilated;
	}
};

int main() {
	Mat image = imread("lena.tif", IMREAD_GRAYSCALE);

	// Compute Laplacian using myLaplacian class
	myLaplacian laplacian;
	laplacian.setKernelSize(7);	// 7x7 laplacian
	Mat laplace = laplacian.computeLaplacian(image);
	Mat laplace_convertDataRange = laplacian.convertDataRange();
	Mat zeroCrossing = laplacian.getZeroCrossings(laplace);

	imwrite("laplace.bmp", laplace);
	imwrite("laplace_convertDataRange.bmp", laplace_convertDataRange);
	imwrite("zeroCrossing.bmp", zeroCrossing);

	// Difference of Gaussians (DoG)
	Mat gauss20, gauss22, dog, dog_zeroCrossing;
	GaussianBlur(image, gauss20, Size(), 2.0);
	GaussianBlur(image, gauss22, Size(), 2.2);
	
	// compute a difference of gaussians
	subtract(gauss20, gauss22, dog, Mat(), CV_32F);
	
	// compute the zero-crossings of DoG
	dog_zeroCrossing = laplacian.getZeroCrossings(dog);
	imwrite("dog_zeroCrossing.bmp", dog_zeroCrossing);
}