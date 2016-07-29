#ifndef __CVPLATERECOG_HPP__
#define __CVPLATERECOG_HPP__

#define PLATE_NONE 0
#define PLATE_BLUE 1
#define PLATE_YELLOW 2

int otsu2 (IplImage *image);
void plateMorphProc(IplImage* src, IplImage* dis);
int getPlateSize(IplImage* src, CvSize* size);

#endif
