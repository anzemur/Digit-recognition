#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

//! Compute the distance between two points
/*! Compute the Euclidean distance between two points
*
* @Param a Point a
* @Param b Point b
*/
static double distanceBtwPoints(const cv::Point2f &a, const cv::Point2f &b)
{
	double xDiff = a.x - b.x;
	double yDiff = a.y - b.y;

	return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

static void applyCLAHE(Mat srcArry, Mat dstArry) {
	//Function that applies the CLAHE algorithm to "dstArry".

	if (srcArry.channels() >= 3) {
		// READ RGB color image and convert it to Lab
		Mat channel;
		cvtColor(srcArry, dstArry,COLOR_BGR2Lab);

		// Extract the L channel
		extractChannel(dstArry, channel, 0);

		// apply the CLAHE algorithm to the L channel
		Ptr<cv::CLAHE> clahe = cv::createCLAHE();
		clahe->setClipLimit(4);
		clahe->apply(channel, channel);

		// Merge the the color planes back into an Lab image
		insertChannel(channel, dstArry, 0);

		// convert back to RGB
		cvtColor(dstArry, dstArry, COLOR_Lab2BGR);

		// Temporary Mat not reused, so release from memory.
		channel.release();
	}

}

int main(int argc, char** argv)
{
	Mat src, gray,rotated,cropped;
	src = imread("D:/Documents/Faks/MM/digit-recognition/example.jpg");
	if (src.empty())
		return -1;

	cvtColor(src, gray, COLOR_BGR2GRAY);
	gray = gray < 200;

	vector<vector<Point> > contours;

	findContours(gray.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect _minAreaRect;

	for (size_t i = 0; i < contours.size(); ++i)
	{
		_minAreaRect = minAreaRect(Mat(contours[i]));
		Point2f pts[4];
		_minAreaRect.points(pts);

		double dist0 = distanceBtwPoints(pts[0], pts[1]);
		double dist1 = distanceBtwPoints(pts[1], pts[2]);

		if ((dist0 > 50 && dist1 > 50))
		{

			Point2f rePoint(0, 0);
			Size2f redSize(20, 20);
			Size resizeSize(32, 32);
			Mat M = getRotationMatrix2D(_minAreaRect.center + rePoint, _minAreaRect.angle, 1.0);
			warpAffine(src, rotated, M, src.size(), INTER_CUBIC);
			// crop the resulting image
			getRectSubPix(rotated, _minAreaRect.size - redSize, _minAreaRect.center + rePoint, cropped);
			Point2f croppedCenter(cropped.cols / 2.0F, cropped.rows / 2.0F);
			Mat rot_mat = getRotationMatrix2D(croppedCenter, 90, 1.0);
			Mat dst,res,gray_res;
			warpAffine(cropped, dst, rot_mat, cropped.size());
			//imshow(std::to_string(i), dst);
			/*imshow("nocon", dst);
			applyCLAHE(dst, dst);
			imshow("con", dst);*/

			cvtColor(dst, gray_res, COLOR_BGR2GRAY);
			resize(gray_res, res, resizeSize);
			imwrite("D:/Documents/Faks/Rac_vid/05_learning/build/digits/" + std::to_string(i) + ".jpeg", res);

			for (int j = 0; j < 4; j++)
				line(src, pts[j], pts[(j + 1) % 4], Scalar(0, 0, 255), 2, LINE_AA);
		}
	}
	Mat dst_src;
	resize(src, dst_src,Size(src.cols*0.25,src.rows*0.25));
	imshow("result", dst_src);
	waitKey(0);
	return 0;
}