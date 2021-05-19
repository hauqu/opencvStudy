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


void threshold_segmentation_otsu(Mat scr, Mat& dst);
//固定阈值
int main()
{
	string path = "Resources/coins.jpg";
	img = imread(path);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//GaussianBlur(imgGray, imgGray, Size(1, 1), 0.5);
	


	


	
		

		imshow("gray", imgGray);
		imshow("img1", img1);
		waitKey(0);
	
	return 0;
}

void threshold_segmentation_otsu(Mat scr, Mat& dst)
{
	scr.copyTo(dst);
	int N = scr.rows * scr.cols;
	const int L = 256;
	vector<int>Ni(0, L);//0-255 灰度阶
	
	vector<double>Wk(0.0, L);
	vector<int>t_Wk(0, L);

	vector<double>Uk(0.0, L);
	vector<int>t_Uk(0, L);
	//计算Ni
	for (int i = 0; i < scr.rows; i++)
	{
		for (int j = 0; j < scr.cols; j++)
		{
			short temp = (short)scr.at<uchar>(i, j);
			Ni[temp]++;
		}
	}

	//计算w（k）
	t_Wk[0] = Ni[0];
	Wk[0] = (double)t_Wk[0] / N;
	for (int i = 1; i < L; i++)
	{
		t_Wk[i] = t_Wk[i - 1] + Ni[i];
		Wk[i] = (double)t_Wk[i] / N;
	}

	//计算u（k）
	t_Uk[0] = 0;
	Uk[0] = 0.0;
	for (int i = 1; i < L; i++)
	{
		t_Uk[i]=i*Ni[i] +t_Uk[i-1];
		Uk[i] = t_Uk[i] / N;
	}
	 
}

