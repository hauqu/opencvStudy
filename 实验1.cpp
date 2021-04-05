#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>

#include<iostream>
#include<vector>
/*
ͨ 181 ������ 189064245


*/
using namespace std;
using namespace cv;

void printImg(Mat& imgGray);

void dilation_piewise_linearization(Mat imgGray,
	int a, int b, int c, int d, int L);

void dilation_histogram_equalization(Mat imgGray);
void showHistogram(Mat img,string window);//������ʾ�Ҷ�ͼ��
int main(int argc,char** argv)
{
	string path = "Resources/testimg.jpg";
	Mat img = imread(path);
	//resize(img, img, Size(), 0.1, 0.1);
	Mat imgGray,imgDil1,imgDil2;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	imgDil1 = imgGray.clone();
	imgDil2 = imgGray.clone();
	
	dilation_piewise_linearization(imgDil1, 100, 100, 0, 254, 255);
	
	dilation_histogram_equalization(imgDil2);


	imshow("img", img);
	imshow("piewise_linearization", imgDil1);
	imshow("histogram_equalization", imgDil2);

	imshow("imgGray", imgGray);

	showHistogram(imgDil2, "dil");
	showHistogram(imgGray, "ori");
	
	
	
	
	
	

	
	waitKey(0);

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
	//�ֶ����Ի�
	
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
	
	//ֱ��ͼ���⻯���Ҷ�ֵȡ 256�� ,���� С�����㾫����ʧ��������������
	vector<int>rk(256, 0);//rk
	vector<int>nk(256, 0);//nk
	
	vector<int>skj(256, 0);
	vector<int>skb(256, 0);
	
	//vector<int>sk(256, 0);
	//vector<int>nsk(256, 0);// nsk
	//vector<double>prs(256, 0.0);

	int n = imgGray.rows * imgGray.cols;//��������
	int dn = n / 255;//һ��ռ�е�����

	for (int i = 0; i < rk.size(); i++)
	{
		rk[i] = dn * (i+1);
	}//rk �þ���������ʾ

	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			uchar fxy = imgGray.at<uchar>(i, j);
			nk[fxy]++; 
		}
	}//ͳ�Ƹ����Ҷȳ��ֵĴ�����

	
	/*
	for (int i = 0; i < pr.size(); i++)
	{
		pr[i] = nk[i] / 480000;
	}//ͳ�Ƹ���
	*/


	skj[0] = nk[0];
	for (int i = 1; i < skj.size(); i++)
	{
		skj[i] = nk[i]+skj[i-1];
	}//�ۼӴ���

	
	for (int i = 0; i < skb.size(); i++)
	{
		int r = skj[i] / dn;
		int t = skj[i] - dn * r;
		if (t > (dn / 2)) r++;
		skb[i] = r;
	}//�鲢

	

	for (int i = 1; i < skb.size(); i++)
	{
		if(skb[i]==0)
		{
			skb[i] = skb[i - 1];
		}//�ϲ���ѯ
	}
	for (int i = 0; i < imgGray.rows; i++)
	{
		for (int j = 0; j < imgGray.cols; j++)
		{
			uchar fxy = imgGray.at<uchar>(i, j);
			imgGray.at<uchar>(i, j) = skb[fxy];
		}
	}
	
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
	}
	

	
	Mat imgH(800, 256*4, CV_8UC3, Scalar(64, 64, 64));//����һ��ͼ��
	putText(imgH, window, Point(400, 100), FONT_HERSHEY_DUPLEX, 2, Scalar(0, 0, 128), 2);
	for (int i = 0; i < rk.size(); i++)
	{
		//����

		line(imgH, Point(i * 4, 800), Point(i * 4, 800 - rk[i]),Scalar(255,0,0));
		circle(imgH, Point(i *4,800-rk[i]), 3, Scalar(255, 255, 0),FILLED);
		
	}
	imshow(window, imgH);

}