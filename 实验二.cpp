#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace cv;
using namespace std;
string path;
Mat img, img_gray, img_median_filter,img_average_filter;
enum class TemplateSize
{
	P3_3 = 3,
	P5_5 =5,
	P7_7 =7,
};

void median_filter(Mat imgScr,Mat& dst,TemplateSize pt);
void average_filter(Mat imgScr, Mat& dst,TemplateSize pt);
int main()
{
	path = "Resources/F.png";
	img = imread(path);
	//resize(img, img, Size(800, 800));
	cvtColor(img, img_gray,COLOR_BGR2GRAY);
	
	median_filter(img_gray, img_median_filter, TemplateSize::P7_7);
	average_filter(img_gray, img_average_filter, TemplateSize::P7_7);
	imwrite("F/imgGray.png", img_gray);
	imwrite("F/img.png", img);
	imwrite("F/imgAverageFilter.png",img_average_filter);
	imwrite("F/imgMedianFilter.png", img_median_filter);
	imshow("img", img);
	imshow("imgGray", img_gray);
	imshow("imgmf", img_median_filter);
	imshow("imgaf", img_average_filter);
	waitKey(0);
	return 0;
}

void median_filter(Mat imgScr, Mat& dst, TemplateSize pt)
{
	
	cout << "rows:" << imgScr.rows << endl;
	cout << "cols:" << imgScr.cols << endl;
//	cout << (int)imgScr.at<uchar>(imgScr.rows-1, imgScr.cols-1) << endl;
	imgScr.copyTo(dst);
	int lu = (int)pt/2;//空出的边界

	for (int i = lu; i < imgScr.rows-lu; i++)
	{
		for (int j = lu; j < imgScr.cols-lu; j++)
		{
			vector<int>t;
			for (int s = -lu; s <= lu; s++)
			{
				for (int e = -lu; e <= lu; e++)
				{
					int temp = (int)imgScr.at<uchar>(i + s, j + e);
					t.push_back(temp);
				}
			}
			sort(t.begin(), t.end());
			int m = ((int)pt * (int)pt)/2;
			dst.at<uchar>(i, j) = (uchar)t[m];
		}
	}

}
void average_filter(Mat imgScr, Mat& dst, TemplateSize pt)
{
	
	cout << "rows:" << imgScr.rows << endl;
	cout << "cols:" << imgScr.cols << endl;
	imgScr.copyTo(dst);
	//cout << (int)imgScr.at<uchar>(imgScr.rows - 1, imgScr.cols - 1) << endl;
	int lu = (int)pt / 2;//空出的边界

	for (int i = lu; i < imgScr.rows - lu; i++)
	{
		for (int j = lu; j < imgScr.cols - lu; j++)
		{
			
			int sum = 0;
			for (int s = -lu; s <= lu; s++)
			{
				for (int e = -lu; e <= lu; e++)
				{
					sum += (int)imgScr.at<uchar>(i + s, j + e);
				}
			}
			dst.at<uchar>(i, j) = (uchar)(sum/ ((int)pt * (int)pt));
		}
	}

	
	
}