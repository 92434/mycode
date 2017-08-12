

/*================================================================
 *   Copyright (C) 2017年08月12日 肖飞 All rights reserved
 *
 *   文件名称：biline_interpolation.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月12日 星期六 13时24分21秒
 *   修改日期：2017年08月12日 星期六 13时24分35秒
 *   描    述：
 *
 *================================================================*/
//#include "biline_interpolation.h"
/*****************************10—6*****************************
*功能：用双线性插值法对源图像进行缩放
*数学原理：
假设原图像A的大小为m*n,新图像B的大小为M*N
如果我们要求B（X,Y）处的像素值：
我们首先可以得到B（X,Y）在图像A中对应的位置（x,y）=(X*(m/M),Y*(N/n))
这个时候求得的x,y是小数值，我们可以通过这个小数值坐标找到距离最近的四个像素点，
假设x,y取下限整数后为_x,_y,小数部分为xp,yp,即x=_x+xp,y=_y+yp;
那么最近的四个点为：（_x,_y）,(_x+1,_y),(_x,_y+1),(_x+1,_y+1)
四个系数分别为a = (1-xp)*(1-yp)，b = (1-xp)*yp，c =xp*(1-yp)，d =  xp*yp
最终，pixelB（X,Y）=a*pixelA（_x,_y）+b*pixelA(_x+1,_y)+c*(_x,_y+1)+d*(_x+1,_y+1)
************************************************************/
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("lena.jpg");//源图像

	/*缩小操作*/
	int Col_s = 2 * image.cols / 3; //新的图像长宽缩小至2/3
	int Row_s = 2 * image.rows / 3;

	Mat smallerImage(Row_s, Col_s, CV_8UC3);


	for (int i = 0; i < Row_s; i++) {
		for (int j = 0; j < Col_s; j++) {
			int x, y;
			float xp, yp;
			float a, b, c, d;
			x = i * 3 / 2;//对应坐标整数部分
			y = j * 3 / 2;
			xp = i * 3 / 2.0 - x;//对应坐标小数部分
			yp = j * 3 / 2.0 - y;
			a = (1 - xp) * (1 - yp);
			b = (1 - xp) * yp;
			c = xp * (1 - yp);
			d = xp * yp;

			smallerImage.at<Vec3b>(i, j) = a * (image.at<Vec3b>(x, y)) + b * (image.at<Vec3b>(x, y + 1))
										   + c * (image.at<Vec3b>(x + 1, y)) + d * (image.at<Vec3b>(x + 1, y + 1));
		}
	}


	/*放大操作*/
	int Col_b = 2 * image.cols ;//新的图像长宽放大2倍
	int Row_b = 2 * image.rows ;

	Mat biggerImage(Row_b, Col_b, CV_8UC3);
	int x = 0;
	int y = 0;
	float xp = 0.0;
	float yp = 0.0;
	float a = 0.0;
	float b = 0.0;
	float c = 0.0;
	float d = 0.0;

	for (int i = 0; i < Row_b; i++) {
		for (int j = 0; j < Col_b; j++) {
			x = i / 2;//对应坐标整数部分
			y = j / 2;
			xp = i / 2.0 - x;//对应坐标小数部分
			yp = j / 2.0 - y;
			a = (1 - xp) * (1 - yp);
			b = (1 - xp) * yp;
			c = xp * (1 - yp);
			d = xp * yp;

			x = x > (image.rows - 2) ? (image.rows - 2) : x;//边缘处理
			x = x < 0 ? 0 : x;
			y = y > (image.cols - 2) ? (image.cols - 2) : y;
			y = y < 0 ? 0 : y;

			biggerImage.at<Vec3b>(i, j) = a * (image.at<Vec3b>(x, y)) + b * (image.at<Vec3b>(x, y + 1))
										  + c * (image.at<Vec3b>(x + 1, y)) + d * (image.at<Vec3b>(x + 1, y + 1));
		}
	}

	imshow("iamge", image);
	imshow("smallerImage", smallerImage);
	imshow("biggerImage", biggerImage);
	waitKey(0);

	return 0;

}
