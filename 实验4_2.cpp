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
void threshold_segmentation_otsu(Mat scr, Mat& dst,vector<int>MT);
//固定阈值
int main()
{
	string path = "Resources/ta.jpg";
	img = imread(path);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//GaussianBlur(imgGray, imgGray, Size(1, 1), 0.5);
	
	showHistogram(imgGray, "h");

	


	
		

	imshow("gray", imgGray);
	//imshow("img1", img1);
	waitKey(0);
	
	return 0;
}

void threshold_segmentation_otsu(Mat scr, Mat& dst, vector<int>MT)
{
	scr.copyTo(dst);
	int N = scr.rows * scr.cols;
	const int L = 256;
	int M = MT.size();

	vector<int>Ni(0, L);//0-255 灰度阶
	
	vector<double>Wk(0.0, L);
	vector<int>t_Wk(0, L);

	vector<double>Uk(0.0, L);
	vector<int>t_Uk(0, L);

	
	vector<double>Wj(0.0, M);
	vector<double>Uj(0.0, M);

	int WT = 1;
	double UT =0.0;
	vector<double>Oj2(0.0, M);

	double OW2 = 0.0;
	double OB2 = 0.0;

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

	
	/*
	w(-1) =0
	U(-1) =0;
	
	*/
	//计算Wj
	Wj[0] = Wk[MT[0]];
	for (int j = 1; j < M; j++)
	{
		Wj[j] = Wk[MT[j]] - Wk[MT[j - 1]];
	}

	//计算Uj

	Uj[0] = Uk[MT[0]] / Wj[0];
	for (int j = 1; j < M; j++)
	{
		Uj[j]= (Uk[MT[j]] - Uk[MT[j - 1]])/Wj[j];
	}
	 
	//计算 Oj^2
	for (int j = 0; j < M; j++)
	{
		
		for (int i = MT[j-1]+1; i < MT[j]; i++)
		{
			Oj2[j] +=(i - Uj[j])* (i - Uj[j])* ((double)Ni[i] / N)/Wj[j];
		}
	}

	//计算UT

	for (int j = 0; j < M; j++)
	{
		UT+= Wj[j] * Uj[j];
	}
	//计算 ow^2
	for (int j =0; j < M; j++)
	{
		OW2+= Wj[j] * Oj2[j];
	}
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
		if (i%50==0)
		{
			line(imgH, Point(i * 4, 0), Point(i * 4, 800 - rk[i]), Scalar(125, 0, 0));
			putText(imgH, to_string(i), Point(i * 4-25, 20), FONT_HERSHEY_DUPLEX,1,Scalar(0, 255, 0));
		}
		
	}
	imshow(window, imgH);
	//显示图像
}