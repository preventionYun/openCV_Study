#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

#define PI 3.1415926

class EdgeDetector {
private:
	// original image
	Mat img;
	// 16-bit signed int image
	Mat sobel;
	// Aperture size of the Sobel kernel
	int aperture;
	// Sobel magnitude
	Mat sobelMagnitude;
	// Sobel orientation
	Mat sobelOrientation;

public:
	EdgeDetector() : aperture(3) {}

	// Set the aperture size of the kernel
	void setAperture(int a) {
		aperture = a;
	}

	// Get the aperture size of the kernel
	int getAperture() const {
		return aperture;
	}

	// Compute the Sobel
	void computeSobel(const Mat& image) {
		Mat sobelX;
		Mat sobelY;

		// Compute Sobel
		Sobel(image, sobelX, CV_32F, 1, 0, aperture);
		Sobel(image, sobelY, CV_32F, 0, 1, aperture);

		imwrite("sobelX.bmp", sobelX);
		imwrite("sobelY.bmp", sobelY);

		// Compute magnitude and orientation
		cartToPolar(sobelX, sobelY, sobelMagnitude, sobelOrientation);
	}

	// Compute the Sobel
	void computeSobel(const Mat& image, Mat &sobelX, Mat &sobelY) {
		// Compute Sobel
		Sobel(image, sobelX, CV_32F, 1, 0, aperture);
		Sobel(image, sobelY, CV_32F, 0, 1, aperture);

		// Compute magnitude and orientation
		cartToPolar(sobelX, sobelY, sobelMagnitude, sobelOrientation);
	}

	// Get Sobel magnitude
	Mat getMagnitude() {
		return sobelMagnitude;
	}

	// Get Sobel orientation
	Mat getOrientation() {
		return sobelOrientation;
	}

	// Get a thresholded binary map
	Mat getBinaryMap(double threshold) {
		Mat bin;
		cv::threshold(sobelMagnitude, bin, threshold, 255, THRESH_BINARY_INV);

		return bin;
	}

	// Get a CV_8U image of the Sobel
	Mat getSobelImage() {
		Mat bin;

		double minval, maxval;
		minMaxLoc(sobelMagnitude, &minval, &maxval);
		sobelMagnitude.convertTo(bin, CV_8U, 255 / maxval);

		return bin;
	}

	// Get a CV_8U image of the Sobel orientation
	// 1 gray-level = 2 degrees
	Mat getSobelOrientationImage() {
		Mat bin;
		
		sobelOrientation.convertTo(bin, CV_8U, 90 / PI);

		return bin;
	}
};

int main() {
	Mat input_gray = imread("road.jpg", IMREAD_GRAYSCALE);
	
	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(input_gray);

	// Apply canny algorithm
	Mat Canny_contours;
	Canny(input_gray,	// gray-level image
		Canny_contours, // output contours
		125,			// low threshold
		350);			// high threshold (255 * rootsquare(2) = 360.624..)

	imwrite("road_gray.bmp", input_gray);
	imwrite("Sobel_magnitude.bmp", ed.getMagnitude());
	imwrite("Sobel_orientation.bmp", ed.getSobelOrientationImage());
	imwrite("Sobel_low_threshold.bmp", ed.getBinaryMap(125));
	imwrite("Sobel_high_threshold.bmp", ed.getBinaryMap(350));	
	imwrite("Canny_contours.bmp", 255 - Canny_contours);
}