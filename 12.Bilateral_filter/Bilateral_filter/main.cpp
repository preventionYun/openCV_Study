#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat original = imread("lena.tif", IMREAD_COLOR);
	Mat bilateral, gaussian;

	GaussianBlur(original, gaussian, Size(5, 5), 2.0);
	bilateralFilter(original, bilateral, 5, 100, 100);

	imwrite("original.bmp", original);
	imwrite("gaussain.bmp", gaussian);
	imwrite("bilateral.bmp", bilateral);
}

