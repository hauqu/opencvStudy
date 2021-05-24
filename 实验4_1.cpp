/*
固定阈值分割、

*/
#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img,imgGray,img1;
int Tset = 0;

void threshold_segmentation1(Mat scr, Mat& dst,short T);
//固定阈值
int main()
{
	string path = "Resources/jimu2.jpg";
	img = imread(path);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgGray, Size(1, 1), 0.5);
	namedWindow("Trackbars", (240, 200));


	createTrackbar("Tset", "Trackbars", &Tset, 255);
	

	while (true)
	{
		int t = Tset;

		threshold_segmentation1(imgGray, img1,t);

		imshow("gray", imgGray);
		imshow("img1", img1);
		waitKey(100);
	}
	return 0;
}

void threshold_segmentation1(Mat scr, Mat& dst, short T)
{
	scr.copyTo(dst);
	for (int i = 0; i < scr.rows; i++)
	{
		for (int j = 0; j < scr.cols; j++)
		{
			short temp = (short)scr.at<uchar>(i, j);
			if (temp<=T)
			{
				dst.at<uchar>(i, j) = 0;
			}else
			{
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}
