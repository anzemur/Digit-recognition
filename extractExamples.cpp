#include "extractExamples.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"


using namespace cv;
using namespace std;

Size resizeSize(RESIZE_SIZE,RESIZE_SIZE);


/*! Compute the Euclidean distance between two points
*/
static double distanceBtwPoints(const cv::Point2f &a, const cv::Point2f &b)
{
	double xDiff = a.x - b.x;
	double yDiff = a.y - b.y;

	return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

/*
* Applies CLAHE algorithm to srcArry to increase the contrast
*/
static void applyCLAHE(Mat srcArry, Mat dstArry) {
	//Function that applies the CLAHE algorithm to "dstArry".

	if (srcArry.channels() >= 3) {
		// READ RGB color image and convert it to Lab
		Mat channel;
		cvtColor(srcArry, dstArry, COLOR_BGR2Lab);

		// Extract the L channel
		extractChannel(dstArry, channel, 0);

		// apply the CLAHE algorithm to the L channel
		Ptr<cv::CLAHE> clahe = cv::createCLAHE();
		clahe->setClipLimit(4);
		clahe->apply(channel, channel);

		insertChannel(channel, dstArry, 0);

		cvtColor(dstArry, dstArry, COLOR_Lab2BGR);

		channel.release();
	}

}

// Find the mass center of all the pixels in the image with colour below COLOR_THRESHOLD
Point findMassCenter(Mat src)
{
	int totalX = 0, totalY = 0;
	int cnt = 0;
	for (int x = 0; x < src.cols; x++)
	{
		for (int y = 0; y < src.rows; y++)
		{
			int val = src.at<uchar>(Point(x, y));
			if (val < COLOR_THRESHOLD)
			{
				totalX += x;
				totalY += y;
				cnt++;
			}
		}
	}
	return Point(totalX / cnt, totalY / cnt);
}

// Moves the mass center of all the pixels to the centre of an image
Mat moveToMassCenter(Mat src)
{
	// Find the center
	Point center = findMassCenter(src);

	// Create a 2-times bigger matrix than src
	Mat bigger(RESIZE_SIZE * 2, RESIZE_SIZE * 2, COLOR_BGR2GRAY);
	bigger = 255;

	// Copy the src to the bigger with the src mass center in the middle of the bigger
	src.copyTo(bigger(cv::Rect(RESIZE_SIZE - 1 - center.x, RESIZE_SIZE - 1 - center.y, src.cols, src.rows)));
	
	// Cut the centre of the image to return a matrix with the same size as src
	Mat ret = bigger(Rect(RESIZE_SIZE / 2 - 1, RESIZE_SIZE / 2 - 1, RESIZE_SIZE, RESIZE_SIZE));
	return ret;
}

int main(int argc, char** argv)
{
	Mat src, gray, rotated, cropped;

	// Read the image from the path
	src = imread("D:/Documents/Faks/MM/digit-recognition/example.jpg");
	if (src.empty())
		return -1;

	// Conver to gray
	cvtColor(src, gray, COLOR_BGR2GRAY);
	gray = gray < 200;

	// Find all the contours in the picture
	vector<vector<Point> > contours;
	findContours(gray.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect _minAreaRect;

	int count = 0;

	// Find all the rectanges in the picture
	for (size_t i = 0; i < contours.size(); ++i)
	{
		_minAreaRect = minAreaRect(Mat(contours[i]));
		Point2f pts[4];
		_minAreaRect.points(pts);

		// Calculate the width and height of the rotated rectangle
		double dist0 = distanceBtwPoints(pts[0], pts[1]);
		double dist1 = distanceBtwPoints(pts[1], pts[2]);

		// Use only rectangles that have a size bigger than MIN_RECT_SIZE
		if ((dist0 > MIN_RECT_SIZE && dist1 > MIN_RECT_SIZE))
		{

			Point2f rePoint(0, 0);

			// Remove the edges of the cut image to remove the borders of the rectangle
			Size2f redSize(EDGE_PIXELS_REMOVED, EDGE_PIXELS_REMOVED);

			// Rotate the image to the right orientation
			Mat M = getRotationMatrix2D(_minAreaRect.center + rePoint, _minAreaRect.angle, 1.0);
			warpAffine(src, rotated, M, src.size(), INTER_CUBIC);

			// Crop the resulting image
			getRectSubPix(rotated, _minAreaRect.size - redSize, _minAreaRect.center + rePoint, cropped);
			Point2f croppedCenter(cropped.cols / 2.0F, cropped.rows / 2.0F);
			Mat rot_mat = getRotationMatrix2D(croppedCenter, 0, 1.0);
			Mat dst, res, gray_res;
			warpAffine(cropped, dst, rot_mat, cropped.size());
			applyCLAHE(dst, dst);

			// Conver to grayscale
			cvtColor(dst, gray_res, COLOR_BGR2GRAY);

			//Resize the image
			resize(gray_res, res, resizeSize);

			// Paint the edges white (in some cases the redSize isn't enough)
			for (int k = 0; k < res.cols; k++)
			{
				res.row(0).col(k) = 255;
				res.row(res.rows - 1).col(k) = 255;
				res.row(k).col(0) = 255;
				res.row(k).col(res.cols - 1) = 255;


			}
			
			// Move to the mass centre
			Point massCenter = findMassCenter(res);
			res = moveToMassCenter(res);

			//Save the image to the folder
			imwrite(SAVE_FOLDER_PATH + std::to_string(count) + ".jpeg", res);
			count++;
		}
	}
	return 0;
}