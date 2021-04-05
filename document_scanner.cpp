#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
///////////////// 文档扫描 静态图像/////////////////////

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

//A4 的大小X2

Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	// 高斯模糊
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
		//存储形状的角点，长度不超过轮廓点
		
		if (area > 1000) {

			double peri = arcLength(contours[i], true);//封闭长度
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			if(area>maxArea&&conPoly[i].size()==4)
			{
				maxArea = area;
				biggest = { conPoly[i][0],conPoly[i][1],
				conPoly[i][2], conPoly[i][3] };
			}
			
			//drawContours(imgDst, conPoly, i, Scalar(255, 0, 255), 2);
			//绘制轮廓		
			//rectangle(imgDst, boundRect[i], Scalar(0, 255, 0), 3);//绘制跟随框矩形

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

	//已知矩形四点坐标，求顺序 

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
	};//转换前的四点，矩形四角顺序
	
	
	//for (int i = 0; i < 4; i++)
		//circle(img, scr[i], 10, Scalar(0, 255, 0), FILLED);
	//绘制 文档四点


	Point2f dst[4] =
	{
		{0.0,0.0},
		{w,0},
		{0.0,h},
		{w,h}

	};//转换后的四点
	matrix = getPerspectiveTransform(scr, dst);//返回转换模式

	warpPerspective(img, imgWarp, matrix, Point(w, h));
	//返回转换后的图像，该函数完成将 普通视角改为俯视图

	return imgWarp;
	

}
int main()
{
	string path = "Resources/paper.jpg";
	vector<Point>initialPoints;
	vector<Point>docPoints;
	imgOriginal = imread(path);
	resize(imgOriginal, imgOriginal,Size(), 0.5, 0.5);
	
	//预处理
	//得到边缘轮廓
	//处理图像


	
	imgThre= preProcessing(imgOriginal);
	initialPoints= getContours(imgThre, imgOriginal);
	//得到的点顺序不确定，需要排序

	
	//drawPoints(imgOriginal, initialPoints);
	docPoints = reorder(initialPoints);
	//drawPoints(imgOriginal,docPoints);

	//warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	//剪裁
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