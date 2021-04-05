#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>

using namespace std;
using namespace cv;
///////////////// cam face /////////////////////
void main()
{

	VideoCapture cap(2);
	Mat img;
	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");
	//加载模型
	if (plateCascade.empty())
	{
		cout << "XML file load fail" << endl;
	}


	vector<Rect>plates;

	while (true) {

		cap.read(img);
		
		plateCascade.detectMultiScale(img, plates, 1.1, 10);
		for (int i = 0; i < plates.size(); i++) {
			Mat imgCrop = img(plates[i]);
			imshow(to_string(i), imgCrop);

			imwrite("Resources/Plates/" + to_string(i) + ".png",imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(0, 255, 0), 3);
		}
		imshow("img", img);
		
		waitKey(1);//等待，决定每一帧停顿的时间

		
	}
}