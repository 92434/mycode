#include "cv.h"
#include "highgui.h"
int main(int argc,char**argv) 
{ 
  IplImage* src, *dst, src_f;  
 
  float low[9] ={
	  	1.0/16, 2.0/16, 1.0/16,
  		2.0/16, 4.0/16, 2.0/16,
		1.0/16, 2.0/16, 1.0/16,
		};//低通滤波核
  float high[9]={
	  	-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
		};//高通滤波核
  CvMat km_low = cvMat( 3, 3, CV_32FC1, low);  //构造单通道浮点矩阵，将图像IplImage结构转换为图像数组 
  CvMat km_high = cvMat( 3, 3, CV_32FC1, high);  //构造单通道浮点矩阵，将图像IplImage结构转换为图像数组 
  src = cvLoadImage( "001.jpg" ); 
  dst = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 3 );
  cvNamedWindow( "src", 0 );
  cvNamedWindow( "filtering_low", 0 );
  cvNamedWindow( "filtering_high", 0 );
  cvShowImage( "src", src );  
  cvFilter2D( src, dst, &km_low, cvPoint( -1, -1 ) );  //设参考点为核的中心
  cvShowImage( "filtering_low", dst ); 
  cvFilter2D( src, dst, &km_high, cvPoint( -1, -1 ) );  //设参考点为核的中心
  cvShowImage( "filtering_high", dst ); 
  cvWaitKey(0); 
  cvReleaseImage( &src ); 
  cvReleaseImage( &dst ); 
  return 0; 
}
