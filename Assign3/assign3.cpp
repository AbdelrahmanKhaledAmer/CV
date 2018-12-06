#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

Mat SSD(Mat, Mat);
void getMinMaxIntensities(Mat, double *, double *);
Mat applyContrastStretching(Mat, int, int);

int main(int argc, char **argv)
{
    Mat image = imread("./input_images/As3.jpg");

    Mat left = Mat(image, Rect(0, 0, image.cols / 2, image.rows));
    Mat right = Mat(image, Rect(image.cols / 2, 0, image.cols / 2, image.rows));

    // std::cout << left.rows << ":" << left.cols << std::endl;
    // std::cout << right.rows << ":" << right.cols << std::endl;

    Mat Cssd = applyContrastStretching(SSD(left, right), 0, 255);
    Cssd.convertTo(Cssd, CV_8U);

    // std::cout << Cssd.rows << ":" << Cssd.cols << std::endl;
    // std::cout << Cssd << std::endl;

    // namedWindow("Full Image");
    // imshow("Full Image", image);
    // namedWindow("Left Image");
    // imshow("Left Image", left);
    // namedWindow("Right Image");
    // imshow("Right Image", right);
    namedWindow("Output Image");
    imshow("Output Image", Cssd);
    waitKey(0);

    return 0;
}

Mat SSD(Mat first, Mat second)
{
    Mat result(Size(first.cols, first.rows), CV_16U);
    int rangeRight = 0;
    int rangeLeft = 75;
    int rangeUp = 0;
    int rangeDown = 0;
    int windowSize = 7;

    // For all the pixels in the first image
    for (int frow = 0; frow < first.rows; frow++)
    {
        for (int fcol = 0; fcol < first.cols; fcol++)
        {
            std::vector<unsigned int> ssds = {};
            // Check the pixels in the second image that are within range
            for (int srow = frow - rangeLeft; srow <= frow + rangeRight; srow++)
            {
                for (int scol = fcol - rangeUp; scol <= fcol + rangeDown; scol++)
                {
                    // Compute SSD
                    unsigned int sum = 0;
                    for (int row = -windowSize / 2; row <= windowSize / 2; row++)
                    {
                        for (int col = -windowSize / 2; col <= windowSize / 2; col++)
                        {
                            int efrow = frow + row;
                            int efcol = fcol + col;
                            int esrow = srow + row;
                            int escol = scol + col;
                            if (efrow >= 0 && efrow < first.rows && efcol >= 0 && efcol < first.cols && esrow >= 0 && esrow < second.rows && escol >= 0 && escol < second.cols)
                            {
                                // std::cout << "Effective Row First: " << efrow << std::endl;
                                // std::cout << "Effective Col First: " << efcol << std::endl;
                                // std::cout << "Effective Row Second: " << esrow << std::endl;
                                // std::cout << "Effective Col Second: " << escol << std::endl;
                                int sqDiff = first.at<uchar>(efrow, efcol) - second.at<uchar>(esrow, escol);
                                sqDiff *= sqDiff;
                                sum += sqDiff;
                            }
                        }
                    }
                    ssds.push_back(sum);
                }
            }
            unsigned int maxElement = 0;
            for (const unsigned int &i : ssds)
            {
                maxElement = (i > maxElement) ? i : maxElement;
            }
            result.at<unsigned int>(frow, fcol) = maxElement;
        }
    }
    return result;
}

void getMinMaxIntensities(Mat img, double *min, double *max)
{
    minMaxLoc(img, min, max);
}

Mat applyContrastStretching(Mat img, int minIntensity, int maxIntensity)
{
    double minActual, maxActual;
    getMinMaxIntensities(img, &minActual, &maxActual);
    Mat dst = ((img - minActual) * ((maxIntensity - minIntensity) / (maxActual - minActual))) + minIntensity;
    return dst;
}