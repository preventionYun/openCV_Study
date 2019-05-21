#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat adaptiveThresholding(Mat &image, int inputThreshold = 10) {
	Mat result = image.clone();

	// compute integral image
	Mat integralImage;
	integral(image, integralImage, CV_32S);

	int blockSize = 21;	// size of the neighborhood
	int threshold = 10;	// pixel will be compared to (mean - threshold)

	// for each row
	int halfSize = blockSize / 2;
	for (int j = halfSize; j < image.rows - halfSize - 1; j++) {

		// get the address of row j
		uchar* data = result.ptr<uchar>(j);
		int* idata1 = integralImage.ptr<int>(j - halfSize);
		int* idata2 = integralImage.ptr<int>(j + halfSize + 1);

		// for pixel of a line
		for (int i = halfSize; i < image.cols - halfSize - 1; i++) {

			// compute sum
			int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] -
				idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize*blockSize);

			// apply adaptive threshold
			if (data[i] < (sum - threshold))
				data[i] = 0;
			else
				data[i] = 255;
		}
	}

	return result;
}

int main() {
	Mat image = imread("book.jpg", IMREAD_GRAYSCALE);
	Mat result = adaptiveThresholding(image);
	Mat result2;

	adaptiveThreshold(image,			// input image
		result2,						// output binary image
		255,							// max value for output
		cv::ADAPTIVE_THRESH_MEAN_C,		// adaptive method
		cv::THRESH_BINARY,				// threshold type
		21,								// size of the block
		10);							// threshold used
	
	imwrite("image_adaptiveThresholding.bmp", result);
	imwrite("image_adaptiveThresholding2.bmp", result2);
	

	waitKey(0);
}