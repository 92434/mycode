#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int main(int argc, char** argv){
	//cvNamedWindow("picture");
	cvNamedWindow("Raw");

	IplImage* image = NULL;
	CvCapture* capture = cvCreateCameraCapture(0);

	if(!capture){
		printf("open camera error\n");	
		return -1;
	}

	while(true){
		image = cvQueryFrame(capture);	
		cvShowImage("Raw", image);

		char c = 0;
		c = cvWaitKey(90);

		//if('p' == c){

		//	printf("grap picture\n");
		//	cvShowImage("picture", image);
		//	cvSaveImage("distance.jpg", image);
		//}

		if(27 == c){
			printf("return\n");
			break;	
		}

	}

	//cvDestroyWindow("picture");
	cvDestroyWindow("Raw");
	return 0;
}
