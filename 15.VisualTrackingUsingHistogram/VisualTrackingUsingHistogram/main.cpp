#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// To create histograms of gray-level images
class Histogram1D {

private:

	int histSize[1];         // number of bins in histogram
	float hranges[2];        // range of values
	const float* ranges[1];  // pointer to the different value ranges
	int channels[1];         // channel number to be examined

public:

	Histogram1D() {

		// Prepare default arguments for 1D histogram
		histSize[0] = 256;   // 256 bins
		hranges[0] = 0.0;    // from 0 (inclusive)
		hranges[1] = 256.0;  // to 256 (exclusive)
		ranges[0] = hranges;
		channels[0] = 0;     // we look at channel 0
	}

	// Sets the channel on which histogram will be calculated.
	// By default it is channel 0.
	void setChannel(int c) {

		channels[0] = c;
	}

	// Gets the channel used.
	int getChannel() {

		return channels[0];
	}

	// Sets the range for the pixel values.
	// By default it is [0,256[
	void setRange(float minValue, float maxValue) {

		hranges[0] = minValue;
		hranges[1] = maxValue;
	}

	// Gets the min pixel value.
	float getMinValue() {

		return hranges[0];
	}

	// Gets the max pixel value.
	float getMaxValue() {

		return hranges[1];
	}

	// Sets the number of bins in histogram.
	// By default it is 256.
	void setNBins(int nbins) {

		histSize[0] = nbins;
	}

	// Gets the number of bins in histogram.
	int getNBins() {

		return histSize[0];
	}

	// Computes the 1D histogram.
	cv::Mat getHistogram(const cv::Mat &image) {

		cv::Mat hist;

		// Compute 1D histogram with calcHist
		cv::calcHist(&image,
			1,			// histogram of 1 image only
			channels,	// the channel used
			cv::Mat(),	// no mask is used
			hist,		// the resulting histogram
			1,			// it is a 1D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}


	// Computes the 1D histogram and returns an image of it.
	cv::Mat getHistogramImage(const cv::Mat &image, int zoom = 1) {

		// Compute histogram first
		cv::Mat hist = getHistogram(image);

		// Creates image
		return Histogram1D::getImageOfHistogram(hist, zoom);
	}

	// static methods
	// Create an image representing a histogram
	static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom) {

		// Get min and max bin values
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		// get histogram size
		int histSize = hist.rows;

		// Square image on which to display histogram
		cv::Mat histImg(histSize*zoom, histSize*zoom, CV_8U, cv::Scalar(255));

		// set highest point at 90% of nbins (i.e. image height)
		int hpt = static_cast<int>(0.9*histSize);

		// Draw vertical line for each bin
		for (int h = 0; h < histSize; h++) {

			float binVal = hist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal*hpt / maxVal);
				cv::line(histImg, cv::Point(h*zoom, histSize*zoom),
					cv::Point(h*zoom, (histSize - intensity)*zoom), cv::Scalar(0), zoom);
			}
		}

		return histImg;
	}
};

template <typename T, int N>
class IntegralImage {
	Mat integralImage;

public:
	IntegralImage(Mat image) {
		// (costly) computation of the integral image
		integral(image, integralImage, DataType<T>::type);
	}

	// compute sum over sub-regions of any size from 4 pixel accesses
	Vec<T, N> operator() (int xo, int yo, int width, int height) {
		// window at (xo, yo) of size width by height
		return (integralImage.at<Vec<T, N>>(yo + height, xo + width)
			- integralImage.at<Vec<T, N>>(yo + height, xo)
			- integralImage.at<Vec<T, N>>(yo, xo + width)
			+ integralImage.at<Vec<T, N>>(yo, xo));
	}
};

// convert to a multi-channel image made of binary planes
// nPlanes must be a power of 2
void convertToBinaryPlanes(const Mat& input, Mat& output, int nPlanes) {
	// number of bits to mask out
	int n = 8 - static_cast<int>(log(static_cast<double>(nPlanes)) / log(2.0));

	// mask used to eliminate least significant bits
	uchar mask = 0xFF << n;

	// create a vetor of binary images
	vector<Mat> planes;
	// reduce to nBins by eliminating least significant bits
	Mat reduced = input & mask;

	// compute each binary image plane
	for (int i = 0; i < nPlanes; i++) {
		// 1 for each pixel equals to i<<shift
		planes.push_back((reduced == (i << n)) & 0x1);
	}

	// create multi-channel image
	merge(planes, output);
}

int main() {
	// open image
	Mat image = imread("bike55.bmp", IMREAD_GRAYSCALE);
	Mat secondImage = imread("bike65.bmp", IMREAD_GRAYSCALE);

	// define image roi
	int xo = 97, yo = 112;
	int width = 25, height = 30;
	Mat roi(image, Rect(xo, yo, width, height));

	// ************** loop, integral example. same result **************  //
	// compute sum
	// returns a Scalar to work with multi-channel images
	Scalar sum = cv::sum(roi);
	cout << sum[0] << endl;

	// compute integral image
	Mat integralImage;
	integral(image, integralImage, CV_32S);
	// get sum over an area using three additions/subractions
	int sumInt = integralImage.at<int>(yo + height, xo + width)
		- integralImage.at<int>(yo + height, xo)
		- integralImage.at<int>(yo, xo + width)
		+ integralImage.at<int>(yo, xo);
	cout << sumInt << endl;
	// ***************************************************************** //

	// histogram of 16 bins
	Histogram1D h;
	h.setNBins(16);
	// compute histogram over image roi
	Mat refHistogram = h.getHistogram(roi);

	// show reference histogram
	namedWindow("Reference Histogram");
	imshow("Reference Histogram", h.getHistogramImage(roi, 20));

	// first create 16-plane binary image
	Mat planes;
	convertToBinaryPlanes(secondImage, planes, 16);
	// then compute integral image
	IntegralImage<float, 16> intHistogram(planes);

	// perform the search
	double maxSimilarity = 0.0;
	int xbest, ybest;

	// loop over a horizontal strip around girl
	// location in initial image
	for (int y = 110; y < 120; y++) {
		for (int x = 0; x < secondImage.cols - width; x++) {
			// compute histogram of 16 bins using integral image
			Vec<float, 16> histogram = intHistogram(x, y, width, height);
			// compute distance with reference histogram
			double distance = compareHist(refHistogram, histogram, CV_COMP_INTERSECT);
			// find position of most similar histogram
			if (distance > maxSimilarity) {
				xbest = x;
				ybest = y;
				maxSimilarity = distance;
			}
		}
	}

	// draw rectangle at searching roi
	rectangle(secondImage, Rect(0, 110, secondImage.cols, 10 + height), 255);
	// draw rectangle at best location
	rectangle(secondImage, Rect(xbest, ybest, width, height), 255);
	imwrite("result.bmp", secondImage);

	waitKey(0);
}