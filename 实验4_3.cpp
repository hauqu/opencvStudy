/*
固定阈值分割、
最大类间方差（Otsu）方法、
最佳熵自动门限方法，
并比较不同算法分割的效果及影响因素

*/
#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace std;
using namespace cv;

Mat img, imgGray, img1;

void showHistogram(Mat img, string window);

void threshold_segmentation1(Mat scr, Mat& dst, short T);

//固定阈值
int main()
{
	string path = "Resources/jimu_singlecolor.jpg";
	img = imread(path);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgGray, Size(1, 1), 0.5);

	showHistogram(imgGray, "h");
	
	
	imshow("gray", imgGray);
	imshow("img1", img1);
	waitKey(0);

	return 0;
}



void showHistogram(Mat img, string window)
{
	// 0-255
	vector<int>rk(256, 0);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			uchar fxy = img.at<uchar>(i, j);
			rk[fxy]++;
		}
	}

	for (auto i = rk.begin(); i != rk.end(); i++)
	{
		(*i) = (*i) / 10;
	}//数据过大，除以10 后处理

	Mat imgH(800, 256 * 4, CV_8UC3, Scalar(64, 64, 64));//创建一个图像
	putText(imgH, window, Point(400, 100), FONT_HERSHEY_DUPLEX, 2, Scalar(0, 0, 128), 2);
	for (int i = 0; i < rk.size(); i++)
	{
		//绘制直线加顶端圆做出直方图效果
		line(imgH, Point(i * 4, 800), Point(i * 4, 800 - rk[i]), Scalar(255, 0, 0));
		circle(imgH, Point(i * 4, 800 - rk[i]), 3, Scalar(255, 255, 0), FILLED);
		if (i % 10 == 0)
		{
			line(imgH, Point(i * 4, 0), Point(i * 4, 800 - rk[i]), Scalar(0, 125, 0));
		}
		if (i % 50 == 0)
		{
			line(imgH, Point(i * 4, 0), Point(i * 4, 800 - rk[i]), Scalar(125, 0, 0));
			putText(imgH, to_string(i), Point(i * 4 - 25, 20), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0));
		}

	}
	imshow(window, imgH);
	//显示图像
}
void threshold_segmentation1(Mat scr, Mat& dst, short T)
{
	scr.copyTo(dst);
	for (int i = 0; i < scr.rows; i++)
	{
		for (int j = 0; j < scr.cols; j++)
		{
			short temp = (short)scr.at<uchar>(i, j);
			if (temp <= T)
			{
				dst.at<uchar>(i, j) = 0;
			}
			else
			{
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}
