#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ColorHistogram {
private:
	int histSize[3];        // size of each dimension
	float hranges[2];       // range of values (same for the 3 dimensions)
	const float* ranges[3]; // array of ranges for each dimension
	int channels[3];        // channel to be considered

public:
	ColorHistogram() {
		// Prepare default arguments for a color histogram
		// each dimension has equal size and range
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;		// BRG range from 0 to 256
		hranges[1] = 256.0;
		ranges[0] = hranges;	// in this class,  
		ranges[1] = hranges;	// all channels have the same range
		ranges[2] = hranges;
		channels[0] = 0;		// the three channels: B
		channels[1] = 1;		// G
		channels[2] = 2;		// R
	}

	// set histogram size for each dimension
	void setSize(int size) {
		// each dimension has equal size 
		histSize[0] = histSize[1] = histSize[2] = size;
	}

	// Computes the histogram.
	Mat getHistogram(const Mat &image) {
		Mat hist;

		// BGR color histogram
		hranges[0] = 0.0;		// BRG range
		hranges[1] = 256.0;
		channels[0] = 0;		// the three channels 
		channels[1] = 1;
		channels[2] = 2;

		// Compute histogram
		calcHist(&image,
			1,			// histogram of 1 image only
			channels,	// the channel used
			Mat(),	// no mask is used
			hist,		// the resulting histogram
			3,			// it is a 3D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}

	// Computes the 1D Hue histogram.
	// BGR source image is converted to HSV
	// Pixels with low saturation are ignored
	Mat getHueHistogram(const Mat &image, int minSaturation = 0) {
		Mat hist;
		// Convert to HSV colour space
		Mat hsv;
		cvtColor(image, hsv, CV_BGR2HSV);

		// Mask to be used (or not)
		Mat mask;
		// creating the mask if required
		if (minSaturation > 0) {
			// Spliting the 3 channels into 3 images
			std::vector<Mat> v;
			split(hsv, v);

			// Mask out the low saturated pixels
			threshold(v[1], mask, minSaturation, 255, THRESH_BINARY);
		}

		// Prepare arguments for a 1D hue histogram
		hranges[0] = 0.0;    // range is from 0 to 180
		hranges[1] = 180.0;
		channels[0] = 0;    // the hue channel 

		// Compute histogram
		calcHist(&hsv,
			1,			// histogram of 1 image only
			channels,	// the channel used
			mask,		// binary mask
			hist,		// the resulting histogram
			1,			// it is a 1D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}
};

class contentFinder {
private:
	// histogram parameters
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold;	// decision threshold
	Mat histogram;		// input histogram

public:
	contentFinder() : threshold(0.1f) {
		// in this class, all channels have the same range
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
	}

	// Sets the reference histogram
	void setHistogram(const Mat& h) {
		histogram = h;
		normalize(histogram, histogram, 1.0);
	}

	void setThreshold(float t) {
		threshold = t;
	}

	// Simplified version in which
	// all channels used, with range [0, 256] by default
	Mat find(const Mat& image) {
		Mat result;
		hranges[0] = 0.0;	// default range [0, 256]
		hranges[1] = 256.0;
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
		return find(image, hranges[0], hranges[1], channels);
	}

	// Finds the pixels belonging to the histogram
	Mat find(const Mat& image, float minValue, float maxValue, int *channels) {
		Mat result;
		hranges[0] = minValue;
		hranges[1] = maxValue;
		// histogram dim matches channel list
		for (int i = 0; i < histogram.dims; i++)
			this->channels[i] = channels[i];

		calcBackProject(&image, 1, // we only use on image
			channels,	// channels used
			histogram,	// the histogram we are using
			result,		// the back projection image
			ranges,		// the range of values for each dimension 
			255.0		// the scaling factor is chosen such that a histogram value of 1 maps to 255
		);

		// Threshold back projection to obtain a binary image
		if (threshold > 0.0) cv::threshold(result, result, 255.0*threshold, 255.0, THRESH_BINARY);

		return result;
	}
};

int main() {
	// Read reference image
	Mat image = imread("baboon01.jpg");
	
	// Baboon's face ROI
	Rect rect_initial(110, 45, 35, 45);
	Mat imageROI = image(rect_initial);
	
	// Get the Hue histogram of baboon's face
	int minSat = 65;
	ColorHistogram hc;
	Mat colorhist = hc.getHueHistogram(imageROI, minSat);
	rectangle(image, rect_initial, Scalar(0, 0, 255));

	contentFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(0.2f);

	imwrite("before_meanShift.bmp", image);

	// ------------
	// Second image
	image = imread("baboon02.jpg");
	// Convert to HSV space
	Mat hsv;
	cvtColor(image, hsv, CV_BGR2HSV);

	// Get back-projection of hue histogram
	int ch[1] = { 0 };
	finder.setThreshold(-1.0f);	// no thresholding
	Mat result = finder.find(hsv, 0.0f, 180.0f, ch);

	// initial window postion
	Rect rect_meanShift(110, 45, 35, 45);

	// search object with mean shift
	TermCriteria criteria(
		TermCriteria::MAX_ITER | TermCriteria::EPS,
		10,	// iterate max 10 times
		1);	// or unitil the change in centroid postion is less than 1px
	
	meanShift(result, rect_meanShift, criteria);
	rectangle(image, rect_initial, Scalar(0, 0, 255));		// draw initial window
	rectangle(image, rect_meanShift, Scalar(0, 255, 0));	// draw output window

	imwrite("backprojection.bmp", result);
	imwrite("meanshift.bmp", image);
}

