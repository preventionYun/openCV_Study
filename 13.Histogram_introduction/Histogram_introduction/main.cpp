#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// To create histograms of gray-level images
class Histogram1D {
private:
	int histSize[1];		// number of bins in histogram
	float hranges[2];		// range of values
	const float* ranges[1];	// pointer to the value ranges
	int channels[1];		// channel number to be examined
public:
	Histogram1D() {
		// Prepare default arguments for 1D histogram
		histSize[0] = 256;	// 256 bins
		hranges[0] = 0.0;	// from 0 (inclusive)
		hranges[1] = 256.0;	// to 256 (exclusive)
		ranges[0] = hranges;
		channels[0] = 0;	// we look at channel 0
	}
	// Computes the 1D histogram.
	Mat getHistogram(const Mat &image) {
		Mat hist;
		// Compute 1D histogram with calcHist
		calcHist(&image, 1,	// histogram of 1 image only
			channels,		// the channel used
			Mat(),			// no mask is used
			hist,			// the resulting histogram
			1,				// it is a 1D histogram
			histSize,		// number of bins
			ranges			// pixel value range
		);

		return hist;
	}

	// Computes the 1D histogram and returns an image of it.
	Mat getHistogramImage(const Mat &image, int zoom = 1) {
		// Compute histogram first
		Mat hist = getHistogram(image);
		// Creates histogram image
		return createImageOfHistogram(hist, zoom);
	}

	// Create an image representing a histogram (static method)
	static Mat createImageOfHistogram(const Mat &hist, int zoom) {
		// Get min and max bin values
		double maxVal = 0;
		double minVal = 0;
		minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		// get histogram size
		int histSize = hist.rows;

		// Square image on which to display histogram
		Mat histImg(histSize *zoom, histSize *zoom, CV_8U, Scalar(255));

		// set highest point at 90% of nbins (i.e. image height)
		int hpt = static_cast<int>(0.9 * histSize);

		// Draw vertical line for each bin
		for (int h = 0; h < histSize; h++) {
			float binVal = hist.at<float>(h);
			if (binVal > 0) {
				int intensity = static_cast<int>(binVal * hpt / maxVal);
				line(histImg, Point(h *zoom, histSize * zoom), Point(h * zoom, (histSize - intensity) * zoom), Scalar(0), zoom);
			}
		}

		return histImg;
	}

	Mat stretchingHistogram(const Mat &image, const Mat &hist, double percentile = 0.8) {
		// number of pixels in percentile
		float number = image.total()*percentile;

		// find end point of the histogram
		int endPoint = 0;
		for (float count = 0.0; endPoint < 256; endPoint++) {
			// number of pixel at endPoint 
			if ((count += hist.at<float>(endPoint)) >= number)
				break;
		}

		// find start point of the histogram
		int startPoint = 255;
		for (float count = 0.0; startPoint >= 0; startPoint--) {
			// number of pixel at start point
			if ((count += hist.at<float>(startPoint)) >= number)
				break;
		}

		// Create lookup table
		int dims[1] = { 256 };
		Mat lookup(1, dims, CV_8U);

		for (int i = 0; i < 256; i++) {
			if (i < startPoint) lookup.at<uchar>(i) = 0;
			else if (i > endPoint) lookup.at<uchar>(i) = 255;
			else lookup.at<uchar>(i) = cvRound(255.0*(i - startPoint) / (endPoint - startPoint));
		}

		// the output image
		Mat result;

		// apply lookup table
		LUT(image, lookup, result);
		return result;
	}
};

int main() {
	Mat image = imread("group.jpg", IMREAD_GRAYSCALE);

	// The histogram object
	Histogram1D h;

	// Compute the histogram
	Mat histogramData = h.getHistogram(image);

	// Loop over each bin
	for (int i = 0; i < 256; i++) {
		cout << "Value " << i << " = " << histogramData.at<float>(i) << endl;
	}

	// Display a histogram as an image
	Mat histogram_image = h.getHistogramImage(image);

	// Binary map
	Mat thresholded;
	threshold(image, thresholded, 70, 255, THRESH_BINARY);
	
	// Stretching a histogram
	Mat stretchedImage = h.stretchingHistogram(image, histogramData, 0.95);
	Mat histogram_stretched = h.getHistogramImage(stretchedImage);

	// Histogram equalization
	Mat equalizedImage;
	equalizeHist(image, equalizedImage);
	Mat histogram_equlized = h.getHistogramImage(equalizedImage);

	imwrite("original.bmp", image);
	imwrite("histogram_image.bmp", histogram_image);
	imwrite("thresholded.bmp", thresholded);
	imwrite("stretchedImage.bmp", stretchedImage);
	imwrite("histogram_stretched.bmp", histogram_stretched);
	imwrite("equalizedImage.bmp", equalizedImage);
	imwrite("histogram_equalized.bmp", histogram_equlized);
}