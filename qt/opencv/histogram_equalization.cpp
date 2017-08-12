

/*================================================================
 *   Copyright (C) 2017年08月12日 肖飞 All rights reserved
 *
 *   文件名称：histogram_equalization.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月12日 星期六 13时28分08秒
 *   修改日期：2017年08月12日 星期六 13时28分20秒
 *   描    述：
 *
 *================================================================*/
//#include "histogram_equalization.h"
/********************************************
功能：灰度图片对直方图均衡化后的效果展示
*************************************/

#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat imageBuf = imread("lena.jpg");

	if (imageBuf.empty()) {
		cout << "image read error!";
	}

	Mat imageGray(imageBuf.rows, imageBuf.cols, CV_8UC1);
	cvtColor(imageBuf, imageGray, CV_BGR2GRAY);//获得灰度图像
	namedWindow("sourceImage", WINDOW_NORMAL);
	imshow("sourceImage", imageGray);
	imwrite("imageGray.jpg", imageGray);

	float p[256];//灰度级
	float n = imageGray.rows * imageGray.cols; //像素总个数

	for (int i = 0; i < 256; i++) {
		p[i] = 0.0;
	}

	for (int i = 0; i < imageGray.rows; i++) {
		for (int j = 0; j < imageGray.cols; j++) {
			p[imageGray.at<uchar>(i, j)] = p[imageGray.at<uchar>(i, j)] + 1;//统计每个灰度级包含多少个像素
		}
	}

	for (int i = 0; i < 256; i++) {
		p[i] = p[i] / n;//每种灰度级的概率
	}

	for (int i = 0; i < imageGray.rows; i++) {
		for (int j = 0; j < imageGray.cols; j++) {
			int gray = imageGray.at<uchar>(i, j);
			float gray_new = 0;

			for (int k = 0; k < gray; k++) {
				gray_new += p[k];
			}

			gray_new = gray_new * 256 - 1;

			if ((gray_new - int(gray_new)) > 0.5) {
				gray_new = int(gray_new);
			} else {
				gray_new = int(gray_new) + 1;
			}

			imageGray.at<uchar>(i, j) = uchar(gray_new);
		}
	}

	namedWindow("resultImage", WINDOW_NORMAL);
	imshow("resultImage", imageGray);
	imwrite("grayResult.jpg", imageGray);
	waitKey(0);
	return 0;
}
