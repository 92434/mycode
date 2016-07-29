
#include "cv.h"
#include "measure.hpp"
#include <stdio.h>

#define DEBUG 0

static int object_width = 0;
static int object_height = 0;
static double widthPerPix = 0;
static double heightPerPix = 0;
static double F = 0;

void getObjectRealSizeAndF(void){
	CvFileStorage* fs = cvOpenFileStorage("object_size.xml", 0, CV_STORAGE_READ);	

	F = cvReadRealByName(fs, 0, "focus", 4.8);

	CvSeq* seq = cvGetFileNodeByName(fs, 0, "object_size")->data.seq;
	object_width = cvReadInt((CvFileNode*)cvGetSeqElem(seq, 0));
	object_height = cvReadInt((CvFileNode*)cvGetSeqElem(seq, 1));

	printf("object_width: %d, object_height: %d\n", object_width, object_height);
	printf("focus: %f\n", F);
	cvReleaseFileStorage( &fs );
}

void getInstrinsicsParam(void){
	CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	double fx = cvGetReal2D(intrinsic, 0, 0);
	double fy = cvGetReal2D(intrinsic, 1, 1);

	widthPerPix = F/fx;
	heightPerPix = F/fy; 
	printf("fx: %f, fy: %f\n", fx, fy);
	printf("widthPerPix: %f, heightPerPix: %f\n", widthPerPix, heightPerPix);
}

double measureDistance(const int widthPix, const int heightPix){
	double Dx = F * object_width / (widthPix * widthPerPix);	
	double Dy = F * object_height / (heightPix * heightPerPix);	

	double avgD = (Dx + Dy) / 2;

	printf("Dx: %f, Dy: %f, avgD: %f\n", Dx, Dy, avgD);

	return avgD;
}

#if(DEBUG == 1)

int main(int argc, char** argv){

	if(argc != 3){
		printf("wrong params , params must be 3\n");	
		return -1;
	}
	getObjectRealSizeAndF();
	getInstrinsicsParam();	

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);

	double distance = measureDistance(width, height);

	printf("width: %d, height: %d, distance: %f\n", width, height, distance);

	return 0;
}
#endif
