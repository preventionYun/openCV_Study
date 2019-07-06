#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#define PI 3.1415926

int main() {
	Mat image = imread("road.jpg", IMREAD_GRAYSCALE);
	
	// Apply Canny algorithm
	Mat contours;
	Canny(image, contours, 125, 350);


	//*************** Basic hough transform *************** //
	Mat BasicHoughTransform;
	image.copyTo(BasicHoughTransform);

	// Hough transform for line detection
	vector<Vec2f> lines;
	HoughLines(contours,	// InputArray
				lines,		// OutputArray
				1,			// rho
				PI / 180,	// theta (step size)
				60);		// threshold (minimum number of votes)

	vector<Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0];	// first element is distance rho
		float theta = (*it)[1]; // second element is angle theta

		if (theta < PI / 4. || theta > 3.*PI / 4.) {	// ~ vertical line
			// point of intersection of the line with first row
			Point pt1(rho / cos(theta), 0);
			// point of intersection of the line with last row
			Point pt2((rho - BasicHoughTransform.rows*sin(theta)) /
				cos(theta), BasicHoughTransform.rows);

			// draw a white line
			line(BasicHoughTransform, pt1, pt2, Scalar(255), 1);
		}
		else {	// ~ horizontal line
			// point of intersection of the line with first column
			Point pt1(0, rho / sin(theta));
			// point of intersection of the line with las column
			Point pt2(BasicHoughTransform.cols,
				(rho - BasicHoughTransform.cols*cos(theta)) / sin(theta));
			
			// draw a white line
			line(BasicHoughTransform, pt1, pt2, Scalar(255), 1);
		}
		++it;
	}

	//*************** Probabilistic hough transform *************** //
	Mat ProbabilisticHoughTransform;
	image.copyTo(ProbabilisticHoughTransform);

	vector<Vec4i> linesP;
	HoughLinesP(contours,	// InputArray
		linesP,				// OutputArray
		1,					// rho
		PI / 180,			// theta (step size)
		60,					// threshold (minimum number of votes)
		100,				// minLength
		20);				// maxGap
		
	// Draw the lines
	Scalar color = Scalar(255, 255, 255);
	vector<Vec4i>::const_iterator it2 = linesP.begin();
	while (it2 != linesP.end()) {
		Point pt1((*it2)[0], (*it2)[1]);
		Point pt2((*it2)[2], (*it2)[3]);
		line(ProbabilisticHoughTransform, pt1, pt2, color);
		++it2;
	}


	//*************** Image write *************** //
	imwrite("grayScale.bmp", image);
	imwrite("contours.bmp", contours);
	imwrite("BasicHoughTransform.bmp", BasicHoughTransform);
	imwrite("ProbabilisticHoughTransform.bmp", ProbabilisticHoughTransform);
}