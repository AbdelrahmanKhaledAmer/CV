#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char const *argv[])
{
  //================//
  // Task 1: Part 1 //
  //================//
  Mat image1 = imread("./input_images/img1.png");

  for (int row = 0; row < image1.rows; row++)
  {
    for (int col = 0; col < image1.cols; col++)
    {
      float ratio = (1.0f - (float)col / (float)(image1.cols - 1));
      image1.at<Vec3b>(row, col) *= 6 * ratio;
    }
  }
  //================//
  // Task 1: Part 2 //
  //================//
  Mat image2 = imread("./input_images/img2.jpg");
  float shrinkRatio = (float)image1.rows / (float)image2.rows;
  resize(image2, image2, Size(), shrinkRatio, shrinkRatio);
  flip(image2, image2, 1);
  Mat colsToAppend(image2.rows, image1.cols - image2.cols, CV_8UC3, Scalar(0, 0, 0));
  hconcat(colsToAppend, image2, image2);

  Mat imageFinal1 = 0.75 * image1 + 0.25 * image2;
  //================//
  // Task 2: Part 1 //
  //================//

  String windowName = "My Image trial";

  namedWindow(windowName);
  imshow(windowName, imageFinal1);

  waitKey(0);
  destroyWindow(windowName);
  return 0;
}
