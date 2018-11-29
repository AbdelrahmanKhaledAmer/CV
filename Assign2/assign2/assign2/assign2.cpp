// Abdelrahman Khaled Amer 34-9791 T-15
// Mohamed Ahmed Mohamed 34-6141 T-11
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

double getVariance(Mat);
void getMinMaxIntensities(Mat, double *, double *);
Mat apply3x3Sobel(Mat);
Mat applyMedianFilter(Mat, int);
Mat applyUnsharpFilter(Mat, double);
Mat applyContrastStretching(Mat);
double getPixelVariance(Mat img, int row, int col);
double measureNoise(Mat img);
Mat fixNoise(Mat img);
double measureBlur(Mat img);
Mat fixBlur(Mat img);
Mat process(Mat img);


int main(int argc, char **argv)
{
	Mat image1 = imread("./imagesA2/1.jpg");
	Mat image2 = imread("./imagesA2/2.jpg");
	Mat image3 = imread("./imagesA2/3.jpg");
	Mat image4 = imread("./imagesA2/4.jpg");
	Mat image5 = imread("./imagesA2/5.jpg");
	Mat image6 = imread("./imagesA2/6.jpg");
	Mat image7 = imread("./imagesA2/7.jpg");
	Mat image8 = imread("./imagesA2/8.jpg");

	
	Mat images[] =  {image1,image2,image3,image4,image5,image6,image7,image8 };
	for (int i = 1; i <= 8; i++)
	{
		Mat dst = process(images[i-1]);
		imwrite("./outputImagesA2/modified_"+ std::to_string(i)+".jpg", dst);
		printf("\n\n");
	}
	 /*namedWindow("Source", WINDOW_NORMAL);
	 namedWindow("Destination", WINDOW_NORMAL);
	 resizeWindow("Source", 600, 500);
	 resizeWindow("Destination", 600, 500);
	 imshow("Source", image8);*/
	 //Mat dst = process(image8);
	 //imshow("Destination", dst);

	 //imwrite("./outputImagesA2/modified_8.jpg", dst);
	 waitKey(0);

	return 0;
}


double getPixelVariance(Mat img, int row, int col)
{
	double varianceSum = 0;

	double mean = 0;
	int counter = 0;
	for (int krow = -1; krow < 2; krow++)
	{
		for (int kcol = -1; kcol < 2; kcol++)
		{
			int erow = row + krow;
			int ecol = col + kcol;
			if (erow >= 0 && erow < img.rows && ecol >= 0 && ecol < img.cols)
			{
				mean += img.at<uchar>(erow, ecol);
				counter++;
			}
		}
	}
	mean /= counter;

	double variance = 0;
	counter = 0;
	for (int krow = -2; krow < 3; krow++)
	{
		for (int kcol = -2; kcol < 3; kcol++)
		{
			int erow = row + krow;
			int ecol = col + kcol;
			if (erow >= 0 && erow < img.rows && ecol >= 0 && ecol < img.cols)
			{
				double tmp = img.at<uchar>(erow, ecol) - mean;
				variance += tmp * tmp;
				counter++;
			}
		}
	}
	variance /= counter;
	return variance;
}


double measureNoise(Mat img) {

	// threshold used = 150
	double threshold = 150;
	double noisyPixels = 0;
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			//printf("px var: %.2f\n", getPixelVariance(img, row, col));
			if (getPixelVariance(img, row, col) > threshold)
				noisyPixels += 1;
		}
	}
	noisyPixels = noisyPixels / (double)(img.rows * img.cols);
	printf("amount of noise in the image %.2f\n", noisyPixels);
	return noisyPixels;

}
Mat fixNoise(Mat img) {

	// threshold 0.6

	if (measureNoise(img) > 0.6) {
		printf("removing salt and pepper\n");
		return applyMedianFilter(img, 5);
	}
	return img;
}

double measureBlur(Mat img) {
	Mat imgSobel = apply3x3Sobel(img);
	double cnt = img.rows*img.cols;
	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			if (imgSobel.at<uchar>(row, col) < 70)
				cnt--;
		}

	}
	printf("percent of blurry pixels %.2f\n", 1-cnt / (double)(img.rows*img.cols));
	return cnt / (double)(img.rows*img.cols);
}


Mat fixBlur(Mat img) {

	if (measureBlur(img) < 0.25)
	{
		printf("fixing blurry image\n");
		return applyUnsharpFilter(img, 0.7);
	}
	return img;
}


Mat process(Mat img) {
	return fixBlur(applyContrastStretching(fixNoise(img)));
}


void getMinMaxIntensities(Mat img, double *min, double *max){
	minMaxLoc(img, min, max);
}

Mat apply3x3Sobel(Mat img){
	Mat g, gx, gy, absGx, absGy;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	Sobel(img, gx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(img, gy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

	convertScaleAbs(gx, absGx);
	convertScaleAbs(gy, absGy);

	g = absGx + absGy;

	return g;
}

Mat applyMedianFilter(Mat img, int size)
{
	Mat dst;
	medianBlur(img, dst, size);
	return dst;
}

Mat applyUnsharpFilter(Mat img, double k)
{
	if (k < 0.2)
	{
		k = 0.2;
	}
	else if (k > 0.7)
	{
		k = 0.7;
	}
	Mat smoothed;
	blur(img, smoothed, Size(7, 7), Point(-1, -1));
	Mat edges = img - smoothed;
	Mat sharped = img + k * edges;
	return sharped;
}

Mat applyContrastStretching(Mat img)
{
	double minActual, maxActual, maxIntensity = 255, minIntensity = 0; 
	getMinMaxIntensities(img, &minActual, &maxActual);
	printf("min %.2f, max %.2f, color range %.2f \n", minActual, maxActual,(maxActual-minActual)/(double)255);
	if ((maxActual - minActual) / (double)255 < 1) {
		printf("fixing contrast\n");
	}
	Mat dst = ((img - minActual) * ((maxIntensity - minIntensity) / (maxActual - minActual))) + minIntensity;
	return dst;
}
