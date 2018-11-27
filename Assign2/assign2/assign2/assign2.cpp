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
	Mat image2 = imread("./imagesA2/2.jpg");
	Mat image3 = imread("./imagesA2/3.jpg");
	Mat image4 = imread("./imagesA2/4.jpg");
	Mat image5 = imread("./imagesA2/5.jpg");
	Mat image6 = imread("./imagesA2/6.jpg");
	Mat image7 = imread("./imagesA2/7.jpg");
	Mat image8 = imread("./imagesA2/8.jpg");

	// double var1 = getVariance2(image1);
	// double var2 = getVariance2(image2);
	// double var3 = getVariance2(image3);
	// double var4 = getVariance2(image4);
	// double var5 = getVariance2(image5);
	// double var6 = getVariance2(image6);
	// double var7 = getVariance2(image7);
	// double var8 = getVariance2(image8);

	// std::cout << "Image   Variance" << std::endl;

	// std::cout << "Image1: " << var1 << std::endl;
	// std::cout << "Image2: " << var2 << std::endl;
	// std::cout << "Image3: " << var3 << std::endl;
	// std::cout << "Image4: " << var4 << std::endl;
	// std::cout << "Image5: " << var5 << std::endl;
	// std::cout << "Image6: " << var6 << std::endl;
	// std::cout << "Image7: " << var7 << std::endl;
	// std::cout << "Image8: " << var8 << std::endl;

	// var1 = getVariance(apply3x3Sobel(image1));
	// var2 = getVariance(apply3x3Sobel(applyMedianFilter(image2, 5)));
	// var3 = getVariance(apply3x3Sobel(image3));
	// var4 = getVariance(apply3x3Sobel(image4));
	// var5 = getVariance(apply3x3Sobel(applyMedianFilter(image5, 5)));
	// var6 = getVariance(apply3x3Sobel(applyMedianFilter(image6, 5)));
	// var7 = getVariance(apply3x3Sobel(image7));
	// var8 = getVariance(apply3x3Sobel(applyMedianFilter(image8, 5)));

	// std::cout << "Image   Sobel Variance" << std::endl;

	// std::cout << "Image1: " << var1 << std::endl;
	// std::cout << "Image2: " << var2 << std::endl;
	// std::cout << "Image3: " << var3 << std::endl;
	// std::cout << "Image4: " << var4 << std::endl;
	// std::cout << "Image5: " << var5 << std::endl;
	// std::cout << "Image6: " << var6 << std::endl;
	// std::cout << "Image7: " << var7 << std::endl;
	// std::cout << "Image8: " << var8 << std::endl;

	// namedWindow("Source", WINDOW_AUTOSIZE);
	// namedWindow("Destination", WINDOW_AUTOSIZE);
	// imshow("Source", image3);
	// imshow("Destination", dst);
	// waitKey(0);

	return 0;
}

double getVariance(Mat img)
{
	double varianceSum = 0;
	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
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
			for (int krow = -1; krow < 2; krow++)
			{
				for (int kcol = -1; kcol < 2; kcol++)
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
			varianceSum += variance;
		}
	}
	double trueVariance = varianceSum / (img.rows * img.cols);
	return trueVariance;
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

	g = absGx + absGy;

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

Mat applyContrastStretching(Mat img, int minIntensity, int maxIntensity)
{
	double minActual, maxActual;
	getMinMaxIntensities(img, &minActual, &maxActual);
	Mat dst = ((img - minActual) * ((maxIntensity - minIntensity) / (maxActual - minActual))) + minIntensity;
	return dst;
}