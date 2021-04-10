#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
///////////////// images/////////////////////
string css[32] =
{
	"½«","12","13","14","15","16","17","18",
	"19","1A","1B","1C","1D","1E","1F","10",
	"21","22","23","24","25","26","27","28",
	"29","2A","2B","2C","2D","2E","2F","20"
};
void main()
{
	string path = "chess/";//Â·¾¶
	for (int i = 0; i < 32; i++)
	{
		Mat img(100, 100,CV_8UC3,Scalar(64,128,128));

		circle(img, Point(50, 50), 50, Scalar(128, 128, 128), FILLED);
		if (i % 2 == 0) {
			circle(img, Point(50, 50), 40, Scalar(0, 0, 255),FILLED);
		}
		else {
			circle(img, Point(50, 50), 40, Scalar(0, 0, 0), FILLED);
		}
		//putText(img, css[i], Point(30, 60), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1);
		imwrite(path + to_string(i) + ".jpg",img);
	}
	
	waitKey(1000);//µÈ´ý
	

}