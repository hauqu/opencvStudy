#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>

//opencv4.5.0，c++

#include<iostream>
#include<vector>
/*
通 181 孙乐乐 189064245
*/
using namespace std;
using namespace cv;//opencv 命名空间

void printImg(Mat& imgGray);
//打印灰度图像各个像素的值

void dilation_piewise_linearization(Mat imgGray,
	int a, int b, int c, int d, int L);
//分段线性化函数

void dilation_histogram_equalization(Mat imgGray);
//直方图均衡化的图像增强

void showHistogram(Mat img,string window);
//仅能显示灰度图像的直方图 size [800X600]比较合适
int main(int argc,char** argv)
{
	string path = "Resources/smile.jpg";
	//图片路径 相对路径
	Mat img = imread(path);
	//读取图片内容
	//resize(img, img, Size(), 0.1, 0.1);

	Mat imgGray,imgDil1,imgDil2;
	
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//原始图像生成灰度图像
	imgDil1 = imgGray.clone();
	imgDil2 = imgGray.clone();
	//深拷贝

	dilation_piewise_linearization(imgDil1, 100, 100, 0, 254, 255);
	//分度线性化增强，各参数见教材
	dilation_histogram_equalization(imgDil2);
	//直方图均衡化增强

	imshow("img", img);
	imshow("imgGray", imgGray);
	imshow("piewise_linearization", imgDil1);
	imshow("histogram_equalization", imgDil2);
	//显示图像

	showHistogram(imgDil2, "dil");
	showHistogram(imgGray, "ori");
	//显示直方图
	
	waitKey(0);
	//每帧时间 0 为等待按键触发刷新
	return 0;
}
void printImg(Mat& imgGray)
{
	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			cout << (int)imgGray.at<uchar>(i, j) << " ";
		}cout << endl;
	}
}



void dilation_piewise_linearization(Mat imgGray, int a,int b,int c,int d,int L)
{
	//分段线性化
	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			uchar fxy = imgGray.at<uchar>(i, j);

			if(fxy<L) 
			{
				if(fxy<b)
				{
					if(fxy<a)
					{
						imgGray.at<uchar>(i, j) = (c / a) * fxy;
					}else
					{
						imgGray.at<uchar>(i, j)=(((d - c) / (b - a)) * (fxy - a)) + c;
					}
				}else
				{
					imgGray.at<uchar>(i, j) = ((L - 1 - d) / (L - 1 - b)) * (fxy - b) + d;
				}
			}
		}
	}
}

void dilation_histogram_equalization(Mat imgGray)
{
	
	//直方图均衡化，灰度值取 256阶 ,由于 概率时小数运算精度损失，采用整数运算
	vector<int>rk(256, 0);//rk
	vector<int>nk(256, 0);//nk
	vector<int>skj(256, 0);
	vector<int>skb(256, 0);
	

	int n = imgGray.rows * imgGray.cols;//数据总量
	int dn = n / 255;//一份占有的数量

	for (int i = 0; i < rk.size(); i++)
	{
		rk[i] = dn * (i+1);
	}//rk 用绝对数量表示

	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			uchar fxy = imgGray.at<uchar>(i, j);
			nk[fxy]++; 
		}
	}//统计各个灰度出现的次数。



	skj[0] = nk[0];
	for (int i = 1; i < skj.size(); i++)
	{
		skj[i] = nk[i]+skj[i-1];
	}//累加次数

	
	for (int i = 0; i < skb.size(); i++)
	{
		int r = skj[i] / dn;
		int t = skj[i] - dn * r;
		if (t > (dn / 2)) r++;
		skb[i] = r;
	}//归并

	

	for (int i = 1; i < skb.size(); i++)
	{
		if(skb[i]==0)
		{
			skb[i] = skb[i - 1];
		}//合并查询
	}

	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			uchar fxy = imgGray.at<uchar>(i, j);
			imgGray.at<uchar>(i, j) = skb[fxy];
		}
	}//修改增强
	
}

void showHistogram(Mat img,string window)
{
	// 0-255
	vector<int>rk(256,0);
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
	
	Mat imgH(800, 256*4, CV_8UC3, Scalar(64, 64, 64));//创建一个图像
	putText(imgH, window, Point(400, 100), FONT_HERSHEY_DUPLEX, 2, Scalar(0, 0, 128), 2);
	for (int i = 0; i < rk.size(); i++)
	{
		//绘制直线加顶端圆做出直方图效果
		line(imgH, Point(i * 4, 800), Point(i * 4, 800 - rk[i]),Scalar(255,0,0));
		circle(imgH, Point(i *4,800-rk[i]), 3, Scalar(255, 255, 0),FILLED);
	}
	imshow(window, imgH);
	//显示图像
}