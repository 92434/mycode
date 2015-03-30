#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

int main()
{
	// Load source image as you wish
	IplImage *imgSrc = cvLoadImage("001.jpg");
	int w_src = imgSrc->width;
	int h_src = imgSrc->height;
	//cvNamedWindow ("src", 1);
	cvNamedWindow( "dst_big", 1 );

	int degree = 30; // rotate 30 degree
	cvCreateTrackbar( "degree", "dst_big", &degree, 359, 0);

	while(true) {
		double angle = degree  * CV_PI / 180.; // angle in radian
		double a = sin(angle), b = cos(angle); // sine and cosine of angle
		//cvShowImage ("src", imgSrc);
		// Make w_dst and h_dst to fit the output image
		int w_dst = int(h_src * fabs(a) + w_src * fabs(b));
		int h_dst = int(w_src * fabs(a) + h_src * fabs(b));

		// map matrix for WarpAffine, stored in statck array
		double map[6];
		CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);

		// Rotation center needed for cv2DRotationMatrix
		CvPoint2D32f pt = cvPoint2D32f(w_src / 2, h_src / 2);
		cv2DRotationMatrix(pt, degree, 1.0, &map_matrix);

		// Adjust rotation center to dst's center,
		// otherwise you will get only part of the result
		map[2] += (w_dst - w_src) / 2;
		map[5] += (h_dst - h_src) / 2;


		// We need a destination image

		IplImage *imgDst = cvCreateImage(cvSize(w_dst, h_dst), 8, 3);
		cvWarpAffine(
				imgSrc, 
				imgDst,
				&map_matrix,
				CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,
				cvScalarAll(0)
			    );

		// Don't forget to release imgSrc and imgDst if you no longer need them
		cvShowImage( "dst_big", imgDst);
		cvReleaseImage(&imgDst);
		if((char)cvWaitKey(50) == 27) break;
	}
	cvReleaseImage(&imgSrc);
	return 0;
}
