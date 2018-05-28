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
int FINAL_SIZE = 32;
Size resizeSize(FINAL_SIZE, FINAL_SIZE);
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

Point findMassCenter(Mat src)
{
	int totalX = 0, totalY = 0;
	int cnt = 0;
	for (int x = 0; x < src.cols; x++)
	{
		for (int y = 0; y < src.rows; y++)
		{
			int val = src.at<uchar>(Point(x,y));
			if (val < 240)
			{
				totalX += x;
				totalY += y;
				cnt++;
			}
		}
	}
	return Point(totalX / cnt, totalY / cnt);
}

Mat moveToMassCenter(Mat src)
{
	Point center = findMassCenter(src);
	Mat bigger(FINAL_SIZE * 2, FINAL_SIZE * 2, COLOR_BGR2GRAY);
	bigger = 255;
	
	src.copyTo(bigger(cv::Rect(FINAL_SIZE - 1 - center.x, FINAL_SIZE - 1 - center.y, src.cols, src.rows)));
	//bigger.copyTo(ret(Rect(FINAL_SIZE / 2 - 1, FINAL_SIZE / 2 - 1, FINAL_SIZE * 1.5 - 1, FINAL_SIZE * 1.5 - 1)));
	//Mat ret(FINAL_SIZE, FINAL_SIZE, COLOR_BGR2GRAY);
	Mat ret = bigger(Rect(FINAL_SIZE / 2 - 1, FINAL_SIZE / 2 - 1, FINAL_SIZE, FINAL_SIZE));
	return ret;
}

int main(int argc, char** argv)
{
	Mat src, gray,rotated,cropped;
	src = imread("C:/Users/Likewse/Documents/digit-recognition/66.jpg");
	if (src.empty())
		return -1;

	cvtColor(src, gray, COLOR_BGR2GRAY);
	gray = gray < 200;

	vector<vector<Point> > contours;

	findContours(gray.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	RotatedRect _minAreaRect;
	int cnt = 51;
	for (size_t i = 0; i < contours.size(); ++i)
	{
		_minAreaRect = minAreaRect(Mat(contours[i]));
		Point2f pts[4];
		_minAreaRect.points(pts);

		double dist0 = distanceBtwPoints(pts[0], pts[1]);
		double dist1 = distanceBtwPoints(pts[1], pts[2]);

		if ((dist0 > 30 && dist1 > 30))
		{

			Point2f rePoint(0, 0);
			Size2f redSize(15, 15);
			
			Mat M = getRotationMatrix2D(_minAreaRect.center + rePoint, _minAreaRect.angle, 1.0);
			warpAffine(src, rotated, M, src.size(), INTER_CUBIC);
			// crop the resulting image
			getRectSubPix(rotated, _minAreaRect.size - redSize, _minAreaRect.center + rePoint, cropped);
			Point2f croppedCenter(cropped.cols / 2.0F, cropped.rows / 2.0F);
			Mat rot_mat = getRotationMatrix2D(croppedCenter, 0, 1.0);
			Mat dst,res,gray_res;
			warpAffine(cropped, dst, rot_mat, cropped.size());
			//imshow(std::to_string(i), dst);
			//imshow("nocon", dst);
			applyCLAHE(dst, dst);
			//imshow("con", dst);

			cvtColor(dst, gray_res, COLOR_BGR2GRAY);
			resize(gray_res, res, resizeSize);
			for (int k = 0; k < res.cols; k++)
			{
				res.row(0).col(k) = 255;
				res.row(res.rows - 1).col(k) = 255;
				res.row(k).col(0) = 255;
				res.row(k).col(res.cols - 1) = 255;


			}
			Point massCenter = findMassCenter(res);
			printf("%d %d\n", massCenter.x, massCenter.y);
			res = moveToMassCenter(res);
			//res.row(massCenter.y).col(massCenter.x) = 0;
			imwrite("C:/Users/Likewse/Documents/digit-recognition/digits/" + std::to_string(cnt) + ".jpeg", res);
			cnt++;
			for (int j = 0; j < 4; j++)
				line(src, pts[j], pts[(j + 1) % 4], Scalar(0, 0, 255), 2, LINE_AA);
		}
	}
	Mat dst_src;
	resize(src, dst_src,Size(src.cols*0.25,src.rows*0.25));
	//imshow("result", dst_src);
	waitKey(0);
	return 0;
}