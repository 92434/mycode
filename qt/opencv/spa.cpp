

/*================================================================
 *   Copyright (C) 2017年08月12日 肖飞 All rights reserved
 *
 *   文件名称：spa.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月12日 星期六 13时29分39秒
 *   修改日期：2017年08月12日 星期六 15时11分48秒
 *   描    述：
 *
 *================================================================*/
//#include "spa.h"
#include <opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
using namespace std;
using namespace cv;

Mat MeanFiltering(Mat image);
int get_middle(double *pArray, int start, int end);
Mat medianFiltering(Mat image);
int main()
{
	Mat image = imread("F:/Qt/Smooth/2.jpg");
	cvtColor(image, image, CV_RGB2GRAY);

	Mat meanFilterImage = MeanFiltering(image);

	Mat medianFilterImage = medianFiltering(image);
	namedWindow("Source", WINDOW_NORMAL);
	imshow("Source", image);
	namedWindow("MeanFilter", WINDOW_NORMAL);
	imshow("MeanFilter", meanFilterImage);
	namedWindow("medianFilter", WINDOW_NORMAL);
	imshow("medianFilter", medianFilterImage);

	waitKey(0);

	return 0;
}

//3*3均值滤波
Mat MeanFiltering(Mat image)
{
	Mat imageBuf(image.rows, image.cols, CV_8UC1);

	for(int i = 0; i < image.rows; i++) {
		for(int j = 0; j < image.cols; j++) {

			if(i > 0 && i < (image.rows - 1) && j > 0 && j < (image.cols - 1)) {
				double GRAY;
				GRAY = (double)image.at<uchar>(i - 1, j - 1) + (double)image.at<uchar>(i - 1, j) + (double)image.at<uchar>(i - 1, j + 1) + (double)image.at<uchar>(i, j - 1) + (double)image.at<uchar>(i, j) +
					   (double)image.at<uchar>(i, j + 1) + (double)image.at<uchar>(i + 1, j - 1) + (double)image.at<uchar>(i + 1, j) + (double)image.at<uchar>(i + 1, j + 1);
				GRAY = GRAY / 9.0;


				imageBuf.at<uchar>(i, j) = (uchar)GRAY;
			} else {

				imageBuf.at<uchar>(i, j) = image.at<uchar>(i, j);
			}
		}
	}

	return imageBuf;
}

//中值滤波
Mat medianFiltering(Mat image)
{
	Mat imageBuf(image.rows, image.cols, CV_8UC1);

	for(int i = 0; i < image.rows; i++) {
		for(int j = 0; j < image.cols; j++) {

			if(i > 0 && i < (image.rows - 1) && j > 0 && j < (image.cols - 1)) {
				double pixelSet[9];
				double RGB;
				pixelSet[0] = (double)image.at<uchar>(i - 1, j - 1);
				pixelSet[1] = (double)image.at<uchar>(i - 1, j);
				pixelSet[2] = (double)image.at<uchar>(i - 1, j + 1);
				pixelSet[3] = (double)image.at<uchar>(i, j - 1);
				pixelSet[4] = (double)image.at<uchar>(i, j);
				pixelSet[5] = (double)image.at<uchar>(i, j + 1);
				pixelSet[6] = (double)image.at<uchar>(i + 1, j - 1);
				pixelSet[7] = (double)image.at<uchar>(i + 1, j);
				pixelSet[8] = (double)image.at<uchar>(i + 1, j + 1);

				RGB = pixelSet[get_middle(pixelSet, 0, 8)];
				imageBuf.at<uchar>(i, j) = (uchar)RGB;
			} else {

				imageBuf.at<uchar>(i, j) = image.at<uchar>(i, j);
			}
		}
	}

	return imageBuf;
}

//获得中位数
int get_middle(double *pArray, int start, int end)
{
	if(start >= end) {
		return start;
	}

	double midvalue = pArray[start];
	int save_ptr = start;
	int front_ptr = start;

	while(front_ptr <= end) {
		if(pArray[front_ptr] < midvalue) {
			pArray[save_ptr] = pArray[front_ptr];
			pArray[front_ptr] = pArray[save_ptr + 1];
			save_ptr++;
		}

		front_ptr++;
	}

	pArray[save_ptr] = midvalue;
	return save_ptr;
}
