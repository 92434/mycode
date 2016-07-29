#include "cv.h"
#include "highgui.h"
#include "measure.hpp"
#include "cvPlateRecog.hpp"
#include <stdio.h>

int main(int argc, char** argv){

	if(argc != 2){
		printf("wrong params , params must be 2\n");	
		return -1;
	}

	CvSize size;
	int width = 0;
	int height = 0;
	double distance = 0;

	getObjectRealSizeAndF();
	getInstrinsicsParam();	

	int i = 0;
	while(i++ < 100) {
		IplImage* src = cvLoadImage(argv[1]);
		if(0 != getPlateSize(src, &size))
		{
			width = size.width;
			height = size.height;
			distance = measureDistance(width, height);
			printf("the plate pixel size, width: %d, height: %d, real distance: %f\n", size.width, size.height, distance);	

			return 0;
		}

		printf("Do not recognise the plate\n");
	}

	return 0;
}
