#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
///////////////// �ĵ�ɨ�� ��̬ͼ��/////////////////////

Mat imgGray,
imgBlur,
imgCanny,
imgDil, 
imgEro,
imgOriginal,
imgThre,
imgWarp,
imgCrop
;

float w = 420;
float h = 560;

//A4 �Ĵ�СX2

Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	// ��˹ģ��
	Canny(imgBlur, imgCanny, 25, 75);
	//
	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(imgCanny, imgDil, kernal);

	 //erode(imgDil, imgEro, kernal);
	

	return imgDil;
}

vector<Point> getContours(Mat imgSrc,Mat imgDst)
{

	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;

	vector<Point>biggest;
	findContours(imgSrc, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	int area = 0; int maxArea=0;
	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i]);
		vector<vector<Point>>conPoly(contours.size());
		//�洢��״�Ľǵ㣬���Ȳ�����������
		
		if (area > 1000) {

			double peri = arcLength(contours[i], true);//��ճ���
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			if(area>maxArea&&conPoly[i].size()==4)
			{
				maxArea = area;
				biggest = { conPoly[i][0],conPoly[i][1],
				conPoly[i][2], conPoly[i][3] };
			}
			
			//drawContours(imgDst, conPoly, i, Scalar(255, 0, 255), 2);
			//��������		
			//rectangle(imgDst, boundRect[i], Scalar(0, 255, 0), 3);//���Ƹ�������

		}
	}
	return biggest;
}
void drawPoints(Mat img,vector<Point>&ps)
{
	for (int i = 0; i < ps.size(); i++)
	{
		circle(img, ps[i], 10, Scalar(0, 255, 0),FILLED);
		putText(img, to_string(i), ps[i], 
			FONT_HERSHEY_PLAIN, 10, Scalar(0, 255, 255, 5));
	}
}
vector<Point>reorder(vector<Point>&ps)
{

	//��֪�����ĵ����꣬��˳�� 

	/*
		0    1

		2    3
	
	*/
	vector<Point>temp;
	vector<int>sumPoints,subPoints;

	int min; int max;
	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(ps[i].x + ps[i].y);
		subPoints.push_back(ps[i].x - ps[i].y);
	}
	
	temp.push_back(ps[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//0
	temp.push_back(ps[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//1
	temp.push_back(ps[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);//2
	temp.push_back(ps[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);//3
	
	return temp;
}


Mat  getWarp(Mat img,vector<Point>&Points,float w,float h)
{
	Mat matrix;
	Point2f scr[4] =
	{
		Points[0],
		Points[1],
		Points[2],
		Points[3]
	};//ת��ǰ���ĵ㣬�����Ľ�˳��
	
	
	//for (int i = 0; i < 4; i++)
		//circle(img, scr[i], 10, Scalar(0, 255, 0), FILLED);
	//���� �ĵ��ĵ�


	Point2f dst[4] =
	{
		{0.0,0.0},
		{w,0},
		{0.0,h},
		{w,h}

	};//ת������ĵ�
	matrix = getPerspectiveTransform(scr, dst);//����ת��ģʽ

	warpPerspective(img, imgWarp, matrix, Point(w, h));
	//����ת�����ͼ�񣬸ú�����ɽ� ��ͨ�ӽǸ�Ϊ����ͼ

	return imgWarp;
	

}
int main()
{
	string path = "Resources/paper.jpg";
	vector<Point>initialPoints;
	vector<Point>docPoints;
	imgOriginal = imread(path);
	resize(imgOriginal, imgOriginal,Size(), 0.5, 0.5);
	
	//Ԥ����
	//�õ���Ե����
	//����ͼ��


	
	imgThre= preProcessing(imgOriginal);
	initialPoints= getContours(imgThre, imgOriginal);
	//�õ��ĵ�˳��ȷ������Ҫ����

	
	//drawPoints(imgOriginal, initialPoints);
	docPoints = reorder(initialPoints);
	//drawPoints(imgOriginal,docPoints);

	//warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	//����
	int cropVal = 10;
	Rect rol(cropVal, cropVal, w - (cropVal * 2), h - (cropVal * 2));

	
	imgCrop = imgWarp(rol);


	imshow("img", imgOriginal);
	imshow("imgThre", imgThre);
	imshow("imgCrop", imgCrop);
	imshow("imgWarp", imgWarp);

	waitKey(0);
	return 0;
}