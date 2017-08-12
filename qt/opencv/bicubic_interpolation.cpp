

/*================================================================
 *   Copyright (C) 2017年08月12日 肖飞 All rights reserved
 *
 *   文件名称：bicubic_interpolation.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月12日 星期六 13时16分34秒
 *   修改日期：2017年08月12日 星期六 13时21分05秒
 *   描    述：
 *
 *================================================================*/
//#include "bicubic_interpolation.h"
/**********************10-9*******************************
功能：双三次插值缩放图片
数学原理：假设原图像A的大小为m*n,新图像B的大小为M*N
如果我们要求B（X,Y）处的像素值：
我们首先可以得到B（X,Y）在图像A中对应的位置（x,y）=(X*(m/M),Y*(N/n))
这个时候求得的x,y是小数值，我们可以通过这个小数值坐标找到距离最近的16个像素点，
利用所选择的基函数，求出对应的每个像素的权值，最终获得pixelB（X,Y）
**********************************************************/

#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

float a = -0.5;//BiCubic基函数
void getW_x(float w_x[4], float x);
void getW_y(float w_y[4], float y);

int main()
{
	Mat image = imread("lena.jpg");//源图像

	float Row_B = image.rows * 2;
	float Col_B = image.cols * 2;


	Mat biggerImage(Row_B, Col_B, CV_8UC3);

	for (int i = 2; i < Row_B - 4; i++) {
		for (int j = 2; j < Col_B - 4; j++) {
			float x = i * (image.rows / Row_B); //放大后的图像的像素位置相对于源图像的位置
			float y = j * (image.cols / Col_B);

			/*if (int(x) > 0 && int(x) < image.rows - 2 && int(y)>0 && int(y) < image.cols - 2){*/
			float w_x[4], w_y[4];//行列方向的加权系数
			getW_x(w_x, x);
			getW_y(w_y, y);

			Vec3f temp = { 0, 0, 0 };

			for (int s = 0; s <= 3; s++) {
				for (int t = 0; t <= 3; t++) {
					temp = temp + (Vec3f)(image.at<Vec3b>(int(x) + s - 1, int(y) + t - 1)) * w_x[s] * w_y[t];
				}
			}

			biggerImage.at<Vec3b>(i, j) = (Vec3b)temp;
		}
	}

	imshow("image", image);
	imshow("biggerImage", biggerImage);
	waitKey(0);

	return 0;
}

/*计算系数*/
void getW_x(float w_x[4], float x)
{
	int X = (int)x;//取整数部分
	float stemp_x[4];
	stemp_x[0] = 1 + (x - X);
	stemp_x[1] = x - X;
	stemp_x[2] = 1 - (x - X);
	stemp_x[3] = 2 - (x - X);

	w_x[0] = a * abs(stemp_x[0] * stemp_x[0] * stemp_x[0]) - 5 * a * stemp_x[0] * stemp_x[0] + 8 * a * abs(stemp_x[0]) - 4 * a;
	w_x[1] = (a + 2) * abs(stemp_x[1] * stemp_x[1] * stemp_x[1]) - (a + 3) * stemp_x[1] * stemp_x[1] + 1;
	w_x[2] = (a + 2) * abs(stemp_x[2] * stemp_x[2] * stemp_x[2]) - (a + 3) * stemp_x[2] * stemp_x[2] + 1;
	w_x[3] = a * abs(stemp_x[3] * stemp_x[3] * stemp_x[3]) - 5 * a * stemp_x[3] * stemp_x[3] + 8 * a * abs(stemp_x[3]) - 4 * a;
}

void getW_y(float w_y[4], float y)
{
	int Y = (int)y;
	float stemp_y[4];
	stemp_y[0] = 1.0 + (y - Y);
	stemp_y[1] = y - Y;
	stemp_y[2] = 1 - (y - Y);
	stemp_y[3] = 2 - (y - Y);

	w_y[0] = a * abs(stemp_y[0] * stemp_y[0] * stemp_y[0]) - 5 * a * stemp_y[0] * stemp_y[0] + 8 * a * abs(stemp_y[0]) - 4 * a;
	w_y[1] = (a + 2) * abs(stemp_y[1] * stemp_y[1] * stemp_y[1]) - (a + 3) * stemp_y[1] * stemp_y[1] + 1;
	w_y[2] = (a + 2) * abs(stemp_y[2] * stemp_y[2] * stemp_y[2]) - (a + 3) * stemp_y[2] * stemp_y[2] + 1;
	w_y[3] = a * abs(stemp_y[3] * stemp_y[3] * stemp_y[3]) - 5 * a * stemp_y[3] * stemp_y[3] + 8 * a * abs(stemp_y[3]) - 4 * a;
}
