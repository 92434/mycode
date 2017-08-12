

/*================================================================
 *   Copyright (C) 2017年08月12日 肖飞 All rights reserved
 *   
 *   文件名称：nearest_neighbor_interpolation.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月12日 星期六 13时25分54秒
 *   修改日期：2017年08月12日 星期六 13时26分44秒
 *   描    述：
 *
 *================================================================*/
//#include "nearest_neighbor_interpolation.h"
/************************2016-10-5*****************************************
*功能：最近邻插值缩放图像
*数学原理；
 假设原图像大小为m*n,新图像的大小为M*N
 那么新图像的(X,Y)点的像素pixel(X,Y)=原图像pixel(X*(m/M),Y*(N/n))
 由于图像坐标不能为小数，对计算出的坐标通过四舍五入或取整处理
**********************************************************************/


#include <opencv2\opencv.hpp>
#include <iostream>


using namespace std;
using namespace cv;


int main(){
    Mat image = imread("lena.jpg");

    int Col_s = image.cols / 2;//新的图像长宽缩小至一半
    int Row_s = image.rows / 2;

    Mat smallerImage(Row_s, Col_s, CV_8UC3);

    for (int i = 0; i < Row_s; i++){
        for (int j = 0; j < Col_s; j++){
            int X = i*2;
            int Y = j*2;        
            smallerImage.at<Vec3b>(i, j) = image.at<Vec3b>(X, Y);
        }
    }


    int Col_b = image.cols * 2;//新的图像长宽增加一倍
    int Row_b = image.rows * 2;

    Mat biggerImage(Row_b, Col_b, CV_8UC3);

    for (int i = 0; i < Row_b; i++){
        for (int j = 0; j < Col_b; j++){
            int X = i / 2;
            int Y = j / 2;
            biggerImage.at<Vec3b>(i, j) = image.at<Vec3b>(X, Y);
        }
    }


    imshow("image", image);
    imshow("smallerImage", smallerImage);
    imshow("biggerImage", biggerImage);
    waitKey(0);

    return 0;
}
