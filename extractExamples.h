#pragma once
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"


using namespace cv;
using namespace std;

#define MIN_RECT_SIZE 30 
#define RESIZE_SIZE 32
#define COLOR_THRESHOLD 240
#define EDGE_PIXELS_REMOVED 15
#define SAVE_FOLDER_PATH "C:/Users/Likewse/Documents/digit-recognition/digits/"

static double distanceBtwPoints(const cv::Point2f &a, const cv::Point2f &b);
static void applyCLAHE(Mat srcArry, Mat dstArry);
Point findMassCenter(Mat src);
Mat moveToMassCenter(Mat src);