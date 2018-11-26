#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

double getVariance(Mat);
void getMinMaxIntensities(Mat, double *, double *);
Mat apply3x3Sobel(Mat);
Mat applyMedianFilter(Mat, int);
Mat applyUnsharpFilter(Mat, Mat, double);
Mat applyContrastStretching(Mat, int, int);

int main(int argc, char **argv)
{
	 Mat image1 = imread("./imagesA2/1.jpg");
	
	 namedWindow("Source", WINDOW_AUTOSIZE);
	 //namedWindow("Destination", WINDOW_AUTOSIZE);
	 imshow("Source", image1);
	 //imshow("destination", dst);
	 waitKey(0);

	return 0;
}

double getVariance(Mat img)
{
	Scalar mean, dev;
	meanStdDev(img, mean, dev);
	double M = mean.val[0];
	double D = dev.val[0];
	return D * D;
}

void getMinMaxIntensities(Mat img, double *min, double *max)
{
	minMaxLoc(img, min, max);
}

Mat apply3x3Sobel(Mat img)
{
	Mat g, gx, gy, absGx, absGy;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	Sobel(img, gx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(img, gy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

	convertScaleAbs(gx, absGx);
	convertScaleAbs(gy, absGy);

	// TODO: Try different weights
	addWeighted(absGx, 0.5, absGy, 0.5, 0, g);

	return g;
}

Mat applyMedianFilter(Mat img, int size)
{
	Mat dst;
	medianBlur(img, dst, size);
	return dst;
}

Mat applyUnsharpFilter(Mat img, Mat smoothed, double k)
{
	if (k < 0.2)
	{
		k = 0.2;
	}
	else if (k > 0.7)
	{
		k = 0.7;
	}
	Mat edges = img - smoothed;
	Mat sharped = img + k * edges;
	return sharped;
}

// TODO: Test
Mat applyContrastStretching(Mat img, int minIntensity, int maxIntensity)
{
	double minActual, maxActual;
	getMinMaxIntensities(img, &minActual, &maxActual);
	Mat dst = ((img - minActual) * ((maxIntensity - minIntensity) / (maxActual - minActual))) + minIntensity;
	return dst;
}