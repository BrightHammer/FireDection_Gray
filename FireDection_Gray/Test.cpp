//��һ��ͼ����ж�ֵ��
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat imag, result;
	imag = imread("C:\\Users\\Bright_Hammer\\Desktop\\Lab\\����ʶ��\\Picture\\11.jpg", 0);	//������Ĳ�ɫͼ��ֱ���ԻҶ�ͼ�����
	namedWindow("ԭͼ", 1);
	imshow("ԭͼ", imag);
	result = imag.clone();
	//���ж�ֵ������ѡ��30��200.0Ϊ��ֵ
	threshold(imag, result, 200, 255.0, CV_THRESH_BINARY);
	namedWindow("��ֵ��ͼ��");
	imshow("��ֵ��ͼ��", result);
	waitKey();
	return 0;
}
