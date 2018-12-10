#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

Mat SSD(Mat, Mat);
void getMinMaxIntensities(Mat, double *, double *);

int main(int argc, char **argv)
{
    Mat image = imread("./input_images/As3.jpg");

    Mat left = Mat(image, Rect(0, 0, image.cols / 2, image.rows));
    cvtColor(left, left, CV_BGR2GRAY);
    Mat right = Mat(image, Rect(image.cols / 2, 0, image.cols / 2, image.rows));
    cvtColor(right, right, CV_BGR2GRAY);

    // std::cout << left.rows << ":" << left.cols << std::endl;
    // std::cout << right.rows << ":" << right.cols << std::endl;

    Mat Cssd = SSD(left, right);
    // std::cout << Cssd.row(0) << std::endl;
    double min, max;
    getMinMaxIntensities(Cssd, &min, &max);
    Cssd = Cssd / max * 255;
    Cssd.convertTo(Cssd, CV_8U);

    imwrite("./CSSD.jpg", Cssd);
    // std::cout << "Rows: " << left.rows << ", Cols: " << left.cols << std::endl;
    // std::cout << Cssd.rows << ":" << Cssd.cols << std::endl;
    // std::cout << Cssd.row(0) << std::endl;

    // namedWindow("Full Image");
    // imshow("Full Image", image);
    namedWindow("Left Image");
    imshow("Left Image", left);
    namedWindow("Right Image");
    imshow("Right Image", right);
    namedWindow("Output Image");
    imshow("Output Image", Cssd);
    waitKey(0);

    return 0;
}

Mat SSD(Mat first, Mat second)
{
    Mat result(Size(first.cols, first.rows), CV_32F);
    int rangeRight = 0;
    int rangeLeft = 75;
    int rangeUp = 0;
    int rangeDown = 0;
    int windowSize = 7;

    // For all the pixels in the first image
    for (int frow = 0; frow < first.rows; frow++)
    {
        for (int fcol = rangeLeft; fcol < first.cols + rangeLeft; fcol++)
        {
            std::vector<double> ssds = {};
            // Check the pixels in the second image that are within range
            for (int srow = frow - rangeUp; srow <= frow + rangeDown; srow++)
            {
                for (int scol = fcol - rangeLeft; scol <= fcol + rangeRight; scol++)
                {
                    // Compute SSD
                    double sum = 0;
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
                                double sqDiff = (double)first.at<uchar>(efrow, efcol) - (double)second.at<uchar>(esrow, escol);
                                sqDiff *= sqDiff;
                                sum += sqDiff;
                            }
                        }
                    }
                    ssds.push_back(sum);
                }
            }
            double minElement = windowSize * windowSize * 255 * 255;
            for (const double &i : ssds)
            {
                minElement = (i < minElement) ? i : minElement;
            }
            result.at<double>(frow, fcol - rangeLeft) = minElement;
        }
    }
    return result;
}

void getMinMaxIntensities(Mat img, double *min, double *max)
{
    minMaxLoc(img, min, max);
}