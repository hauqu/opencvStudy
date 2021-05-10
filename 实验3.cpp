
/*
��ǿ�Ҷ�ͼ��ı�Ե
�����ٰ�����
�ݶ�����
Roberts���ӡ�
Sobel���ӡ�
������˹����
��LOG���ӣ�
���Ƚϲ�ͬ�㷨��ȡ��Ե��Ч����Ӱ�����ء�
*/
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace cv;
using namespace std;
string path;
Mat img, img_gray,img1,img2,img3,img4,img5,img6;

void getCanny(Mat scr, Mat& dst, vector<vector<short>>& t, short l,short h);
//����ģ��ʹ��

void RobertsCanny(Mat scr, Mat &dst,short l,short h);//ģ����ż�����������
/*
l,h�������������ڱ�Ե����ǿ �������� h ����ǿΪ255 С��l ������0

*/
vector<vector<short>>Roberts =
{
	{0,-1 },
	{1,0}
};

vector<vector<short>>Prewitt45 = 
{
	{-1, -1, 0},
	{-1,  0, 1},
	{ 0,  1, 1}
};
vector<vector<short>>Prewitt_45 =
{
	{0, 1, 1},
	{- 1, 0, 1 },
	{-1, -1, 0}
};
vector<vector<short>>  Sobel45=
{
	{-2,-1,0},
	{-1,0,1},
	{0,1,2}
};
vector<vector<short>> Sobel_45=
{
	{0,1,2},
	{-1,0,1},
	{-2,-1,0}
};

vector<vector<short>>  LOG5X5=
{
	{-2, -4, -4, -4, -2},
	{-4, 0, 8, 0, -4},
	{-4, 8, 24, 8, -4},
	{-4, 0, 8, 0, -4},
	{-2, -4, -4, -4, -2}
};
int main()
{
	path = "Resources/shapes.png";
	img = imread(path);
	resize(img, img, Size(400, 400));
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	GaussianBlur(img_gray, img_gray,Size(3,3),3,0);
	getCanny(img_gray, img1, Sobel45,25,50);
	getCanny(img_gray, img2, Sobel_45,25,50);
	getCanny(img_gray, img3, LOG5X5,50,100);
	RobertsCanny(img_gray, img4,10,20);

	getCanny(img_gray, img5, Prewitt45, 75, 150);
	getCanny(img_gray, img6, Prewitt_45, 75, 150);


	imshow("org", img);
	imshow("Gray", img_gray);
	imshow("soble45", img1);
	imshow("soble-45", img2);
	imshow("LOG5X5", img3);
	imshow("roberts", img4);

	imshow("prewitt45", img5);
	imshow("prewitt-45", img6);
	waitKey(0);
	return 0;
}
void getCanny(Mat scr, Mat& dst, vector<vector<short>>& t, short l, short h)
{
	scr.copyTo(dst);
	int s1 = t.size() / 2;
	int s2 = t[0].size() / 2;

	for (int i = s1; i < scr.rows - s1; i++)
	{
		
		for (int j = s2; j < scr.cols - s2; j++)
		{
			int temp = 0;
			for (int s = -s1; s <= s1; s++)
			{
				for (int e = -s2; e <= s2; e++)
				{
					temp = temp + (int)t[s+s1][e+s2] * (int)scr.at<uchar>(i + s, j + e);
				}
			}
			if (temp>h)
			{
				temp = 255;
			}else
			{
				if (temp<l)
				{
					temp = 0;
				}
			}
			dst.at<uchar>(i, j) = (uchar)temp;
		}
	}
}

void RobertsCanny(Mat scr, Mat& dst, short l, short h)
{
	scr.copyTo(dst);
	/*
	{0,-1 },
	{1,0}
	*/
	for (int i = 0; i < scr.rows-1; i++)
	{
		for (int j = 0; j < scr.cols-1; j++)
		{
			int temp = (int)scr.at<uchar>(i, j+1)- (int)scr.at<uchar>(i+1, j);
			if (temp > h)
			{
				temp = 255;
			}
			else
			{
				if (temp < l)
				{
					temp = 0;
				}
			}
			dst.at<uchar>(i, j) = temp;
		}
	}
}