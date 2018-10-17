#include<opencv2/opencv.hpp>
#include <iostream>
#include <ctype.h>
#include<cv.h>
#include "Otsu.h"

using namespace std;
using namespace cv;
Mat FireDection(const Mat &inImg);
static void onMouse(int event, int x, int y, int, void*);
void on_Trackbar(int);
void Fire(const Mat bwImg);

bool selectObject = false;	//鼠标按下时为true，松开为false
//bool bNewTh = false;
int trackObject = 0;
Point origin;	//鼠标选中的起点
Rect selection;//选中的区域：左上角顶点坐标+宽+高
int grayMax = 128;
Mat srcImg;// = imread("C:\\Users\\Bright_Hammer\\Desktop\\Lab\\火焰识别\\Picture\\12.jpg");
//源图像，不可更改
bool fireWindow = false;//火焰检测结果的窗口是否已经打开
double fireGray;//当前区域的灰度平均值
bool fireExist = false;//该区域是否存在火焰

const char* keys =
{
	"{1|  | 0 | camera number}"
};

int main()
{
	VideoCapture cap;
	cap.open("C:\\Users\\Bright_Hammer\\Desktop\\火焰视频女朋友剪\\3.mpg");
	if (!cap.isOpened())
	{
		cout << "不能初始化摄像头\n";
	}
	namedWindow("FireDection Demo", 1);
	setMouseCallback("FireDection Demo", onMouse, 0);//设置鼠标的回调函数
	cvCreateTrackbar("阈值调整", "FireDection Demo", &grayMax, 256, on_Trackbar);//设置灰度阈值调整的条

	//主循环
	Mat frame, fireImg;//frame用于显示蓝色框,fireImg为二值化之后的图像
	cout << "请调整适当的阈值，并框选出火焰区域" << endl;
	while (1) {
		cap >> srcImg;
		if (srcImg.empty())
			break;
		fireImg = FireDection(srcImg).clone();
		if (!(selection.width > 0 && selection.height > 0))
			imshow("FireDection Demo", srcImg);
		else {
			frame = srcImg.clone();
			rectangle(frame, selection, Scalar(255, 0, 0));
			if (!frame.empty())
				imshow("FireDection Demo", frame);
			else
				cout << "error::in line 66" << endl;
			waitKey(10);
			if (fireWindow)
				Fire(fireImg);
		}
		if (trackObject < 0 && !fireWindow)//如果鼠标松开，开始执行
		{
			Fire(fireImg);
			trackObject = 1;
		}
		waitKey(5);
	}
	waitKey(1);
	return 0;
}

//--------------------------------【火焰检测函数】------------------------------------
//		描述：判断截取的部分火焰是否熄灭
//-------------------------------------------------------------------------------------------------
void Fire(const Mat bwImg)
{
	Mat Img;
	Mat smallImg;
	if (selection.width > 0 && selection.height > 0)
		smallImg = bwImg(selection);
	else{
		cout << "error\nerror\nerrorerror\nerror\nerrorerror\nerror\nerrorerror\nerror\nerror" << endl;
		return;
	}
	Img = smallImg.clone();
	namedWindow("检测结果", 0);
	if(!Img.empty())
		imshow("检测结果", Img);
	else
		cout << "error::in line 90" << endl;
	waitKey(1);
	IplImage tmp = IplImage(Img);
	fireGray = cvAvg((CvArr*)&tmp).val[0];
	cout << "框选成功，当前区域的平均灰度为" << fireGray << endl;
	if (fireGray > 10)
		fireExist = true;
	else
		fireExist = false;
	cout << "当前火焰状态:" << fireExist << endl;
	fireWindow = true;
}

//--------------------------------【火焰检测函数】------------------------------------
//		描述：检测火焰，返回一张二值化图像
//-------------------------------------------------------------------------------------------------
Mat FireDection(const Mat &inImg)
{
	Mat grayImg, bwImg;
	static double grayTh = 200;//灰度的阈值
	cvtColor(inImg, grayImg, CV_BGR2GRAY);//转灰度图

	grayTh = getThreshVal_Otsu_8u(grayImg);//利用otsu法得到合适的阈值
	grayTh = grayTh * ((double)grayMax / 512 + 1.75);
	if (grayTh >= 255)
		grayTh = 240;
	else if (grayTh < 160)
		grayTh = 180;
	cout << "当前灰度阈值为为为：：：：：：：：：：：：：：：：：：：" << grayTh << endl;
	//bNewTh = false;

	bwImg = grayImg.clone();
	threshold(grayImg, bwImg, grayTh, 255.0, CV_THRESH_BINARY);
	namedWindow("二值化图像");
	if(!bwImg.empty())
		imshow("二值化图像", bwImg);
	else
		cout << "error::in line 127" << endl;
	cout << "当前灰度阈值为为为：：：：：：：：：：：：：：：：：：：" << grayTh << endl;
	//DrawFire(inImg, bwImg);
	waitKey(1);
	return bwImg;
}

//--------------------------------【onMouse( )回调函数】------------------------------------
//		描述：鼠标操作回调
//-------------------------------------------------------------------------------------------------
static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		//selection &= Rect(0, 0, srcImg.cols, srcImg.rows);
	}

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN://按下鼠标
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		trackObject = 0;
		break;
	case CV_EVENT_LBUTTONUP://松开鼠标
		selectObject = false;
		//if (!bNewTh)
		//	bNewTh = true;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		break;
	}
}

//--------------------------------【Trackbar 回调函数】------------------------------------
//		描述：阈值条回调函数,更新检测结果显示
//-------------------------------------------------------------------------------------------------
void on_Trackbar(int)
{
	Mat tempImg;
	if (srcImg.data) {
		tempImg = FireDection(srcImg).clone();
	}
	if (fireWindow) {
		//tempImg = tempImg(selection);
		Fire(tempImg);
		/*namedWindow("debug", 0);
		imshow("debug", tempImg);
		IplImage tmp = IplImage(tempImg);
		cout << "当前区域平均灰度：" << cvAvg((CvArr*)&tmp).val[0] << endl;*/
	}
}


