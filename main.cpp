#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat getAffinedMatrix(Mat src, Mat dst, Point2f srcTri[3], Point2f dstTri[3])
{
  Mat dstMat(dst.rows, dst.cols, CV_8UC3, Scalar(0, 0, 0));
  Mat myAff = getAffineTransform(srcTri, dstTri);
  warpAffine(src, dstMat, myAff, dstMat.size());
  for (int row = 0; row < dst.rows; row++)
  {
    for (int col = 0; col < dst.cols; col++)
    {
      if (dstMat.at<Vec3b>(row, col) != Vec3b(0, 0, 0))
      {
        dst.at<Vec3b>(row, col) = dstMat.at<Vec3b>(row, col);
      }
    }
  }
  return dst;
}

Mat getHomographiedMatrix(Mat src, Mat dst, Point2f srcCor[4], Point2f dstCor[4])
{
  Mat dstMat(dst.rows, dst.cols, CV_8UC3, Scalar(0, 0, 0));
  Mat myAff = getPerspectiveTransform(srcCor, dstCor);
  warpPerspective(src, dstMat, myAff, dstMat.size());
  for (int row = 0; row < dst.rows; row++)
  {
    for (int col = 0; col < dst.cols; col++)
    {
      if (dstMat.at<Vec3b>(row, col) != Vec3b(0, 0, 0))
      {
        dst.at<Vec3b>(row, col) = dstMat.at<Vec3b>(row, col);
      }
    }
  }
  return dst;
}

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
  imwrite("./output_images/Q1O.jpg", imageFinal1);

  //================//
  // Task 2: Part 1 //
  //================//
  Mat image3 = imread("./input_images/img3.jpg");
  Mat image4 = imread("./input_images/img4.jpg");
  //{bottom right , Bottom Left, Top Right}
  Point2f srcTri[3] = {Point2f(image3.cols, image3.rows), Point2f(0, image3.rows), Point2f(image3.cols, 0)};
  Point2f dstTri[3] = {Point2f(1310, 516), Point2f(1220, 516), Point2f(1310, 378)};

  image4 = getAffinedMatrix(image3, image4, srcTri, dstTri);
  imwrite("./output_images/Q2O1.jpg", image4);

  //================//
  // Task 2: Part 2 //
  //================//
  Mat image5 = imread("./input_images/img5.jpg");
  Point2f srcTri2[3] = {Point2f(image3.cols, image3.rows), Point2f(0, image3.rows), Point2f(image3.cols, 0)};
  Point2f dstTri2[3] = {Point2f(663, 557), Point2f(326, 525), Point2f(705, 128)};

  image5 = getAffinedMatrix(image3, image5, srcTri2, dstTri2);
  imwrite("./output_images/Q2O2.jpg", image5);


  //================//
  // Task 3: Part 1 //
  //================//
  Mat image6 = imread("./input_images/img6.jpg");

  Point2f srcPts[4] = { Point2f(0, image3.rows), Point2f(image3.cols, 0), Point2f(image3.cols, image3.rows),Point2f(0, 0) };
  Point2f dstPts[4] = { Point2f(158, 389), Point2f(468, 70), Point2f(463, 353), Point2f(164, 37) };

  Mat image6Bad = getAffinedMatrix(image3, image6, srcPts, dstPts);
  imwrite("./output_images/Q3O1.jpg", image6Bad);


  //================//
  // Task 3: Part 2 //
  //================//

  image6 = getHomographiedMatrix(image3, image6, srcPts, dstPts);
  // String windowName = "My Image trial";
  // namedWindow(windowName);
  // imshow(windowName, image6);
//image Q3O2 
  imwrite("./output_images/Q3O2.jpg", image6);

  waitKey(0);
  // destroyWindow(windowName);
  return 0;
}
