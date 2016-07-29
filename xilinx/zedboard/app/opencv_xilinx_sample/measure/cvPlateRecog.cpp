#include "cv.h"
#include "highgui.h"
#include "cvPlateRecog.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;

#define DEBUG 0

int getPlateSize(IplImage* src, CvSize* size)
{
	IplImage* src_image = src;
	IplImage* yPlate_image = cvCreateImage(cvGetSize(src_image), src_image->depth, src_image->nChannels);
	cvCopy(src_image, yPlate_image);
	IplImage* disRect_image = cvCreateImage(cvGetSize(src_image), src_image->depth, src_image->nChannels);
	cvCopy(src_image, disRect_image);
	IplImage* hsv_image = cvCreateImage(cvGetSize(src_image), src_image->depth, src_image->nChannels);
	IplImage* contour_image = cvCreateImage(cvGetSize(src_image), src_image->depth, 1);
	IplImage* h_image = cvCreateImage(cvGetSize(src_image), src_image->depth, 1);
	IplImage* s_image = cvCreateImage(cvGetSize(src_image), src_image->depth, 1);
	IplImage* v_image = cvCreateImage(cvGetSize(src_image), src_image->depth, 1);

	int flag = PLATE_NONE;

	cvCvtColor(src_image, hsv_image, CV_BGR2HSV);
	cvSplit(hsv_image, h_image, s_image, v_image ,0);
	
	for(int y = 0; y < h_image->height; y++){
		uchar* srcptr = (uchar*)(src_image->imageData + y * src_image->widthStep);
		uchar* yptr = (uchar*)(yPlate_image->imageData + y * src_image->widthStep);
		uchar* hptr = (uchar*)(h_image->imageData + y * h_image->widthStep);
		uchar* sptr = (uchar*)(s_image->imageData + y * s_image->widthStep);
		uchar* vptr = (uchar*)(v_image->imageData + y * v_image->widthStep);

		for(int x = 0; x < h_image->width; x++){
//			printf("h: %d, s: %d, v: %d\n", hptr[x], sptr[x], vptr[x]);
			if(hptr[x] >= 100 && hptr[x] <= 124 && sptr[x] >= 43 && vptr[x] >= 46)	
				;
			else
			{
				srcptr[3 * x + 0] = 0;
				srcptr[3 * x + 1] = 0;
				srcptr[3 * x + 2] = 0;
			}

			if(hptr[x] >= 15 && hptr[x] <= 34 && vptr[x] >= 46)
				;
			else
			{
				yptr[3 * x + 0] = 0;
				yptr[3 * x + 1] = 0;
				yptr[3 * x + 2] = 0;
			}
		}
	
	}

	plateMorphProc(src_image, contour_image);

	cvNamedWindow("dis");
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq *seq = NULL;
	double rectArea = 0;
	double contArea = 0;
	double long2Short = 0;
	double rectDegree = 0;
	CvContourScanner scanner = cvStartFindContours(contour_image, storage);
	while((seq = cvFindNextContour(scanner)) != NULL){
		printf("contour total: %d\n", seq->total);
		CvRect rect = cvBoundingRect(seq);
		contArea = cvContourArea(seq);
		rectArea = rect.width * rect.height;
		long2Short = 1.0 * rect.width / rect.height;
		rectDegree = contArea / rectArea;

		//printf("correct width: %d, height: %d\n, contArea: %f, rectArea: %f, long2short: %f, rectDegree: %f\n", rect.width, rect.height, contArea, rectArea, long2Short, rectDegree);
		if(long2Short < 2.2 || long2Short > 3.8 || rectDegree < 0.63 || rectArea < 800 || rectArea > 50000)
			continue;
		cvRectangle(disRect_image, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(0, 255, 0), 8, 8);
		printf("correct width: %d, height: %d\n", rect.width, rect.height);
		flag = PLATE_BLUE;
		size->width = rect.width;
		size->height = rect.height;
		cvShowImage("dis", disRect_image);
	//	cvWaitKey();
		break;
	}

	if(flag == PLATE_NONE){
		plateMorphProc(yPlate_image, contour_image);
		scanner = cvStartFindContours(contour_image, storage);
		while((seq = cvFindNextContour(scanner)) != NULL){
			printf("contour total: %d\n", seq->total);
			CvRect rect = cvBoundingRect(seq);
			contArea = cvContourArea(seq);
			rectArea = rect.width * rect.height;
			long2Short = 1.0 * rect.width / rect.height;
			rectDegree = contArea / rectArea;

			//printf("correct width: %d, height: %d\n, contArea: %f, rectArea: %f, long2short: %f, rectDegree: %f\n", rect.width, rect.height, contArea, rectArea, long2Short, rectDegree);
			if(long2Short < 1.2 || long2Short > 3.0 || rectDegree < 0.63 || rectArea < 800 || rectArea > 50000)
				continue;
			cvRectangle(disRect_image, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(0, 255, 0), 8 , 8);
			printf("correct width: %d, height: %d\n", rect.width, rect.height);
			flag = PLATE_YELLOW;
			size->width = rect.width;
			size->height = rect.height;
			cvShowImage("dis", disRect_image);
		//	cvWaitKey();
			break;
		}
	}
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&contour_image);
	cvReleaseImage(&yPlate_image);

	//cvNamedWindow("hsv");
	//cvShowImage("hsv", hsv_image);
	//cvNamedWindow("h");
	//cvShowImage("h", h_image);
	//cvNamedWindow("s");
	//cvShowImage("s", s_image);
	//cvNamedWindow("v");
	//cvShowImage("v", v_image);
	//cvNamedWindow("src");
	//cvShowImage("src", src_image);
	cvShowImage("dis", disRect_image);

	cvWaitKey(500);

	//cvReleaseImage(&hsv_image);
	//cvDestroyWindow("hsv");
	//cvReleaseImage(&h_image);
	//cvDestroyWindow("h");
	//cvReleaseImage(&s_image);
	//cvDestroyWindow("s");
	//cvReleaseImage(&v_image);
	//cvDestroyWindow("v");
	//cvReleaseImage(&src_image);
	//cvDestroyWindow("src");
	//cvReleaseImage(&disRect_image);
	//cvDestroyWindow("dis");

	return flag;
}

