#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class WatershedSegmenter {
private:
	Mat markers;
public:
	void setMarkers(const Mat& markerImage) {
		// Convert to image of ints
		markerImage.convertTo(markers, CV_32SC1);
	}
	Mat process(const Mat &image) {
		// Apply watershed
		watershed(image, markers);
		return markers;
	}
	Mat getSegmentation() {
		Mat tmp;
		// all segment with label higher than 255 will be assigned value 255
		markers.convertTo(tmp, CV_8UC1);

		return tmp;
	}
	// Return watershed in the form of an image
	Mat getWatersheds() {
		Mat tmp;
		// Each pixel p is transformed into 255p + 255 before conversion
		// boundary = -1, (255 * -1 + 255 = 0) 
		markers.convertTo(tmp, CV_8UC1, 255, 255);

		return tmp;
	}
};

int main() {
	Mat cow_binary = imread("cow_binary.bmp", IMREAD_GRAYSCALE);
	Mat cow_original = imread("cow_original.jpg", IMREAD_COLOR);

	// Eliminate noise and smaller objects
	Mat foreground;
	erode(cow_binary, foreground, Mat(), Point(-1, -1), 4);

	// Identify image pixels without objects
	Mat background;
	dilate(cow_binary, background, Mat(), Point(-1, -1), 4);
	threshold(background, background, 1, 128, THRESH_BINARY_INV);

	// Create markers image
	Mat markers(cow_binary.size(), CV_8UC1, Scalar(0));
	markers = foreground + background;

	// Create watershed segmentation object
	WatershedSegmenter segmenter;

	// Set markers and process
	segmenter.setMarkers(markers);
	Mat watershedResult = segmenter.process(cow_original);

	// getSegmentation
	Mat segmentation = segmenter.getSegmentation();
	// get watershed
	Mat watershed = segmenter.getWatersheds();

	imwrite("foreground.bmp", foreground);
	imwrite("background.bmp", background);
	imwrite("markers.bmp", markers);
	imwrite("segmentation.bmp", segmentation);
	imwrite("watershed.bmp", watershed);
}