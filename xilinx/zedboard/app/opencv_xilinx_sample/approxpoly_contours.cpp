#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

int main()
{
	IplImage* src = NULL;
	IplImage* img = NULL;
	IplImage* dst = NULL;

	CvMemStorage* storage = cvCreateMemStorage (0);
	CvMemStorage* storage1 = cvCreateMemStorage (0);
	CvSeq* contour = 0;
	CvSeq* cont;
	CvSeq* mcont;

	src = cvLoadImage ("002.jpg", 1);
	img = cvCreateImage (cvGetSize(src), IPL_DEPTH_8U, 1);
	dst = cvCreateImage (cvGetSize(src), src->depth, src->nChannels);

	cvCvtColor (src, img, CV_BGR2GRAY);
	cvThreshold (img, img, 100, 200, CV_THRESH_BINARY);

	cvFindContours (img, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	if (contour)
	{
		CvTreeNodeIterator iterator;
		cvInitTreeNodeIterator (&iterator, contour,  1);
		while (0 != (cont = (CvSeq*)cvNextTreeNode (&iterator)))
		{
			mcont = cvApproxPoly (cont, sizeof(CvContour), storage1, CV_POLY_APPROX_DP, cvContourPerimeter(cont)*0.02,0);
			cvDrawContours (dst, mcont, CV_RGB(255,0,0),CV_RGB(0,0,100),1,2,8,cvPoint(0,0));
		}
	}

	cvNamedWindow ("Contour", 1);
	cvShowImage ("Contour", dst);

	cvWaitKey (0);

	cvReleaseMemStorage (&storage);
	cvReleaseImage (&src);
	cvReleaseImage (&img);
	cvReleaseImage (&dst);

	return 0;
}
