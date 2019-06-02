#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

using namespace std;
using namespace cv;


int main() {
	Mat image = imread("building.jpg", IMREAD_GRAYSCALE);

	// basic MSER detector
	Ptr<MSER> ptrMSER =
		MSER::create(5,		// delta value for local detection
			200,	// min acceptable area
			2000);	// max acceptable area

	// vector of point sets
	vector<vector<Point>> points;
	// vector of rectangles
	vector<Rect> rects;
	// detect MSER features
	ptrMSER->detectRegions(image, points, rects);

	cout << points.size() << " MSERs detected" << endl;

	// create white image
	Mat output(image.size(), CV_8UC3);
	output = Scalar(255, 255, 255);

	// OpenCV random number generator
	RNG rng;

	// Display the MSERs in color areas 
	// for each detected feature
	// reverse order to display the larger MSER first
	for (vector<vector<Point>>::reverse_iterator it = points.rbegin();
		it != points.rend(); ++it) {

		// generate a random color
		Vec3b c(rng.uniform(0, 254), rng.uniform(0, 254), rng.uniform(0, 254));
		// for each point in MSER set
		for (vector<Point>::iterator itPts = it->begin();
			itPts != it->end(); ++itPts) {
			// do not overwrite MSER pixels
			if (output.at<Vec3b>(*itPts)[0] == 255) {
				output.at<Vec3b>(*itPts) = c;
			}
		}
	}

	imwrite("mser.bmp", output);
}