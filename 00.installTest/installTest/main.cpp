#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image;
	
	// read image
	image = imread("dog.jpg");

	if (image.empty()) {	// error handling, no image
		cout << "loading image fail";
	}
	else {
		namedWindow("dog");
		imshow("dog", image);
	}

	waitKey(0);
	return 0;
}
