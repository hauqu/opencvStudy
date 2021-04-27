#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<iostream>
#include<algorithm>
#include<stack>
using namespace cv;
using namespace std;
string path;
Mat img, img_gray, img_median_filter,img_average_filter;
enum class TemplateSize
{
	P3_3 = 9,
	P5_5 =25,
};

void median_filter(Mat imgScr,Mat dst);
void average_filter(Mat imgScr, Mat dst);
int main()
{
	path = "Resources/smile.jpg";
	img = imread(path);
	cvtColor(img, img_gray,COLOR_BGR2GRAY);
	median_filter(img_gray, img_median_filter);
	average_filter(img_gray, img_average_filter);
	return 0;
}

void median_filter(Mat imgScr, Mat dst)
{
	imgScr.copyTo(dst);

	cout << "rows:" << imgScr.rows << endl;
	cout << "cols:" << imgScr.cols << endl;
	cout << (int)imgScr.at<uchar>(imgScr.rows-1, imgScr.cols-1) << endl;
	for (int i = 1; i < imgScr.rows-1; i++)
	{
		for (int j = 1; j < imgScr.cols-1; j++)
		{
			vector<int>t =
			{
				(int)imgScr.at<uchar>(i - 1,j - 1),(int)imgScr.at<uchar>(i + 0,j - 1),(int)imgScr.at<uchar>(i + 1,j - 1),
				(int)imgScr.at<uchar>(i - 1,j + 0),(int)imgScr.at<uchar>(i + 0,j + 0),(int)imgScr.at<uchar>(i + 1,j + 0),
				(int)imgScr.at<uchar>(i - 1,j + 1),(int)imgScr.at<uchar>(i + 0,j + 1),(int)imgScr.at<uchar>(i + 1,j + 1)
			};
			sort(t.begin(), t.end());
			dst.at<uchar>(i, j) = (uchar)t[4];
		}
	}

}
void average_filter(Mat imgScr, Mat dst)
{
	imgScr.copyTo(dst);

	cout << "rows:" << imgScr.rows << endl;
	cout << "cols:" << imgScr.cols << endl;
	cout << (int)imgScr.at<uchar>(imgScr.rows - 1, imgScr.cols - 1) << endl;
	for (int i = 1; i < imgScr.rows - 1; i++)
	{
		for (int j = 1; j < imgScr.cols - 1; j++)
		{
			
			int sum =
				(int)imgScr.at<uchar>(i - 1, j - 1) + (int)imgScr.at<uchar>(i + 0, j - 1) + (int)imgScr.at<uchar>(i + 1, j - 1) +
				(int)imgScr.at<uchar>(i - 1, j + 0) + (int)imgScr.at<uchar>(i + 0, j + 0) + (int)imgScr.at<uchar>(i + 1, j + 0) +
				(int)imgScr.at<uchar>(i - 1, j + 1) + (int)imgScr.at<uchar>(i + 0, j + 1) + (int)imgScr.at<uchar>(i + 1, j + 1)
				;
			
			dst.at<uchar>(i, j) = (uchar)sum/9;
		}
	}

	
	for (int s = -(int)TemplateSize::P3_3/2; s <= (int)TemplateSize::P3_3/2; s++)
	{
		for (int e = -(int)TemplateSize::P3_3 / 2; e <= (int)TemplateSize::P3_3; e++)
		{
			sum += (int)imgScr.at<uchar>(i+s, j + e);
		}
	}
}