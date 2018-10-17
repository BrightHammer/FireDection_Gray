/*利用最大间方差法求合适的阈值*/
#pragma once
#include<opencv2/opencv.hpp>
#include<cv.h>
using namespace cv;

extern double getThreshVal_Otsu_8u(const Mat& _src);