void plateMorphProc(IplImage* src, IplImage* dis)
{

	if(src == NULL || dis ==NULL || src->nChannels != 3 || dis->nChannels != 1)
	{
		printf("%s: ERROR\n", __FUNCTION__);
		return;	
	}
	IplImage* gray_image = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage* temp_image = cvCreateImage(cvGetSize(src), src->depth, 1);

	cvCvtColor(src, temp_image, CV_BGR2GRAY);
	int thre2 = otsu2(temp_image);
	cvThreshold(temp_image, gray_image, thre2, 255, CV_THRESH_BINARY);

	IplImage* mor_temp = cvCreateImage(cvGetSize(gray_image), gray_image->depth, 1);

	IplConvKernel* ckernel = cvCreateStructuringElementEx(5, 3, 2, 1, CV_SHAPE_RECT);
	IplConvKernel* okernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);

	cvZero(mor_temp);
	cvMorphologyEx(gray_image, gray_image, mor_temp, ckernel, CV_MOP_CLOSE);
	cvZero(mor_temp);
	cvMorphologyEx(gray_image, gray_image, mor_temp, ckernel, CV_MOP_CLOSE);
	cvZero(mor_temp);
	cvMorphologyEx(gray_image, gray_image, mor_temp, ckernel, CV_MOP_CLOSE);
	cvZero(mor_temp);
	cvMorphologyEx(gray_image, gray_image, mor_temp, okernel, CV_MOP_OPEN);

	cvSmooth(gray_image, dis, CV_MEDIAN, 3, 3);

	//cvNamedWindow("gray");
	//cvShowImage("gray", gray_image);
//	cvWaitKey();
	cvReleaseImage(&gray_image);
	cvDestroyWindow("gray");

	cvReleaseImage(&mor_temp);
	cvReleaseImage(&temp_image);
	cvReleaseStructuringElement(&ckernel);
	cvReleaseStructuringElement(&okernel);

}

int otsu2 (IplImage *image)
{
	int w = image->width;
	int h = image->height;

	unsigned char*np; // 图像指针
	unsigned char pixel;
	int thresholdValue=1; // 阈值
	int ihist[256]; // 图像直方图，256个点

	int i, j, k; // various counters
	int n, n1, n2, gmin, gmax;
	double m1, m2, sum, csum, fmax, sb;

	// 对直方图置零...
	memset(ihist, 0, sizeof(ihist));

	gmin=255; gmax=0;
	// 生成直方图
	for (i =0; i < h; i++) 
	{
		np = (unsigned char*)(image->imageData + image->widthStep*i);
		for (j =0; j < w; j++) 
		{
			pixel = np[j];
			ihist[ pixel]++;
			if(pixel > gmax) gmax= pixel;
			if(pixel < gmin) gmin= pixel;
		}
	}

	// set up everything
	sum = csum =0.0;
	n =0;

	for (k =0; k <=255; k++) 
	{
		sum += k * ihist[k]; /* x*f(x) 质量矩*/
		n += ihist[k]; /* f(x) 质量 */
	}

	if (!n) 
	{
		// if n has no value, there is problems...
		//fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");
		thresholdValue =160;
		goto L;
	}

	// do the otsu global thresholding method
	fmax =-1.0;
	n1 =0;
	for (k =0; k <255; k++) 
	{
		n1 += ihist[k];
		if (!n1) { continue; }
		n2 = n - n1;
		if (n2 ==0) { break; }
		csum += k *ihist[k];
		m1 = csum / n1;
		m2 = (sum - csum) / n2;
		sb = n1 * n2 *(m1 - m2) * (m1 - m2);
		/* bbg: note: can be optimized. */
		if (sb > fmax)
		{
			fmax = sb;
			thresholdValue = k;
		}
	}

L:
	for (i =0; i < h; i++) 
	{
		np = (unsigned char*)(image->imageData + image->widthStep*i);
		for (j =0; j < w; j++) 
		{
			if(np[j] >= thresholdValue)
				np[j] =255;
			else np[j] =0;
		}
	}

	cout<<"The Threshold of this Image in Otsu is:"<<thresholdValue<<endl;
	return(thresholdValue);
}

#if(DEBUG == 1)

int main(int argc, char** argv){
	IplImage* src = cvLoadImage(argv[1]);
	CvSize size;
	
	if(0 != getPlateSize(src, &size))
	{
		printf("find the plate, size, width: %d, height: %d\n", size.width, size.height);	
	}

}
#endif
