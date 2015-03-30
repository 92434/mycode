#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <cvaux.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#define LINE_BUFFER 2048

using namespace cv;
using namespace std;


#define COLOR_SIZE 5
static CvScalar color_tab[COLOR_SIZE] = {
	CV_RGB(255,0,0),
	CV_RGB(0,255,0),
	CV_RGB(100,100,255),
	CV_RGB(255,0,255),
	CV_RGB(255,255,0),
};

//函数：lhStructuringElementCopy
//
//功能：获得形态学结构元素的拷贝
//
//参数：se 输入结构元素
//
//返回：se的拷贝，注意：在不使用时需要用cvReleaseStructuringElement释放
//
//源码：
//
IplConvKernel* lhStructuringElementCopy(IplConvKernel* se) {

	IplConvKernel* copy = cvCreateStructuringElementEx( se->nCols, se->nRows,

			se->anchorX, se->anchorY, 0, NULL );

	copy->nShiftR = se->nShiftR;

	memcpy( copy->values, se->values, sizeof(int) * se->nRows * se->nCols );

	return copy;

}

//函数：lhStructuringElementNot
//
//功能：获得与当前形态学结构元素相反的结构元素
//
//参数：se 输入结构元素
//
//返回：se的取反，注意：在不使用时需要用cvReleaseStructuringElement释放
//
//源码：
//
IplConvKernel* lhStructuringElementNot(IplConvKernel* se) {

	IplConvKernel* temp = cvCreateStructuringElementEx( se->nCols, se->nRows,

			se->anchorX, se->anchorY, 0, NULL );

	temp->nShiftR = se->nShiftR;

	memcpy( temp->values, se->values, sizeof(int) * se->nRows * se->nCols );

	for(int i=0; i<temp->nRows * temp->nCols ; i++)

		temp->values[i] = !temp->values[i] ;

	return temp;

}



//函数：lhStructuringElementMap
//
//功能：获得当前形态学结构元素的映射（反射）结构元素
//
//参数：se 输入结构元素
//
//返回：se的映射（反射），注意：在不使用时需要用cvReleaseStructuringElement释放
//
//源码：
//
IplConvKernel* lhStructuringElementMap(IplConvKernel* se) {

	CvMat *mat = cvCreateMat( se->nRows,  se->nCols, CV_32SC1);

	memcpy(mat->data.i, se->values, sizeof(int) * se->nRows * se->nCols );

	cvFlip(mat, NULL, -1);

	IplConvKernel* semap = cvCreateStructuringElementEx( se->nCols, se->nRows,

			se->nCols-se->anchorX-1, se->nRows-se->anchorY-1, 0, NULL );

	semap->nShiftR = se->nShiftR;

	memcpy( semap->values, mat->data.i, sizeof(int) * se->nRows * se->nCols );

	cvReleaseMat(&mat);

	return semap;

}





//函数：lhMorpStructuringElementCard
//
//功能：获得当前形态学结构元素的基数
//
//参数：se 输入结构元素
//
//返回：结构元素的基数
//
int lhMorpStructuringElementCard(IplConvKernel* se) {

	assert(se != NULL);

	int i, cnt = 0;

	for (i=0; i<se->nCols*se->nRows; i++)

	{

		cnt += se->values[i];

	}

	return cnt;

}



//函数：lhStructuringElementLine
//
//功能：形态学线性结构元素的创建，常用于形态学方向分析
//
//参数：angle len 分别为线性结构元素的角度和长度
//
//返回：线性结构元素的指针
//
IplConvKernel* lhStructuringElementLine(unsigned int angle, unsigned int len) {

	assert(len > 2);

	angle = angle%180;

	CvPoint p1 = {0};

	CvPoint p2 = {0};

	int width = cvRound(len*cos((float)angle*CV_PI/180.0));

	int height = cvRound(len*sin((float)angle*CV_PI/180.0));

	height = height >= 1 ? height : 1;

	if (width < 0)

	{

		width = width <= -1 ? width : -1;

		p1.x = 0;

		p1.y = 0;

		p2.x = -width -1;

		p2.y = height -1;

	}

	else

	{

		width = width >= 1 ? width : 1;

		p1.x = 0;

		p1.y = height -1;

		p2.x = width -1;

		p2.y = 0;

	}

	CvMat *temp = cvCreateMat(height, width, CV_32SC1);

	cvZero(temp);

	cvLine(temp, p1, p2, cvScalar(1,1,1), 1, 4, 0);

	IplConvKernel* se = cvCreateStructuringElementEx( width, height,

			(width-1)/2, (height-1)/2,  CV_SHAPE_CUSTOM, temp->data.i );

	cvReleaseMat(&temp);

	return se;
}

//函数：lhMorpOpen
//
//说明：形态学开运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se 结构元素指针
//
//iterations 开运算中膨胀和腐蚀次数
//
//源码：
//
void lhMorpOpen(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	cvErode( src, dst, se, iterations );

	//注：采用结构元素的映射进行膨胀！！！

	IplConvKernel* semap = lhStructuringElementMap(se);

	cvDilate( dst, dst, semap, iterations );

	cvReleaseStructuringElement(&semap);

}

//函数：lhMorpClose
//
//说明：形态学闭运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se 结构元素指针
//
//iterations 闭运算中膨胀和腐蚀次数
//
//源码：
//
void lhMorpClose(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	cvDilate( src, dst, se, iterations );

	//注:采用结构元素的映射进行腐蚀

	IplConvKernel* semap = lhStructuringElementMap(se);

	cvErode( dst, dst, semap, iterations );

	cvReleaseStructuringElement(&semap);

}

//函数：lhMorpGradient
//
//说明：形态学基本梯度运算，通过结构元素的改变，可以进行厚梯度，方向梯度等形态学计算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpGradient(const IplImage* src, IplImage* dst, IplConvKernel* element=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	IplImage*  temp = cvCloneImage(src);

	cvErode( src, temp, element, iterations );

	cvDilate( src, dst, element, iterations );

	cvSub( dst, temp, dst );

	cvReleaseImage(&temp);

}


//函数：lhMorpGradientIn
//
//说明：形态学内梯度运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpGradientIn(const IplImage* src, IplImage* dst, IplConvKernel* element=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	cvErode( src, dst, element, iterations );

	cvSub( src, dst, dst );

}

//函数：lhMorpGradientOut
//
//说明：形态学外梯度运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpGradientOut(const IplImage* src, IplImage* dst, IplConvKernel* element=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	cvDilate( src, dst, element, iterations );

	cvSub(dst, src, dst );

}

//函数：lhMorpWhiteTopHat
//
//说明：形态学白顶帽运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：

void lhMorpWhiteTopHat(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	lhMorpOpen(src, dst, se, iterations );

	cvSub( src, dst, dst );

}

//函数：lhMorpBlackTopHat
//
//说明：形态学黑顶帽运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpBlackTopHat(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	lhMorpClose(src, dst, se, iterations );

	cvSub(dst, src, dst );

}

//函数：lhMorpQTopHat
//
//说明：形态学自补顶帽运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpQTopHat(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	IplImage*  temp = cvCloneImage(src);

	lhMorpClose( src, temp, se, iterations );

	lhMorpOpen( src, dst, se, iterations );

	cvSub(temp, dst, dst );

	cvReleaseImage(&temp);

}


//函数：lhMorpEnhance
//
//说明：形态学对比度增强运算
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//element结构元素
//
//iterations膨胀和腐蚀次数
//
//源码：
//
void lhMorpEnhance(const IplImage* src, IplImage* dst, IplConvKernel* se=NULL, int iterations=1) {

	assert(src != NULL && dst != NULL && src != dst);

	IplImage*  temp = cvCloneImage(src);

	lhMorpWhiteTopHat( src, temp, se, iterations );

	lhMorpBlackTopHat( src, dst, se, iterations );

	cvAdd(src, temp, temp);

	cvSub(temp, dst, dst );

	cvReleaseImage(&temp);

}


//函数：lhMorpHMTB
//
//说明：形态学二值击中-击不中变换
//
//参数：
//
//src 输入图像，二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//源码：
//
void lhMorpHMTB(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	if (sebg == NULL)

		sebg = lhStructuringElementNot(sefg);

	IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

	cvErode( src, temp1, sefg);

	cvNot(src, temp2);

	cvErode( temp2, temp2, sebg);

	cvAnd(temp1, temp2, dst);

	cvReleaseImage(&temp1);

	cvReleaseImage(&temp2);

	cvReleaseStructuringElement(&sebg);

}

//函数：lhMorpHMTU
//
//说明：形态学非约束击中-击不中变换，当为二值图像时，结果与lhMorpHMTB相同。
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//源码：
//
void lhMorpHMTU(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	if (sebg == NULL)

		sebg = lhStructuringElementNot(sefg);

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  mask = cvCreateImage(cvGetSize(src), 8, 1);

	cvZero(mask);

	cvErode( src, temp, sefg);

	cvDilate(src, dst, sebg);

	cvCmp(temp, dst, mask, CV_CMP_GT);

	cvSub(temp, dst, dst, mask);

	cvNot(mask, mask);

	cvSet(dst, cvScalar(0), mask);

	cvReleaseImage(&mask);

	cvReleaseImage(&temp);

	cvReleaseStructuringElement(&sebg);

}

//函数：lhMorpHMTC
//
//说明：形态学约束击中-击不中变换，当为二值图像时，结果与lhMorpHMTB相同。
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//源码：
//
void lhMorpHMTC(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	if (sebg == NULL)

		sebg = lhStructuringElementNot(sefg);

	IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  temp3 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  temp4 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  mask1 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  mask2 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  mask3 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  mask4 = cvCreateImage(cvGetSize(src), 8, 1);

	cvZero(mask1);

	cvZero(mask2);

	cvZero(mask3);

	cvZero(mask4);

	cvZero(dst);

	cvErode( src, temp1, sebg);

	cvDilate( src, temp2, sebg);

	cvErode( src, temp3, sefg);

	cvDilate( src, temp4, sefg);

	cvCmp(src, temp3, mask1, CV_CMP_EQ);

	cvCmp(temp2, src,  mask2, CV_CMP_LT);

	cvAnd(mask1, mask2, mask2);

	cvCmp(src, temp4, mask3 , CV_CMP_EQ);

	cvCmp(temp1, src, mask4 , CV_CMP_GT);

	cvAnd(mask3, mask4, mask4);

	cvSub(src, temp2, dst, mask2);

	cvSub(temp1, src, dst, mask4);

	cvReleaseImage(&mask1);

	cvReleaseImage(&mask2);

	cvReleaseImage(&mask3);

	cvReleaseImage(&mask4);

	cvReleaseImage(&temp1);

	cvReleaseImage(&temp2);

	cvReleaseImage(&temp3);

	cvReleaseImage(&temp4);

	cvReleaseStructuringElement(&sebg);

}


//函数：lhMorpHMT
//
//说明：形态学击中-击不中变换
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type 击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：

#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpHMT(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY) {

	switch(type)

	{

		case LH_MORP_TYPE_BINARY:

			{

				lhMorpHMTB(src, dst, sefg, sebg);

				break;

			}

		case LH_MORP_TYPE_UNCONSTRAIN:

			{

				lhMorpHMTU(src, dst, sefg, sebg);

				break;

			}

		case LH_MORP_TYPE_CONSTRAIN:

			{

				lhMorpHMTC(src, dst, sefg, sebg);

				break;

			}



		default:

			break;

	}

}

//函数：lhMorpHMTOpen
//
//说明：形态学击中-击不中开变换
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type 击中-击不中开变换中击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：

#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpHMTOpen(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY)

{

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	IplConvKernel* semap = lhStructuringElementMap(sefg);

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	lhMorpHMT(src, temp, sefg, sebg, type);

	cvDilate(temp, dst, semap);

	cvReleaseImage(&temp);

	cvReleaseStructuringElement(&semap);

}


//函数：lhMorpThin
//
//说明：形态学细化变换
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type变换中击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：

#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpThin(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	lhMorpHMT(src, dst, sefg, NULL, type);

	cvSub(src, dst, dst);

}


//函数：lhMorpThinFit
//
//说明：形态学细化匹配变换
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type变换中击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：

#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpThinFit(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY)

{

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	lhMorpHMTOpen(src, dst, sefg, NULL, type);

	cvSub(src, dst, dst);

}

//函数：lhMorpThick
//
//说明：形态学粗化运算
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type变换中击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：
//
#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpThick(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	lhMorpHMT(src, dst, sefg, NULL, type);

	cvAdd(src, dst, dst);

}


//函数：lhMorpThickMiss
//
//说明：形态学粗化不匹配运算
//
//参数：
//
//src 输入图像，灰度或二值图像
//
//dst 输出图像
//
//sefg 前景结构元素
//
//sebg 背景结构元素，如为空，则默认为前景结构元素sefg的取反
//
//type变换中击中-击不中变换的类型，二值 0 非约束 1 约束 2
//
//定义如下：
//
#define LH_MORP_TYPE_BINARY                   0

#define LH_MORP_TYPE_UNCONSTRAIN       1

#define LH_MORP_TYPE_CONSTRAIN           2

//源码：

void lhMorpThickMiss(const IplImage* src, IplImage* dst, IplConvKernel* sefg, IplConvKernel* sebg =NULL, int type=LH_MORP_TYPE_BINARY) {

	assert(src != NULL && dst != NULL && src != dst && sefg!= NULL && sefg!=sebg);

	lhMorpHMTOpen(src, dst, sefg, NULL, type);

	cvAdd(src, dst, dst);

}

//函数：lhMorpRDilate
//
//说明：形态学测地膨胀和膨胀重建运算
//
//参数：
//
//src 输入图像
//
//msk 掩模图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations测地膨胀的次数，当默认为-1时，为重建运算
//
//源码：
//
void lhMorpRDilate(const IplImage* src, const IplImage* msk, IplImage* dst, IplConvKernel* se = NULL, int iterations=-1) {

	assert(src != NULL && msk != NULL && dst != NULL && src != dst );

	if(iterations < 0)

	{

		//膨胀重建

		cvMin(src, msk, dst);

		cvDilate(dst, dst, se);

		cvMin(dst, msk, dst);

		IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

		IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

		do

		{

			cvCopy(dst, temp1);

			cvDilate(dst, dst, se);

			cvMin(dst, msk, dst);

			cvCmp(temp1, dst, temp2, CV_CMP_NE );

		}

		while(cvSum(temp2).val[0] != 0);

		cvReleaseImage(&temp1);

		cvReleaseImage(&temp2);

		return;    

	}

	else if (iterations == 0)

	{

		cvCopy(src, dst);

	}

	else

	{

		//普通测地膨胀 p136(6.1)

		cvMin(src, msk, dst);

		cvDilate(dst, dst, se);

		cvMin(dst, msk, dst);

		for(int i=1; i<iterations; i++)

		{

			cvDilate(dst, dst, se);

			cvMin(dst, msk, dst);

		}

	}

}

//函数：lhMorpRErode
//
//说明：形态学测地腐蚀和腐蚀重建运算
//
//参数：
//
//src 输入图像
//
//msk 掩模图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations测地腐蚀的次数，当默认为-1时，为腐蚀重建运算
//
//源码：
//
void lhMorpRErode(const IplImage* src,  const IplImage* msk, IplImage* dst, IplConvKernel* se = NULL, int iterations=-1) {

	assert(src != NULL  && msk != NULL && dst != NULL && src != dst );

	if(iterations < 0)

	{

		//腐蚀重建

		cvMax(src, msk, dst);

		cvErode(dst, dst, se);

		cvMax(dst, msk, dst);

		IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

		IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

		do

		{

			cvCopy(dst, temp1);

			cvErode(dst, dst, se);

			cvMax(dst, msk, dst);

			cvCmp(temp1, dst, temp2, CV_CMP_NE);

		}

		while(cvSum(temp2).val[0] != 0);

		cvReleaseImage(&temp1);

		cvReleaseImage(&temp2);

		return;    

	}

	else if (iterations == 0)

	{

		cvCopy(src, dst);

	}

	else

	{

		//普通测地腐蚀 p137(6.2)

		cvMax(src, msk, dst);

		cvErode(dst, dst, se);

		cvMax(dst, msk, dst);

		for(int i=1; i<iterations; i++)

		{

			cvErode(dst, dst, se);

			cvMax(dst, msk, dst);

		}

	}

}

//函数：lhMorpRSelfDual
//
//说明：形态学测地自对偶和自对偶重建运算
//
//参数：
//
//src 输入图像
//
//msk 掩模图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations测地自对偶运算的次数，当默认为-1时，为自对偶重建运算
//
//源码：

void lhMorpRSelfDual(const IplImage* src, const IplImage* msk, IplImage* dst, IplConvKernel* se = NULL, int iterations=-1) {

	assert(src != NULL  && msk != NULL && dst != NULL && src != dst );

	IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

	cvZero(temp2);

	lhMorpRDilate(src, msk, temp1, se, iterations);

	lhMorpRErode(src, msk, dst, se, iterations);

	cvCmp(src, msk, temp2, CV_CMP_LE);

	cvCopy(temp1, dst, temp2);

	cvReleaseImage(&temp1);

	cvReleaseImage(&temp2);

}

//函数：lhMorpRMin
//
//说明：形态学区域极小值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//源码：
//
void lhMorpRMin(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvAddS(src, cvScalar(1), temp);

	lhMorpRErode(temp, src, dst, se);

	cvSub(dst, src, dst);

	cvReleaseImage(&temp);

}

//函数：lhMorpRMax
//
//说明：形态学区域极大值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//源码：
//
void lhMorpRMax(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvSubS(src, cvScalar(1), temp);

	lhMorpRDilate(temp, src, dst, se);

	cvSub(src, dst, dst);

	cvReleaseImage(&temp);

}

//函数：lhMorpHMax
//
//说明：形态学H极大值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpHMax(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvSubS(src, cvScalar(h), temp);

	lhMorpRDilate(temp, src, dst, se);

	cvReleaseImage(&temp);

}

//函数：lhMorpHMin
//
//说明：形态学H极小值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpHMin(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvAddS(src, cvScalar(h), temp);

	lhMorpRErode(temp, src, dst, se);

	cvReleaseImage(&temp);

}

//函数：lhMorpHConcave
//
//说明：形态学H凹变换
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpHConcave(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	lhMorpHMin(src, dst, h, se);

	cvSub(dst, src, dst);

}

//函数：lhMorpHConvex
//
//说明：形态学H凸变换
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpHConvex(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	lhMorpHMax(src, dst, h, se);

	cvSub(src, dst, dst);

}

//函数：lhMorpEMin
//
//说明：形态学扩展极小值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpEMin(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	lhMorpHMin(src, temp, h, se);

	lhMorpRMin(temp, dst, se);

	cvReleaseImage(&temp);

}

//函数：lhMorpEMax
//
//说明：形态学扩展极大值
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//h 深度或动态范围阈值
//
//se  结构元素
//
//源码：
//
void lhMorpEMax(const IplImage* src, IplImage* dst, unsigned char h, IplConvKernel* se = NULL) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	lhMorpHMax(src, temp, h, se);

	lhMorpRMax(temp, dst, se);

	cvReleaseImage(&temp);

}

//函数：lhMorpRankFilterB
//
//说明：二值图像的形态学等级滤波，值得注意的是：腐蚀、膨胀、中值滤波是特殊的等级滤波
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素（默认为3*3矩形）
//
//rank 等级（默认为0时，rank取SE基数的中值，此时相当于中值滤波）
//
void lhMorpRankFilterB(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, unsigned int rank = 0) {

	assert(src != NULL  &&  dst != NULL && src != dst );

	bool defaultse = false;

	int card;

	if (se == NULL)

	{

		card = 3*3;

		assert(rank >= 0 && rank <= card);

		se = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);

		defaultse = true;

	}

	else

	{

		card = lhMorpStructuringElementCard(se);

		assert(rank >= 0 && rank <= card);

	}

	//default rank is median

	if (rank == 0)

		rank = card/2+1;

	IplConvKernel* semap =  lhStructuringElementMap(se);

	CvMat *semat = cvCreateMat(semap->nRows, semap->nCols, CV_32FC1);

	int i;

	for (i=0; i<semap->nRows*semap->nCols; i++)

	{

		semat->data.fl[i] = semap->values[i];

	}

	cvThreshold(src, dst, 0, 1, CV_THRESH_BINARY);

	IplImage *temp = cvCreateImage(cvGetSize(dst), 8, 1);

	cvFilter2D(dst, temp, semat, cvPoint(semap->anchorX, semap->anchorY));

	cvThreshold(temp, dst, card-rank, 255, CV_THRESH_BINARY);

	cvReleaseMat(&semat);

	cvReleaseStructuringElement(&semap);

	if (defaultse)

		cvReleaseStructuringElement(&se);     

	cvReleaseImage(&temp);

}

//函数：lhMorpROpen
//
//说明：形态学开重建
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations产生标记图像所用的腐蚀的次数
//
//源码：
//
void lhMorpROpen(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations=1) {

	assert(src != NULL  && dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvErode(src, temp, se, iterations);

	lhMorpRDilate(temp, src, dst, se, -1);

	cvReleaseImage(&temp);

}



//函数：lhMorpRWTH
//
//说明：形态学白帽重建
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations产生标记图像所用的腐蚀的次数
//
//源码：

void lhMorpRWTH(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations=1) {

	assert(src != NULL  && dst != NULL && src != dst );

	lhMorpROpen(src, dst, se, iterations);

	cvSub(src, dst, dst);

}

//函数：lhMorpRClose
//
//说明：形态学闭重建
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations产生标记图像所用的膨胀的次数
//
//源码：

void lhMorpRClose(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations=1) {

	assert(src != NULL  && dst != NULL && src != dst );

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvDilate(src, temp, se, iterations);

	lhMorpRErode(temp, src, dst, se, -1);

	cvReleaseImage(&temp);

}





//函数：lhMorpRBTH
//
//说明：形态学黑帽重建
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//se  结构元素
//
//iterations产生标记图像所用的膨胀的次数
//
//源码：
//
void lhMorpRBTH(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations=1) {

	assert(src != NULL  && dst != NULL && src != dst );

	lhMorpRClose(src, dst, se, iterations);

	cvSub(dst, src, dst);

}

//函数：lhMorpGradientDir
//说明：形态学方向梯度
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//angle线性结构元素的角度
//
//len线性结构元素的长度
//
//源码：
//
void lhMorpGradientDir(const IplImage* src, IplImage* dst, unsigned int angle, unsigned int len ) {

	assert(src != NULL && dst != NULL && src != dst);

	IplConvKernel* se = lhStructuringElementLine(angle, len);

	lhMorpGradient(src, dst, se);

	cvReleaseStructuringElement(&se);

}

//函数：lhMorpRemoveBoderObj
//
//说明：去除边界的连通区域
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//源码：

void lhMorpRemoveBoderObj(const IplImage* src, IplImage* dst) {

	IplImage *temp = cvCloneImage(src);

	cvRectangle(temp, cvPoint(3,3), cvPoint(temp->width-7, temp->height-7), CV_RGB(0,0,0), -1);

	lhMorpRDilate(temp, src, dst);

	cvReleaseImage(&temp);

	cvSub(src, dst, dst);

}

//函数：lhMorpFillHole
//
//说明：空洞的填充
//
//参数：
//
//src 输入图像
//
//dst 输出图像
//
//源码：
//
void lhMorpFillHole(const IplImage* src, IplImage* dst) {

	IplImage *temp = cvCloneImage(src);

	double min, max;

	cvMinMaxLoc(src, &min, &max);

	cvRectangle(temp, cvPoint(3,3), cvPoint(temp->width-7, temp->height-7), CV_RGB(max,max,max), -1);

	lhMorpRErode(temp, src, dst);

	cvReleaseImage(&temp);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//public class Smoother {
//	//平滑
//	public static void smooth(String filename) { 
//		IplImage image = cvLoadImage(filename);
//		if (image != null) {
//			cvSmooth(image, image, CV_GAUSSIAN, 3);
//			cvSaveImage(filename, image);
//			cvReleaseImage(image);
//		}
//	}
//	//增加外框
//	public static void padding(String filename)
//	{
//		CvMat src,dst;
//		int top=10,  left=10;
//		int borderType=IPL_BORDER_CONSTANT;
//		CvScalar value;
//		/// Load an image
//		src = cvLoadImageM(filename);
//		//dst =cvCreateImage( cvSize( src.cvSize().width()+left, src.cvSize().height()+top ), IPL_DEPTH_8U, 3 ).asCvMat();
//		dst =  cvCreateMat(src.rows()+left,src.cols()+top,CV_8UC3);
//		value = new CvScalar( 0,0,0,0);
//		//point为src 在dst图像上的左上角坐标
//		CvPoint point=cvPoint(left/2,top/2);
//		cvCopyMakeBorder( src, dst, point, borderType, value );
//		cvSaveImage("D:\\IBM\\pad_new.JPG", dst);
//	}
//	//金字塔放大
//	public static void pyramid_up(String filename)
//	{
//		CvMat src, dst, tmp;
//		/// Load an image
//		src = cvLoadImageM(filename);
//		//tmp = src;
//		dst = cvCreateMat(src.rows()*2,src.cols()*2,src.type());
//		cvPyrUp( src, dst, CV_GAUSSIAN_5x5);
//		//cvPyrDown( tmp, dst,2);
//		//tmp = dst;
//		cvSaveImage("D:\\IBM\\pyramid_up.JPG", dst);
//	}
//	//金字塔缩小
//	public static void pyramid_down(String filename)
//	{
//		CvMat src, dst, tmp;
//		/// Load an image
//		src = cvLoadImageM(filename);
//		//tmp = src;
//		dst = cvCreateMat(src.rows()/2,src.cols()/2,src.type());
//		cvPyrDown( src, dst, CV_GAUSSIAN_5x5);
//		//cvPyrDown( tmp, dst,2);
//		// tmp = dst;
//		cvSaveImage("D:\\IBM\\pyramid_down.JPG", dst);
//	}
//
//	//扩张，将目标的边缘的“毛刺”踢除掉
//	public static void morphology_Dilation(String filename,int dilation_elem)
//	{
//		CvMat src, dilation_dst;
//		src = cvLoadImageM(filename);
//		dilation_dst=src;
//		int dilation_type=CV_SHAPE_RECT;
//		if( dilation_elem == 0 ){ dilation_type = CV_SHAPE_RECT; }
//		else if( dilation_elem == 1 ){ dilation_type = CV_SHAPE_CROSS; }
//		else if( dilation_elem == 2) { dilation_type = CV_SHAPE_ELLIPSE; }
//		//    CvMat element = cvGetStructuringElement( dilation_type,
//		//    cvSize( 2*1 + 1, 2*1+1 ),
//		//    cvPoint( 2, 2 ) );
//		/// Apply the dilation operation
//		IplConvKernel kernel=cvCreateStructuringElementEx(3,3,1,1,dilation_type,null);
//		cvDilate( src, dilation_dst, kernel,1);
//		cvReleaseStructuringElement( kernel );
//		cvSaveImage("D:\\IBM\\morphology_Dilation_"+dilation_type+".JPG", dilation_dst);
//	}
//
//	//侵蚀，将目标的边缘或者是内部的坑填掉
//	public static void morphology_Erosion(String filename,int dilation_elem)
//	{
//		CvMat src, erosion_dst;
//		src = cvLoadImageM(filename);
//		erosion_dst=src;
//		int dilation_type=CV_SHAPE_RECT;
//		if( dilation_elem == 0 ){ dilation_type = CV_SHAPE_RECT; }
//		else if( dilation_elem == 1 ){ dilation_type = CV_SHAPE_CROSS; }
//		else if( dilation_elem == 2) { dilation_type = CV_SHAPE_ELLIPSE; }
//		//    CvMat element = cvGetStructuringElement( dilation_type,
//		//    cvSize( 2*1 + 1, 2*1+1 ),
//		//    cvPoint( 2, 2 ) );
//		/// Apply the dilation operation
//		IplConvKernel kernel=cvCreateStructuringElementEx(3,3,1,1,dilation_type,null);
//		cvErode( src, erosion_dst, kernel,1);
//		cvReleaseStructuringElement( kernel );
//		cvSaveImage("D:\\IBM\\morphology_Erosion_"+dilation_type+".JPG", erosion_dst);
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:51:47 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 最简单图像分割方法
//	 * @param filename
//	 */
//	//The simplest segmentation method
//	public static void Thresholding(String filename,int type)
//	{
//		IplImage src, pGrayImg,dst;
//		int threshold_value = 0;
//		/* 0: Binary
//1: Binary Inverted
//2: Threshold Truncated
//3: Threshold to Zero
//4: Threshold to Zero Inverted
//*/
//		int threshold_type = type;
//		int  max_BINARY_value = 255;
//		src=cvLoadImage(filename);
//		pGrayImg=gray(src);
//		dst=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
//		/**
//		 * – src_gray: Our input image
//		 – dst: Destination (output) image
//		 – threshold_value: The thresh value with respect to which the thresholding operation is made
//		 – max_BINARY_value: The value used with the Binary thresholding operations (to set the chosen pixels)
//		 – threshold_type: One of the 5 thresholding operations.
//		 */
//		cvThreshold( pGrayImg, dst, threshold_value, max_BINARY_value,threshold_type );
//		cvSaveImage("D:\\IBM\\morphology_Thresholding_"+type+".JPG",dst);
//		cvReleaseImage(src);
//		cvReleaseImage(dst);
//		cvReleaseImage(pGrayImg);
//	}
//
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:52:51 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 计算灰度图
//	 * @param src
//	 * @return
//	 */
//	public static IplImage gray(IplImage src)
//	{
//
//		//将RGB色彩空间转换成BGR色彩空间 8位 3通道
//		IplImage  pImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
//		/*src是源图像；
//		  dst是转换后的图像；
//		  flags是转换的模式，可以取0：没有变化；1：垂直翻转，即沿x轴翻转；2：交换红蓝信道；
//		  */
//		cvConvertImage(src, pImg, 2);
//
//
//		//将RGB转换成Gray度图
//		IplImage  pGrayImg = cvCreateImage(
//				cvGetSize(pImg),
//				IPL_DEPTH_8U,
//				1);
//		cvCvtColor(pImg, pGrayImg, CV_RGB2GRAY);
//		cvReleaseImage(pImg);
//		return pGrayImg;
//		//cvSaveImage("D:\\IBM\\gray.jpg",pGrayImg);
//	}
//	//sobel边缘检测
//	public static void sobel(String filename)
//	{
//		CvMat src, src_gray;
//		CvMat grad=null;
//		int scale = 1;
//		int delta = 0;
//		int ddepth = CV_16S;
//		int c;
//		src=cvLoadImageM(filename);
//		//GaussianBlur( src, src, cvSize(3,3), 0, 0, BORDER_DEFAULT );
//		cvSmooth(src, src, CV_GAUSSIAN, 3);
//		src_gray=gray(src.asIplImage()).asCvMat();
//		CvMat grad_x=null, grad_y=null;
//		CvMat abs_grad_x=null, abs_grad_y=null;
//		/// Gradient X
//		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
//		grad_x=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		cvSobel( src_gray, grad_x, 1, 0, 3);
//		abs_grad_x=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		/**
//		 * src ： 原数组
//		 　　dst ：输出数组 (深度为 8u).
//		 　　scale ：比例因子.
//		 　　shift ：原数组元素按比例缩放后添加的值
//		 */
//		cvConvertScaleAbs( grad_x, abs_grad_x,1,0);
//		/// Gradient Y
//		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
//		grad_y=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		abs_grad_y=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		cvSobel( src_gray, grad_y, 0, 1, 3);
//		cvConvertScaleAbs( grad_y, abs_grad_y,1,0 );
//		/// Total Gradient (approximate)
//		grad=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		cvAddWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0,grad );
//		cvSaveImage("D:\\IBM\\sobel.jpg",grad);
//
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:51:47 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 拉普拉斯边缘检测
//	 * @param filename
//	 */
//	public static void laplacian(String filename)
//	{
//		CvMat src, src_gray,dst,abs_dst;
//		src=cvLoadImageM(filename);
//		cvSmooth(src, src, CV_GAUSSIAN, 3);
//		src_gray=gray(src.asIplImage()).asCvMat();
//		dst=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		abs_dst=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		/**
//		 * src_gray  输入图像
//		 * dst 输出图像
//		 * 3 核大小
//		 */
//		cvLaplace( src_gray, dst,3);
//		cvConvertScaleAbs( dst, abs_dst,1,0);
//		cvWaitKey(0);  
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:51:47 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc canny边缘检测
//	 * @param filename
//	 */
//	public static void canny(String filename)
//	{
//
//		CvMat src,src_gray, detected_edges,dst;
//		src=cvLoadImageM(filename);
//		src_gray=gray(src.asIplImage()).asCvMat();
//		detected_edges=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();
//		cvSmooth(src_gray, detected_edges, CV_GAUSSIAN, 3); 
//		/**
//		 * image 输入图像，这个必须是单通道的，即灰度图
//		 　　edges 输出的边缘图像 ，也是单通道的，但是是黑白的
//		 　　threshold1 第一个阈值
//		 　　threshold2 第二个阈值
//		 　　aperture_size Sobel 算子内核大小
//		 * canny算子得实质：如果一个像素的梯度大与上限值，
//		 * 则被认为是边缘像素，如果小于下限阈值，则被抛弃，
//		 * 那么如果该店的梯度位于两者之间呢？则当其与高于上限值的像素点连接时我们才保留，否则删除。
//		 */
//		cvCanny(detected_edges, detected_edges,90,90*3,3);
//		//dst = Scalar::all(0);
//		dst=cvCreateMat(src.rows(),src.cols(), src.type());
//		//cvSetIdentity(dst,cvRealScalar(0));
//		cvSet(dst, cvScalar(0,0,0,0),null);
//		cvCopy( src,dst,detected_edges);
//		cvSaveImage("D:\\IBM\\canny.jpg",dst);
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:51:15 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 标准哈夫变换直线
//	 * @param filename
//	 */
//	public static void standardHoughLine(String filename)
//	{
//		CvMat src, detected_edges,color_dst;
//		src=cvLoadImageM(filename,0);//加载灰度图
//		detected_edges=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();//创建一通道图像
//		color_dst = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 3).asCvMat();  //创建三通道图像
//		//cvSmooth(src, detected_edges, CV_GAUSSIAN, 3);
//		//边缘检测
//		cvCanny(src, detected_edges,50,200,3);
//		//src_gray=gray(detected_edges.asIplImage()).asCvMat();
//		cvCvtColor( detected_edges, color_dst, CV_GRAY2BGR ); //色彩空间转换，将dst转换到另外一个色彩空间即3通道图像
//
//		CvMemStorage storage=cvCreateMemStorage(0);
//		/*
//		 * detected_edges：8-bit, single-channel binary source image
//		 * storage：存储检测到得线段
//		 * method：CV_HOUGH_STANDARD(标准哈夫变换) ，CV_HOUGH_PROBABILISTIC(矩阵必须是CV_32SC4类型)，CV_HOUGH_MULTI_SCALE 
//		 * rho  与像素相关单位的距离精度
//		 * theta 弧度测量的角度精度
//		 * threshold 累计阀值
//		 * param1：当method=CV_HOUGH_STANDARD 设置为0表示不需要，当method=CV_HOUGH_PROBABILISTIC 表示最小线长度，当method=CV_HOUGH_MULTI_SCALE 意义还不清楚
//		 * param2：当method=CV_HOUGH_STANDARD 设置为0表示不需要，当method=CV_HOUGH_PROBABILISTIC 表示线的最大空隙；大于此空隙算作两个线，当method=CV_HOUGH_MULTI_SCALE 意义还不清楚
//		 */
//		CvSeq lines  = cvHoughLines2( detected_edges, storage, CV_HOUGH_STANDARD, 1, Math.PI/180, 150, 0, 0); 
//		//循环直线序列 
//		for( int i = 0; i < lines.total(); i++ )
//		{
//			FloatPointer line=new FloatPointer(cvGetSeqElem(lines,i));//用GetSeqElem获得直线
//			//    CvPoint2D32f point = new CvPoint2D32f(cvGetSeqElem(lines, i));
//			//    
//			//                   float rho=point.x();
//			//                float theta=point.y();
//			//对于SHT和MSHT（标准变换）这里line[0]，line[1]是rho（与像素相干单位的距离精度）和theta（弧度测量的角度精度）
//			float rho = line.get(0);
//			float theta =line.get(1);
//			System.out.println(rho+"::"+theta);
//			CvPoint pt1, pt2;
//			float a = (float)Math.cos(theta), b = (float)Math.sin(theta);
//			float x0 = a*rho, y0 = b*rho;
//			//pt1.position(0).x(Math.round(x0 + 1000*(-b))) ;
//			//pt1.position(0).y(Math.round(y0 + 1000*(a)));
//			pt1=new CvPoint(Math.round(x0 + 1000*(-b)),Math.round(y0 + 1000*(a)));
//			//pt2.position(0).x(Math.round(x0 - 1000*(-b)));
//			//pt2.position(0).y(Math.round(y0 - 1000*(a)));
//			pt2=new CvPoint(Math.round(x0 - 1000*(-b)),Math.round(y0 - 1000*(a)));
//			cvLine( color_dst, pt1, pt2, CV_RGB(0,0,255), 1, CV_AA,0);
//		}
//		cvNamedWindow("Hough");
//		cvShowImage( "Hough", color_dst ); 
//		cvWaitKey();
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:50:37 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 哈夫变换直线
//	 * @param filename
//	 */
//	//The Probabilistic Hough Line Transform
//	public static void houghLine(String filename)
//	{
//		CvMat src, detected_edges,color_dst;
//		src=cvLoadImageM(filename,0);//加载灰度图
//		detected_edges=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1).asCvMat();//创建一通道图像
//		color_dst = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 3 ).asCvMat();  //创建三通道图像
//		//cvSmooth(src, detected_edges, CV_GAUSSIAN, 3);
//		//边缘检测
//		cvCanny(src, detected_edges,50,200,3);
//		//src_gray=gray(detected_edges.asIplImage()).asCvMat();
//		cvCvtColor( detected_edges, color_dst, CV_GRAY2BGR ); //色彩空间转换，将dst转换到另外一个色彩空间即3通道图像
//
//		CvMemStorage storage=cvCreateMemStorage(0);
//		/*
//		 * detected_edges：8-bit, single-channel binary source image
//		 * storage：存储检测到得线段
//		 * method：CV_HOUGH_STANDARD(标准哈夫变换) ，CV_HOUGH_PROBABILISTIC(矩阵必须是CV_32SC4类型)，CV_HOUGH_MULTI_SCALE 
//		 * rho  与像素相关单位的距离精度
//		 * theta 弧度测量的角度精度
//		 * threshold 累计阀值
//		 * param1：当method=CV_HOUGH_STANDARD 设置为0表示不需要，当method=CV_HOUGH_PROBABILISTIC 表示最小线长度，当method=CV_HOUGH_MULTI_SCALE 意义还不清楚
//		 * param2：当method=CV_HOUGH_STANDARD 设置为0表示不需要，当method=CV_HOUGH_PROBABILISTIC 表示线的最大空隙；大于此空隙算作两个线，当method=CV_HOUGH_MULTI_SCALE 意义还不清楚
//		 */
//		CvSeq lines  = cvHoughLines2( detected_edges, storage, CV_HOUGH_PROBABILISTIC, 1, Math.PI/180, 150, 50, 10); 
//		//循环直线序列  
//		for( int i = 0; i < lines.total(); i++ )  //lines存储的是直线  
//		{       
//			CvPoint line = new CvPoint(cvGetSeqElem(lines,i));
//			/**
//			 *  img – Image.
//			 pt1 – First point of the line segment.
//			 pt2 – Second point of the line segment.
//			 color – Line color.
//			 thickness – Line thickness.
//			 lineType –
//			 Type of the line:
//			 8 (or omitted) - 8-connected line.
//			 4 - 4-connected line.
//			 CV_AA - antialiased line.
//			 shift – Number of fractional bits in the point coordinates.
//			 */
//			cvLine( color_dst,new CvPoint(line.position(0)),new CvPoint(line.position(1)), CV_RGB( 0, 255, 0 ),1,CV_AA,0 );  //将找到的直线标记为红色  
//			//color_dst是三通道图像用来存直线图像  
//		} 
//		cvNamedWindow("Hough");
//		cvShowImage( "Hough", color_dst ); 
//		cvWaitKey();
//		//Canvas.showImage(color_dst);
//		// cvSaveImage("D:\\IBM\\houghLineCV_AA.jpg",color_dst);
//	}
//
//	/**
//	 * 
//	 * @date Jul 2, 2012 3:58:29 PM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 哈夫变换，检测圆
//	 * @param filename
//	 */
//	public static void houghCircle(String filename){
//		CvMat src, src_gray,color_dst;
//		src=cvLoadImageM(filename);//加载灰度图
//		src_gray=gray(src.asIplImage()).asCvMat();
//		cvSmooth(src_gray, src_gray, CV_GAUSSIAN, 3);
//		CvMemStorage storage=cvCreateMemStorage(0);
//		/// Apply the Hough Transform to find the circles
//		/**
//		  image
//		  输入 8-比特、单通道灰度图像.
//		  circle_storage
//		  检测到的圆存储仓. 可以是内存存储仓 (此种情况下，一个线段序列在存储仓中被创建，并且由函数返回）或者是包含圆参数的特殊类型的具有单行/单列的CV_32FC3型矩阵(CvMat*). 矩阵头为函数所修改，使得它的 cols/rows 将包含一组检测到的圆。如果 circle_storage 是矩阵，而实际圆的数目超过矩阵尺寸，那么最大可能数目的圆被返回
//		  . 每个圆由三个浮点数表示：圆心坐标(x,y)和半径.
//
//		  method
//		  Hough 变换方式，目前只支持CV_HOUGH_GRADIENT, which is basically 21HT, described in [Yuen03].
//		  dp
//		  累加器图像的分辨率。这个参数允许创建一个比输入图像分辨率低的累加器。（这样做是因为有理由认为图像中存在的圆会自然降低到与图像宽高相同数量的范畴）。如果dp设置为1，则分辨率是相同的；如果设置为更大的值（比如2），累加器的分辨率受此影响会变小（此情况下为一半）。dp的值不能比1小。
//		  Resolution of the accumulator used to detect centers of the circles. For example, if it is 1, the accumulator will have the same resolution as the input image, if it is 2 - accumulator will have twice smaller width and height, etc.
//
//		  min_dist
//		  该参数是让算法能明显区分的两个不同圆之间的最小距离。
//		  Minimum distance between centers of the detected circles. If the parameter is too small, multiple neighbor circles may be falsely detected in addition to a true one. If it is too large, some circles may be missed.
//
//		  param1
//		  用于Canny的边缘阀值上限，下限被置为上限的一半。
//		  The first method-specific parameter. In case of CV_HOUGH_GRADIENT it is the higher threshold of the two passed to Canny edge detector (the lower one will be twice smaller).
//
//		  param2
//		  累加器的阀值。
//		  The second method-specific parameter. In case of CV_HOUGH_GRADIENT it is accumulator threshold at the center detection stage. The smaller it is, the more false circles may be detected. Circles, corresponding to the larger accumulator values, will be returned first.
//
//		  min_radius
//		  最小圆半径。
//		  Minimal radius of the circles to search for.
//
//		  max_radius
//		  最大圆半径。
//		  */
//		CvSeq circles=cvHoughCircles( src_gray, storage, CV_HOUGH_GRADIENT, 1, src.rows()/8, 200, 100, 0, 0 );
//		/// Draw the circles detected
//		for( int i = 0; i < circles.total(); i++ )
//		{
//			FloatPointer seq=new FloatPointer(cvGetSeqElem(circles,i));
//			System.out.println(seq.get(0)+","+seq.get(1)+","+seq.get(2));
//			CvPoint center=new CvPoint (Math.round(seq.get(0)), Math.round(seq.get(1)));
//			int radius = Math.round(seq.get(2));
//			// circle center
//			cvCircle( src, center, 3, CV_RGB(0,255,0), -1, 8, 0 );
//			// circle outline
//			cvCircle( src, center, radius, CV_RGB(255,0,0), 3, 8, 0 );
//		}
//		/// Show your results
//		cvNamedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
//		cvShowImage( "Hough Circle Transform Demo", src );
//		cvWaitKey(0);
//
//	}
//	/**
//	 * 
//	 * @date Jul 2, 2012 3:57:45 PM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 直方图均衡化，增强图像的对比度
//	 * @param filename
//	 */
//	public static void  histogramEqualization(String filename)
//	{
//		CvMat src,src_gray, detected_edges,dst;
//		src=cvLoadImageM(filename);
//		src_gray=gray(src.asIplImage()).asCvMat();
//		dst =  cvCreateMat(src.rows(),src.cols(),CV_8UC1);
//		/// Apply Histogram Equalization
//		cvEqualizeHist( src_gray, dst );
//		/// Display results
//		cvNamedWindow( "source_window", CV_WINDOW_AUTOSIZE );
//		cvNamedWindow( "equalized_window", CV_WINDOW_AUTOSIZE );
//		cvShowImage( "source_window", src );
//		cvShowImage( "equalized_window", dst );
//		/// Wait until user exits the program
//		cvWaitKey(0);
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:49:50 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 直方图计算,归一化参数为1
//	 */
//	public static void histogramCalculationFor1(String filename)
//	{
//		CvMat src,redImage,greenImage,blueImage;
//		src=cvLoadImageM(filename);
//		redImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//		greenImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//		blueImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//
//		cvSplit(src,blueImage,greenImage,redImage,null);  
//		IplImage  b_planes[] = {blueImage.asIplImage()};
//		IplImage  g_planes[] = {greenImage.asIplImage()};
//		IplImage  r_planes[] = {redImage.asIplImage()};
//		/// Establish the number of bins
//		int histSize = 256;
//		/// Set the ranges ( for B,G,R) )
//		float range[] = { 0, 255} ;
//		float[] histRange[] = { range };
//		int hist_size[] = {histSize};
//		/**
//		 * param1:直方图维数，
//		 * param2:bin的个数，
//		 * param3:直方图表示格式，CV_HIST_ARRAY（多维浓密数组），CV_HIST_SPARSE（多维稀疏数组）
//		 * param4:param5为1时，ranges[i](0<=i<cDims，译者注：cDims为直方图的维数)是包含两个元素的范围数组，包括直方图第i维的上界和下界。
//		 * 在第i维上的整个区域 [lower,upper]被分割成 dims[i]（译者注：dims[i]表示直方图第i维的bin数） 个相等的bin;
//		 * param5为0时,则ranges[i]是包含dims[i]+1个元素的范围数组，
//		 * 包括lower0, upper0, lower1, upper1 == lower2, ..., upper(dims[i]-1),
//		 *  其中lowerj 和upperj分别是直方图第i维上第 j 个bin的上下界（针对输入象素的第 i 个值）
//		 * param5:归一化标识，0或1
//		 * 
//		 */
//		CvHistogram b_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,1); 
//		/// Compute the histograms:
//		/**
//		 * param1:输入图像
//		 * param2:直方图
//		 * param3:累计标识,如果设置，则直方图在开始时不被清零。这个特征保证可以为多个图像计算一个单独的直方图，或者在线更新直方图。
//		 * param4:mask ,操作 mask, 确定输入图像的哪个象素被计数 
//		 */
//		cvCalcHist(b_planes,b_hist,0,null);
//
//		CvHistogram g_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,1); 
//		/// Compute the histograms:
//		cvCalcHist(g_planes,g_hist,0,null);
//		CvHistogram r_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,1); 
//		/// Compute the histograms:
//		cvCalcHist(r_planes,r_hist,0,null);
//
//		// Draw the histograms for B, G and R
//		int hist_w = 512; int hist_h = 400;
//		int bin_w = Math.round(hist_w/histSize );
//		CvMat histImage=cvCreateMat(hist_h, hist_w, CV_8UC3);
//		cvSet(histImage,CV_RGB( 0,0,0),null); 
//		///归一化， Normalize the result to [ 0, histImage.rows ]
//		cvNormalize(b_hist.mat(), b_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		cvNormalize(g_hist.mat(),g_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		cvNormalize(r_hist.mat(),r_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		/// Draw for each channel
//		for( int i = 1; i < histSize; i++ )
//		{
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h -(int)(Math.round( cvGetReal1D( b_hist.bins(), i-1 ))) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( b_hist.mat(),i)) ),
//					CV_RGB( 255, 0, 0), 2, 8, 0 );
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h - (int)Math.round(cvGetReal1D( g_hist.bins(),i-1)) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( g_hist.bins(),i)) ),
//					CV_RGB( 0, 255, 0), 2, 8, 0 );
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h - (int)Math.round(cvGetReal1D( r_hist.bins(),i-1)) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( r_hist.bins(),i)) ),
//					CV_RGB( 0, 0, 255), 2, 8, 0 );
//		}
//		/// Display
//		cvNamedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
//		cvShowImage("calcHist Demo", histImage );
//		cvWaitKey(0);
//	}
//	/**
//	 * 
//	 * @date Jul 3, 2012 10:49:50 AM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 直方图计算，归一化参数为0
//	 */
//	public static void histogramCalculationFor0(String filename)
//	{
//		CvMat src,redImage,greenImage,blueImage;
//		src=cvLoadImageM(filename);
//		redImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//		greenImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//		blueImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();  
//
//		cvSplit(src,blueImage,greenImage,redImage,null);  
//		IplImage  b_planes[] = {blueImage.asIplImage()};
//		IplImage  g_planes[] = {greenImage.asIplImage()};
//		IplImage  r_planes[] = {redImage.asIplImage()};
//		/// Establish the number of bins
//		int histSize = 3;
//		/// Set the ranges ( for B,G,R) )
//		float range[] = { 0, 100,101,200,201,255} ;
//		float[] histRange[] = { range };
//		int hist_size[] = {histSize};
//		/**
//		 * param1:直方图维数，
//		 * param2:bin的个数，
//		 * param3:直方图表示格式，CV_HIST_ARRAY（多维浓密数组），CV_HIST_SPARSE（多维稀疏数组）
//		 * param4:param5为1时，ranges[i](0<=i<cDims，译者注：cDims为直方图的维数)是包含两个元素的范围数组，包括直方图第i维的上界和下界。
//		 * 在第i维上的整个区域 [lower,upper]被分割成 dims[i]（译者注：dims[i]表示直方图第i维的bin数） 个相等的bin;
//		 * param5为0时,则ranges[i]是包含dims[i]+1个元素的范围数组，
//		 * 包括lower0, upper0, lower1, upper1 == lower2, ..., upperdims[i]-1,
//		 *  其中lowerj 和upperj分别是直方图第i维上第 j 个bin的上下界（针对输入象素的第 i 个值）
//		 * param5:归一化标识，0或1
//		 * 
//		 */
//		CvHistogram b_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,0); 
//		/// Compute the histograms:
//		cvCalcHist(b_planes,b_hist,0,null);
//
//		CvHistogram g_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,0); 
//		/// Compute the histograms:
//		cvCalcHist(g_planes,g_hist,0,null);
//		CvHistogram r_hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,histRange,0); 
//		/// Compute the histograms:
//		cvCalcHist(r_planes,r_hist,0,null);
//
//		// Draw the histograms for B, G and R
//		int hist_w = 512; int hist_h = 400;
//		int bin_w = Math.round(hist_w/histSize );
//		//CvMat histImage=new CvMat( hist_h, hist_w, CV_8UC3);
//		//dst = Scalar::all(0);
//		CvMat histImage=cvCreateMat(hist_h, hist_w, CV_8UC3);
//		//cvSetIdentity(dst,cvRealScalar(0));
//		cvSet(histImage,CV_RGB( 0,0,0),null); 
//		/// Normalize the result to [ 0, histImage.rows ]
//		cvNormalize(b_hist.mat(), b_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		cvNormalize(g_hist.mat(),g_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		cvNormalize(r_hist.mat(),r_hist.mat(), 1, histImage.rows(), NORM_MINMAX ,null);
//		/// Draw for each channel
//		for( int i = 1; i < histSize; i++ )
//		{
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h -(int)(Math.round( cvGetReal1D( b_hist.bins(), i-1 ))) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( b_hist.mat(),i)) ),
//					CV_RGB( 255, 0, 0), 2, 8, 0 );
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h - (int)Math.round(cvGetReal1D( g_hist.bins(),i-1)) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( g_hist.bins(),i)) ),
//					CV_RGB( 0, 255, 0), 2, 8, 0 );
//			cvLine( histImage, cvPoint( bin_w*(i-1), hist_h - (int)Math.round(cvGetReal1D( r_hist.bins(),i-1)) ) ,
//					cvPoint( bin_w*(i), hist_h - (int)Math.round(cvGetReal1D( r_hist.bins(),i)) ),
//					CV_RGB( 0, 0, 255), 2, 8, 0 );
//		}
//		/// Display
//		cvNamedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
//		cvShowImage("calcHist Demo", histImage );
//		cvWaitKey(0);
//	}
//
//	public static void histogramComparison(String src,String test1,String test2)
//	{
//		CvMat src_base,src_half,src_test1,src_test2,hsv_base,hsv_test1,hsv_test2,hsv_half_down;
//		src_base=cvLoadImageM(src);
//		src_test1=cvLoadImageM(test1);
//		src_test2=cvLoadImageM(test2);
//		System.out.println("坐标起始点:"+ src_base.asIplImage().origin());
//		src_half=cvCreateMatHeader(src_test2.rows()/2,src_test2.cols(),src_test2.type());
//		CvRect rect=cvRect(0,  src_base.rows()/2, src_base.cols(), src_base.rows()/2);
//		cvGetSubRect(src_base,src_half, rect);
//		hsv_base = cvCreateImage(cvGetSize(src_base),8,3).asCvMat();
//		hsv_test1 = cvCreateImage(cvGetSize(src_test1),8,3).asCvMat();
//		hsv_test2 = cvCreateImage(cvGetSize(src_test2),8,3).asCvMat();
//		hsv_half_down=cvCreateImage(cvGetSize(src_half),8,3).asCvMat();
//		/// Convert to HSV
//		cvCvtColor( src_base, hsv_base, CV_BGR2HSV );
//		cvCvtColor( src_test1, hsv_test1, CV_BGR2HSV );
//		cvCvtColor( src_test2, hsv_test2, CV_BGR2HSV );
//		cvCvtColor( src_half, hsv_half_down, CV_BGR2HSV );
//		//获得h和s通道的值
//		IplImage hsv_base_h_plane = cvCreateImage(cvGetSize(src_base),8,1);  
//		IplImage hsv_base_s_plane = cvCreateImage(cvGetSize(src_base),8,1);  
//		cvSplit(hsv_base,hsv_base_h_plane,hsv_base_s_plane,null,null);  
//		IplImage[] hsv_base_array={hsv_base_h_plane,hsv_base_s_plane};
//		//获得h和s通道的值
//		IplImage hsv_test1_h_plane = cvCreateImage(cvGetSize(src_test1),8,1);  
//		IplImage hsv_test1_s_plane = cvCreateImage(cvGetSize(src_test1),8,1);  
//		cvSplit(hsv_test1,hsv_test1_h_plane,hsv_test1_s_plane,null,null);  
//		IplImage[] hsv_test1_array={hsv_test1_h_plane,hsv_test1_s_plane};
//		//获得h和s通道的值
//		IplImage hsv_test2_h_plane = cvCreateImage(cvGetSize(src_test2),8,1);  
//		IplImage hsv_test2_s_plane = cvCreateImage(cvGetSize(src_test2),8,1);  
//		cvSplit(hsv_test2,hsv_test2_h_plane,hsv_test2_s_plane,null,null); 
//		IplImage[] hsv_test2_array={hsv_test2_h_plane,hsv_test2_s_plane};
//		//获得h和s通道的值
//		IplImage hsv_half_down_h_plane = cvCreateImage(cvGetSize(hsv_half_down),8,1);  
//		IplImage hsv_half_down_s_plane = cvCreateImage(cvGetSize(hsv_half_down),8,1);  
//		cvSplit(hsv_half_down,hsv_half_down_h_plane,hsv_half_down_s_plane,null,null); 
//		IplImage[] hsv_half_down_array={hsv_half_down_h_plane,hsv_half_down_s_plane};
//
//
//		/// Using 30 bins for hue and 32 for saturation
//		int h_bins = 50; int s_bins = 60;
//		int histSize[] = { h_bins, s_bins };
//		// hue varies from 0 to 256, saturation from 0 to 180
//		float h_ranges[] = { 0, 256 };
//		float s_ranges[] = { 0, 180 };
//		float[] ranges[] = { h_ranges, s_ranges };
//		// Use the o-th and 1-st channels
//		//redImage=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1).asCvMat();
//		CvHistogram hist_base = cvCreateHist(2,histSize,CV_HIST_ARRAY,ranges,1); 
//		/// Compute the histograms:
//		cvCalcHist(hsv_base_array,hist_base,0,null);
//		cvNormalize(hist_base.mat(), hist_base.mat(), 0,1, NORM_MINMAX ,null);
//
//		CvHistogram hist_half_down = cvCreateHist(2,histSize,CV_HIST_ARRAY,ranges,1); 
//		/// Compute the histograms:
//		cvCalcHist(hsv_half_down_array,hist_half_down,0,null);
//		cvNormalize(hist_half_down.mat(), hist_half_down.mat(), 0,1, NORM_MINMAX ,null);
//
//		CvHistogram hist_test1 = cvCreateHist(2,histSize,CV_HIST_ARRAY,ranges,1); 
//		/// Compute the histograms:
//		cvCalcHist(hsv_test1_array,hist_test1,0,null);
//		cvNormalize(hist_test1.mat(), hist_test1.mat(), 0,1, NORM_MINMAX ,null);
//
//		CvHistogram hist_test2 = cvCreateHist(2,histSize,CV_HIST_ARRAY,ranges,1); 
//		/// Compute the histograms:
//		cvCalcHist(hsv_test2_array,hist_test2,0,null);
//		cvNormalize(hist_test2.mat(), hist_test2.mat(), 0,1, NORM_MINMAX ,null);
//
//		for( int i = 0; i < 4; i++ )
//		{ 
//			int compare_method = i;
//			double base_base = cvCompareHist( hist_base, hist_base, compare_method );
//			double base_half = cvCompareHist( hist_base, hist_half_down, compare_method );
//			double base_test1 = cvCompareHist( hist_base, hist_test1, compare_method );
//			double base_test2 = cvCompareHist( hist_base, hist_test2, compare_method );
//			System.out.println( " Method [%d] Perfect, Base-Half, Base-Test(1)," +
//					" Base-Test(2) : %f, %f, %f, %f \n"+i+","+ base_base+","+ base_half+","+base_test1+","+base_test2);
//		}
//	}
//	/**
//	 * 
//	 * @date Jul 6, 2012 4:39:19 PM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 反向投影
//	 * @param filename
//	 * @param bins
//	 */
//	public static void backProjection(String filename,int bins){
//		/**
//		 *  filename – Name of file to be loaded.
//		 flags –
//		 Flags specifying the color type of a loaded image:
//		 >0 Return a 3-channel color image
//		 =0 Return a grayscale image
//		 <0 Return the loaded image as is
//		 */
//
//		IplImage target=cvLoadImage(filename,1);  //装载图片
//		IplImage target_hsv=cvCreateImage( cvGetSize(target), IPL_DEPTH_8U, 3 );
//		IplImage target_hue=cvCreateImage( cvGetSize(target), IPL_DEPTH_8U, 1);
//		cvCvtColor(target,target_hsv,CV_BGR2HSV);       //转化到HSV空间
//		cvSplit( target_hsv, target_hue, null, null, null );    //获得H分量
//		//    int ch[] = { 0, 0 };
//		//    IplImage[] target_hsv_s={target_hsv};
//		//    IplImage[] target_hue_s={target_hue};
//		//    cvMixChannels(target_hsv_s, 1, target_hue_s, 1, ch, 1 );
//		if(bins<2) bins=2;
//		int hist_size[]={bins};          //将H分量的值量化到[0,255]
//		float[] ranges[]={ {0,180} };    //H分量的取值范围是[0,360)
//		CvHistogram hist=cvCreateHist(1, hist_size, CV_HIST_ARRAY,ranges, 1);
//		IplImage[] target_hues={target_hue};
//		cvCalcHist(target_hues,hist, 0, null);
//		cvNormalize(hist.mat(), hist.mat(), 0,255, NORM_MINMAX ,null);
//		IplImage result=cvCreateImage(cvGetSize(target),IPL_DEPTH_8U,1);
//
//		cvCalcBackProject(target_hues,result,hist);
//		cvShowImage( "BackProj", result );
//		//cvShowImage( "src", target );
//		cvWaitKey(0);
//	}
//
//	/**
//	 * 
//	 * @date Jul 6, 2012 5:33:11 PM
//	 * @author suntengjiao@ceopen.cn
//	 * @desc 模版匹配
//	 * @param src
//	 * @param template
//	 * @param match_method
//	 */
//	public static void templateMatching(String src,String template,int match_method)
//	{
//		CvMat img=cvLoadImageM(src,1);  //装载图片
//		CvMat templ=cvLoadImageM(template,1);  //装载图片
//		/// Create the result matrix
//		int result_cols = img.cols() - templ.cols() + 1;
//		int result_rows = img.rows() - templ.rows() + 1;
//		CvMat result=cvCreateMat( result_cols, result_rows, CV_32FC1 );
//		cvMatchTemplate( img, templ, result, 0 );
//		cvNormalize(result, result, 0,1, NORM_MINMAX ,null);
//		/// Localizing the best match with minMaxLoc
//		double minVal[]=new double[5]; double maxVal[]=new double[5]; CvPoint minLoc=new CvPoint();
//		CvPoint maxLoc=new CvPoint();
//		CvPoint matchLoc=new CvPoint();
//		cvMinMaxLoc( result, minVal, maxVal, minLoc, maxLoc,null);
//		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
//		if( match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
//		{ matchLoc = minLoc; }
//		else
//		{ matchLoc = maxLoc; }
//		/// Source image to display
//		CvMat img_display=cvCreateMat( img.cols(),img.rows(), img.type() );;
//		cvCopy(img,img_display,null);
//		/// Show me what you got
//		cvRectangle( img_display, matchLoc,new CvPoint( matchLoc.x() + templ.cols() , matchLoc.y() + templ.rows() ), new CvScalar(0,0,0,0), 2, 8, 0);
//		cvRectangle( result, matchLoc, new CvPoint( matchLoc.x() + templ.cols() , matchLoc.y() + templ.rows() ), new CvScalar(0,0,0,0), 2, 8, 0 );  
//		cvShowImage( "image_window", img_display );
//		cvShowImage( "result_window", result );
//	}
//	public static void main(String...strings){
//		//Smoother.smooth("D:\\IBM\\test.JPG");
//		//Smoother.padding("D:\\IBM\\pad.JPG");
//		// Smoother.pyramid_up("D:\\IBM\\pad.JPG");
//		//Smoother.pyramid_down("D:\\IBM\\pad.JPG");
//		//    morphology_Dilation("D:\\IBM\\morp.JPG",0);
//		//    morphology_Dilation("D:\\IBM\\morp.JPG",1);
//		//    morphology_Dilation("D:\\IBM\\morp.JPG",2);
//
//		//    morphology_Erosion("D:\\IBM\\morp.JPG",0);
//		//    morphology_Erosion("D:\\IBM\\morp.JPG",1);
//		//    morphology_Erosion("D:\\IBM\\morp.JPG",2);
//		//    Thresholding("D:\\IBM\\pad32.jpg",0);
//		//    Thresholding("D:\\IBM\\morp.jpg",1);
//		//    Thresholding("D:\\IBM\\morp.jpg",2);
//		//    Thresholding("D:\\IBM\\morp.jpg",3);
//		//    Thresholding("D:\\IBM\\morp.jpg",4);
//		//gray("D:\\IBM\\pad32.jpg");
//		//sobel("D:\\IBM\\pad32.jpg");
//		//laplacian("D:\\IBM\\pad32.jpg");
//		//canny("D:\\IBM\\pad32.jpg");
//		// houghLine("D:\\IBM\\luoma.jpg");
//		//standardHoughLine("D:\\IBM\\luoma.jpg");
//		//houghCircle("D:\\IBM\\177.jpg");
//		//histogramEqualization("D:\\IBM\\040.jpg");
//		// histogramCalculationFor1("D:\\IBM\\pad32.jpg");
//		//histogramCalculationFor0("D:\\IBM\\pad32.jpg");
//		//histogramComparison("D:\\IBM\\morp.JPG","D:\\IBM\\177.jpg","D:\\IBM\\pad32.jpg");
//		backProjection("D:\\IBM\\hand2.jpg",2);
//		backProjection("D:\\IBM\\hand2.jpg",5);
//		//    backProjection("D:\\IBM\\hand2.jpg",10);
//		//    backProjection("D:\\IBM\\hand2.jpg",12);
//		//    backProjection("D:\\IBM\\hand2.jpg",20);
//		//    backProjection("D:\\IBM\\hand2.jpg",40);
//		//    backProjection("D:\\IBM\\hand2.jpg",80);
//		//    backProjection("D:\\IBM\\hand2.jpg",120);
//		//    backProjection("D:\\IBM\\hand2.jpg",200);
//		//    backProjection("D:\\IBM\\hand2.jpg",255);
//	}
//}////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void test_color_table() {
#define IMG_W 500
#define IMG_H 500

	IplImage* img = cvCreateImage(cvSize(IMG_W, IMG_H), 8, 3);
	CvPoint2D32f pt =  cvPoint2D32f(IMG_W/2.0, IMG_H/2.0);
	int radius = MIN(IMG_W/2.0, IMG_H/2.0);
	int step = radius / COLOR_SIZE;

	cvNamedWindow("test_color_table", 1 );

	for(int i = 0; i < COLOR_SIZE; i++) {
		cvCircle(img, cvPointFrom32f(pt), radius, color_tab[i], CV_FILLED);
		radius -= step;
	}

	cvShowImage("test_color_table", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("test_color_table");

}

void get_select_rect_by_2_point(CvPoint *pt1, CvPoint *pt2, CvRect *rect) {
	rect->x = MIN(pt1->x, pt2->x);
	rect->y = MIN(pt1->y, pt2->y);

	rect->width = MAX(pt1->x, pt2->x) - rect->x;
	rect->height = MAX(pt1->y, pt2->y) - rect->y;
}

CvScalar random_color(CvRNG* rng)
{
	int icolor = cvRandInt(rng);
	return CV_RGB(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

void test_seclect_region_on_mouse(int event, int x, int y, int flags, void* param) {
	static CvPoint origin;
	static CvPoint pre;
	static IplImage *img_op = NULL;
	static CvRNG rng = cvRNG((unsigned)-1);

	if(img_op == NULL) {
		img_op = cvCloneImage((IplImage *)param);
	}


	switch(event)
	{
		case CV_EVENT_MOUSEMOVE:
			if(flags & CV_EVENT_FLAG_LBUTTON) {
				CvPoint now = cvPoint(x, y);
				cvLine(img_op, pre, now, cvScalarAll(255), 5, 8, 0 );
				cvShowImage("test_seclect_region", img_op);
				//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
				pre = now;
			}
			break;
		case CV_EVENT_LBUTTONDOWN:
			origin = cvPoint(x, y);
			pre = origin;
			break;
		case CV_EVENT_LBUTTONUP:
			{
				CvRect rect;
				CvPoint now = cvPoint(x, y);
				get_select_rect_by_2_point(&origin, &now, &rect);

				cvSetImageROI(img_op, rect);
				cvXorS(img_op, cvScalarAll(255), img_op, 0);
				cvResetImageROI(img_op);
				cvRectangle(img_op, origin, now, 
						random_color(&rng), cvRandInt(&rng)%10-1, CV_AA, 0);

				cvShowImage("test_seclect_region", img_op);

				cvReleaseImage(&img_op);
				img_op = NULL;
			}
			break;
	}
}

void test_seclect_region() {
#define IMG_W 500
#define IMG_H 500
	IplImage* img = cvCreateImage(cvSize(IMG_W, IMG_H), 8, 3);
	cvNamedWindow("test_seclect_region", 1 );
	cvSetMouseCallback("test_seclect_region", test_seclect_region_on_mouse, img);
	cvShowImage("test_seclect_region", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("test_seclect_region");
}


void flannFindPairs(const CvSeq* objectDescriptors, const CvSeq* imageDescriptors, vector<int>& ptpairs) { 
	int length = (int)(objectDescriptors->elem_size/sizeof(float));

	Mat m_object(objectDescriptors->total, length, CV_32F);
	Mat m_image(imageDescriptors->total, length, CV_32F);

	// copy descriptors
	CvSeqReader obj_reader;
	float* obj_ptr = m_object.ptr<float>(0);
	cvStartReadSeq(objectDescriptors, &obj_reader);
	for(int i = 0; i < objectDescriptors->total; i++)
	{
		const float* descriptor = (const float*)obj_reader.ptr;
		CV_NEXT_SEQ_ELEM(obj_reader.seq->elem_size, obj_reader);
		memcpy(obj_ptr, descriptor, length*sizeof(float));
		obj_ptr += length;
	}

	CvSeqReader img_reader;
	float* img_ptr = m_image.ptr<float>(0);
	cvStartReadSeq(imageDescriptors, &img_reader);
	for(int i = 0; i < imageDescriptors->total; i++)
	{
		const float* descriptor = (const float*)img_reader.ptr;
		CV_NEXT_SEQ_ELEM(img_reader.seq->elem_size, img_reader);
		memcpy(img_ptr, descriptor, length*sizeof(float));
		img_ptr += length;
	}

	// find nearest neighbors using FLANN
	Mat m_indices(objectDescriptors->total, 2, CV_32S);
	Mat m_dists(objectDescriptors->total, 2, CV_32F);
	cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
	flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

	int* indices_ptr = m_indices.ptr<int>(0);
	float* dists_ptr = m_dists.ptr<float>(0);
	for (int i=0; i<m_indices.rows; ++i) {
		if (dists_ptr[2*i]<0.6*dists_ptr[2*i+1]) {
			ptpairs.push_back(i);//index in objectKeypoints
			ptpairs.push_back(indices_ptr[2*i]);//index in imageKeypoints
		}
	}
}

int get_object_homography(CvSeq *objectKeypoints, CvSeq *imageKeypoints, vector<int>& ptpairs, double h[9]) {
	CvMat _h = cvMat(3, 3, CV_64F, h);
	vector<CvPoint2D32f> pt1, pt2;
	CvMat _pt1, _pt2;
	int n = ptpairs.size()/2;

	pt1.resize(n);
	pt2.resize(n);
	for(int i = 0; i < n; i++)
	{
		pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i*2]))->pt;
		pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs[i*2+1]))->pt;
	}

	_pt1 = cvMat(1, n, CV_32FC2, &pt1[0]);//question:vector->array
	_pt2 = cvMat(1, n, CV_32FC2, &pt2[0]);
	return cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5);
}

void get_correspond_dots(double h[9], const vector<CvPoint>& src_dots, vector<CvPoint>& dst_dots) {
	for(int i = 0; i < src_dots.size(); i++)
	{
		double x = src_dots[i].x, y = src_dots[i].y;
		double Z = 1./(h[6]*x + h[7]*y + h[8]);
		double X = (h[0]*x + h[1]*y + h[2])*Z;
		double Y = (h[3]*x + h[4]*y + h[5])*Z;
		dst_dots[i] = cvPoint(cvRound(X), cvRound(Y));
	}
}

int find_object_in_image(IplImage* object, IplImage* image,
		vector<int>& ptpairs, double h[9],
		CvSeq *& objectKeypoints, CvSeq *& objectDescriptors,
		CvSeq *& imageKeypoints, CvSeq *& imageDescriptors) {
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSURFParams params = cvSURFParams(500, 1);

	double tt = (double)cvGetTickCount();
	cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
	printf("Object Descriptors: %d\n", objectDescriptors->total);
	cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );
	printf("Image Descriptors: %d\n", imageDescriptors->total);
	tt = (double)cvGetTickCount() - tt;
	printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));

	flannFindPairs(objectDescriptors, imageDescriptors, ptpairs);
	if(ptpairs.size() <= 0) {
		return -1;
	}

	if(get_object_homography(objectKeypoints, imageKeypoints, ptpairs, h) == -1) {
		return -1;
	}

	return 0;
}

void draw_rectangle_conors(IplImage* img, vector<CvPoint> cornors, CvScalar color, CvPoint offset) {
	if(cornors.size() < 2) {
		return;
	}

	for(int i = 0; i < cornors.size(); i++) {
		CvPoint r1 = cornors[i%cornors.size()];
		CvPoint r2 = cornors[(i+1)%cornors.size()];
		cvLine(img,
				cvPoint(r1.x + offset.x, r1.y + offset.y),
				cvPoint(r2.x + offset.x, r2.y + offset.y),
				color, 5, 8, 0);
	}
}

void test_find_object_in_image() {
	const char* object_filename = "box.png";
	const char* scene_filename = "box_in_scene.png";
	static CvScalar colors[] = 
	{
		{0,0,255},
		{0,128,255},
		{0,255,255},
		{0,255,0},
		{255,128,0},
		{255,255,0},
		{255,0,0},
		{255,0,255},
		{255,255,255}
	};
	IplImage* object = cvLoadImage( object_filename, CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* image = cvLoadImage( scene_filename, CV_LOAD_IMAGE_GRAYSCALE );
	if( !object || !image )
	{
		fprintf( stderr, "Can not load %s and/or %s\n"
				"Usage: find_obj [<object_filename> <scene_filename>]\n",
				object_filename, scene_filename );
		exit(-1);
	}


	IplImage* correspond = cvCreateImage(cvSize(image->width, object->height+image->height), 8, 1);
	cvSetImageROI(correspond, cvRect(0, 0, object->width, object->height));
	cvCopy(object, correspond);
	cvSetImageROI(correspond, cvRect(0, object->height, correspond->width, correspond->height));
	cvCopy(image, correspond);
	cvResetImageROI(correspond);
	IplImage* img_color = cvCreateImage(cvGetSize(correspond), 8, 3);
	cvCvtColor( correspond, img_color, CV_GRAY2BGR );
	cvReleaseImage(&correspond);


	vector<int> ptpairs;
	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
	double h[9];
	int result = find_object_in_image(object, image, ptpairs, h,
			objectKeypoints, objectDescriptors,
			imageKeypoints, imageDescriptors);
	if(result == -1) {
		return;
	}

	vector<CvPoint> src_cornors, dst_corners;
	src_cornors.push_back({0, 0});
	src_cornors.push_back({object->width, 0});
	src_cornors.push_back({object->width, object->height});
	src_cornors.push_back({0, object->height});
	dst_corners.resize(4);
	get_correspond_dots(h, src_cornors, dst_corners);
	draw_rectangle_conors(img_color, src_cornors, colors[6], {0,0});
	draw_rectangle_conors(img_color, dst_corners, colors[6], {0, object->height});

	for(int i = 0; i < (int)ptpairs.size(); i += 2)
	{
		CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i]);
		CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs[i+1]);
		cvLine(img_color, cvPointFrom32f(r1->pt),
				cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), colors[1]);
	}

	cvNamedWindow("test_find_object_in_image", 0 );
	cvShowImage("test_find_object_in_image", img_color);
	cvWaitKey(0);
	cvClearSeq(objectKeypoints);
	cvClearSeq(objectDescriptors);
	cvClearSeq(imageKeypoints);
	cvClearSeq(imageDescriptors);
	cvReleaseImage(&image);
	cvReleaseImage(&object);
	cvReleaseImage(&img_color);
	cvDestroyWindow("test_find_object_in_image");
}

void test_filter2d_on_mouse(int event, int x, int y, int flags, void* param) {
	static CvPoint pre;
	static IplImage *mask = NULL;
	static IplImage *copy = NULL;
	static IplImage *out = NULL;
	static IplImage *out_ = NULL;

	if(mask == NULL) {
		mask = cvCreateImage(cvGetSize((IplImage *)param), IPL_DEPTH_8U, 1);
		cvAndS(mask, cvScalarAll(0), mask, 0);

		out = cvCreateImage(cvGetSize((IplImage *)param), IPL_DEPTH_8U, 3);
		cvAndS(out, cvScalarAll(0), out, 0);

		copy = cvCloneImage((IplImage *)param);
	}


	switch(event)
	{
		case CV_EVENT_MOUSEMOVE:
			if(flags & CV_EVENT_FLAG_LBUTTON) {
				CvPoint now = cvPoint(x, y);
				cvLine(mask, pre, now, cvScalarAll(255), 50, 8, 0 );
				cvAnd(copy, copy, out, mask);
				cvShowImage("test_filter2d.mask", out);
				//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
				pre = now;
			}
			break;
		case CV_EVENT_LBUTTONDOWN:
			pre = cvPoint(x, y);
			break;
		case CV_EVENT_LBUTTONUP:
			{
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

				cvAndS(copy, cvScalarAll(0), copy, mask);

				out_ = cvCloneImage(out);

				cvFilter2D(out, out_, &km_high, cvPoint(-1, -1));  //设参考点为核的中心
				cvAdd(copy, out_, out_, 0);
				cvShowImage("filtering_high", out_); 

				cvFilter2D(out, out_, &km_low, cvPoint( -1, -1 ));  //设参考点为核的中心
				cvAdd(copy, out_, out_, 0);
				cvShowImage("filtering_low", out_); 

				cvReleaseImage(&mask);
				cvReleaseImage(&copy);
				cvReleaseImage(&out);
				cvReleaseImage(&out_);
				mask = NULL;
				copy = NULL;
				out = NULL;
				out_ = NULL;
			}
			break;
	}
}

int test_filter2d() {
	IplImage *src = cvLoadImage("001.jpg"); 
	IplImage *dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cvNamedWindow("test_filter2d", 0);
	cvNamedWindow("test_filter2d.mask", 0);
	cvSetMouseCallback("test_filter2d", test_filter2d_on_mouse, src);
	cvNamedWindow("filtering_low", 0);
	cvNamedWindow("filtering_high", 0);
	cvShowImage("test_filter2d", src);
	cvShowImage("test_filter2d.mask", src);

	cvWaitKey(0); 

	cvReleaseImage(&src); 
	cvReleaseImage(&dst); 
	cvDestroyWindow("filtering_low");
	cvDestroyWindow("filtering_high");
	cvDestroyWindow("test_filter2d");
	cvDestroyWindow("test_filter2d.mask");
}

int test_smooth() {
	//char* filename = "fruits.jpg";
	const char* filename = "001.jpg";
	//char* filename = "002.jpg";
	IplImage *image = NULL;
	int filter_type = 3;//CV_BLUR_NO_SCALE CV_BLUR CV_GAUSSIAN CV_MEDIAN CV_BILATERAL
	int window_size = 3;
	int edge_thresh1 = 14;
	int edge_thresh2 = 200;
	int filter_type_ = 3;
	int window_size_ = 3;
	int edge_thresh1_ = 14;
	int edge_thresh2_ = 200;

	if((image = cvLoadImage(filename, 1)) == 0)
		return -1;

	// Create the output image
	IplImage *cedge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);

	// Convert to grayscale
	IplImage *gray = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
	IplImage *edge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
	//cvCvtColor(image, gray, CV_BGR2GRAY);

	// Create a window
	cvNamedWindow("test_smooth", 1);
	cvNamedWindow("trackbar", 1);

	// create a toolbar
	cvCreateTrackbar("filter_type", "trackbar", &filter_type, 4, 0);
	cvCreateTrackbar("window_size", "trackbar", &window_size, 100, 0);
	cvCreateTrackbar("edge_thresh1", "trackbar", &edge_thresh1, 255, 0);
	cvCreateTrackbar("edge_thresh2", "trackbar", &edge_thresh2, 255, 0);

	// Wait for a key stroke; the same function arranges events processing
	while(true) {
		cvSmooth( image, cedge, filter_type_, window_size_, 0, 0 );

		cvCvtColor(cedge, gray, CV_BGR2GRAY);

		// Run the edge detector on grayscale
		cvCanny(gray, edge, (float)edge_thresh1_, (float)edge_thresh2_, 3);

		//cvShowImage(wndname, cedge);
		cvShowImage("test_smooth", edge);

		char c = (char)cvWaitKey(30);
		if(c == 'q') {
			break;
		} else if(c == 'a') {
			filter_type_ = filter_type;
			window_size_ = window_size == 0 ? 1 : window_size;
		} else {
			edge_thresh1_ = edge_thresh1;
			edge_thresh2_ = edge_thresh2;
		}
	}
	cvReleaseImage(&image);
	cvReleaseImage(&cedge);
	cvReleaseImage(&gray);
	cvReleaseImage(&edge);
	//cvReleaseImage(&gray);
	//cvReleaseImage(&edge);
	cvDestroyWindow("test_smooth");

	return 0;
}

int test_people_detect() {
	Mat img;
	Mat show;
	img = imread("003.jpg");
	int size_1 = 2,size_2 = 2;

	cvNamedWindow("test_people_detect", 1);

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	cvCreateTrackbar("size_1", "test_people_detect", &size_1, 100, 0);
	cvCreateTrackbar("size_2", "test_people_detect", &size_2, 100, 0);

	while(true) {
		img.copyTo(show);
		vector<Rect> found, found_filtered;
		double t = (double)getTickCount();
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(show, found, 0, Size(size_1,size_1), Size(size_2,size_2), 1.05, 2);
		t = (double)getTickCount() - t;
		printf("tdetection time = %gms\n", t*1000./getTickFrequency());

		size_t i, j;
		printf("found size:%d\n\n", found.size());
		for(i = 0; i < found.size(); i++)
		{
			Rect r = found[i];
			for(j = 0; j < found.size(); j++)
				if(j != i && (r & found[j]) == r)
					break;
			if(j == found.size())
				found_filtered.push_back(r);
		}
		for(i = 0; i < found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(show, r.tl(), r.br(), Scalar(0,255,0), 3);
		}
		imshow("test_people_detect", show);

		char c = (char)waitKey(0);
		if(c == 'q' || c == 'Q') {
			break;
		}
	}

	cvDestroyWindow("test_people_detect");
}

#include <iostream> 
int test_backproject() {
	IplImage *src = cvLoadImage("hand0.jpg");
	cvShowImage("src",src);
	IplImage *hsv_src = cvCloneImage(src);
	cvCvtColor(src,hsv_src,CV_BGR2HSV);

	IplImage *h_plane_src = cvCreateImage(cvGetSize(src),8,1);
	cvSplit(hsv_src,h_plane_src,NULL,NULL,NULL);
	cvSave("h.xml",h_plane_src);
	int dims = 1;
	int size[] = {180};
	float ranges_h[] ={0,181};
	float *ranges[] = {ranges_h};
	CvHistogram *hist_src = cvCreateHist(1,size,CV_HIST_ARRAY,ranges);
	cvCalcHist(&h_plane_src,hist_src);


	IplImage *dst = cvLoadImage("hand1.jpg");
	IplImage *hsv_dst = cvCreateImage(cvGetSize(dst),8,3);
	cvCvtColor(dst,hsv_dst,CV_BGR2HSV);
	IplImage *h_plane_dst = cvCreateImage(cvGetSize(dst),8,1);

	cvSplit(hsv_dst,h_plane_dst,NULL,NULL,NULL);

	IplImage *dst_probability = cvCreateImage(cvGetSize(h_plane_dst),IPL_DEPTH_8U,1);

	int threshold1 = 250, threshold2 = 255;
	cvCreateTrackbar("threshold1", "trackbar", &threshold1, 255, 0);
	cvCreateTrackbar("threshold2", "trackbar", &threshold2, 255, 0);

	while(true) {
		cvZero(dst_probability);
		cvCalcBackProject(&h_plane_dst,dst_probability,hist_src);
		cout<<cvSum(dst_probability).val[0]<<endl;
		cvShowImage("result",dst_probability);
		cvThreshold(dst_probability,dst_probability,threshold1,threshold2,CV_THRESH_BINARY);
		cvShowImage("dst1",dst_probability);
		IplConvKernel *kernel = cvCreateStructuringElementEx(31,31,15,15,CV_SHAPE_RECT);
		cvMorphologyEx(dst_probability,dst_probability,NULL,kernel,CV_MOP_OPEN);
		cvShowImage("dst2",dst_probability);

		if((char)cvWaitKey(200) == 'q') {
			break;
		}
	}

	cvReleaseImage(&src);
	cvReleaseImage(&hsv_src);
	cvReleaseImage(&h_plane_src);
	cvReleaseImage(&dst);
	cvReleaseImage(&hsv_dst);
	cvReleaseImage(&h_plane_dst);
	cvReleaseImage(&dst_probability);
	cvReleaseHist(&hist_src);
	cvDestroyAllWindows();
}


int test_open_close() {
	char mat[] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 1,
		0, 0, 0, 0, 0,
	};
	CvMat MAT = cvMat(3, 5, CV_8U, mat);
	CvMat *src = &MAT;
	CvMat *dst = cvCloneMat(src);
	IplConvKernel *kernel = cvCreateStructuringElementEx(3,1,1,0,CV_SHAPE_RECT);
	unsigned int dilate = 0;
	while(true) {
		if(dilate == 0) {
			//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
			cvErode(dst,dst,kernel,1);
		} else {
			//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
			cvDilate(dst,dst,kernel,1);
		}

		unsigned char *ptr = dst->data.ptr;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 5; j++) {
				printf("%d ", ptr[i * 5 + j]);	
			}
			printf("\n");
		}
		char c = (char)getchar();
		if(c == 'q') {
			break;
		}
		if(c == 's') {
			dilate = ~dilate;
		}
	}
	cvReleaseMat(&dst);
}

int test_cvload_cvsave() {
	CvMat *the_matrix_data = cvCreateMat(5,5,CV_32FC1);
	float element_3_2 = 7.7;
	*((float*)CV_MAT_ELEM_PTR( *the_matrix_data, 3,2) ) = element_3_2;
	cvmSet(the_matrix_data,4,4,0.5000);
	cvSetReal2D(the_matrix_data,3,3,0.5000);

	CvMat A = cvMat( 5, 5, CV_32F, the_matrix_data );

	printf("xiaofei_test:\nSaving my_matrix.xml\n");
	cvSave( "my_matrix.xml", &A );
	//. . .
	// to load it then in some other program use …
	printf("Loading my_matrix.xml\n");
	CvMat* A1 = (CvMat*) cvLoad( "my_matrix.xml" );
}

int test_face_detect() {
	Mat img = imread("001.jpg", 1);
	CascadeClassifier cascade, nestedCascade;
	cascade.load("data/haarcascades/haarcascade_frontalface_alt.xml");
	nestedCascade.load("data/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
	const Scalar colors[] = {
		CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255)
	};

	double scale = 1.0;
	Mat gray, smallImg(cvRound(img.rows/scale), cvRound(img.cols/scale), CV_8UC1);

	cvtColor(img, gray, CV_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	while(true) {
		double t = (double)cvGetTickCount();
		vector<Rect> faces;
		cascade.detectMultiScale( smallImg, faces, 1.1, 2, 
				0 
				//|CV_HAAR_FIND_BIGGEST_OBJECT 
				//|CV_HAAR_DO_ROUGH_SEARCH 
				//|CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30)
				);
		t = (double)cvGetTickCount() - t;
		printf("detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.));
		int i = 0;
		for(vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
		{
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i%8];
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
			if( nestedCascade.empty() )
				continue;
			smallImgROI = smallImg(*r);
			nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
					1.1, 2, 0
					//|CV_HAAR_FIND_BIGGEST_OBJECT
					//|CV_HAAR_DO_ROUGH_SEARCH
					//|CV_HAAR_DO_CANNY_PRUNING
					|CV_HAAR_SCALE_IMAGE
					, Size(30, 30)
					);
			for(vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
			{
				center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
				center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
				radius = cvRound((nr->width + nr->height)*0.25*scale);
				circle( img, center, radius, color, 3, 8, 0 );
			}
		}
		cvNamedWindow("test_face_detect", 1);
		cv::imshow("test_face_detect", img);    
		char c = (char)cvWaitKey(0);
		if(c == 'q') {
			break;
		}
	}

	cvDestroyWindow("test_color_table");
}

void split_string(char *buffer, vector<string>& strs) {
	int len = strlen(buffer);
	for(int i = 0; i < len; i++) {
		if(isspace(buffer[i]) != 0) {
			buffer[i] = 0;
		}
	}

	for(int i = 0; i < len; i++) {
		if(buffer[i] != 0) {
			strs.push_back(buffer + i);
			i += strlen(buffer + i);
		}
	}
}

void get_file_list(const char *fname, vector<string>& lines) {
	FILE *f = fopen(fname,"r");
	if(f <= 0) {
		return;
	}

	char line[LINE_BUFFER];
	//COUNT THE NUMBER OF IMAGES:
	//while(fscanf(f,"%s",name)==1){
	while(fgets(line, LINE_BUFFER, f) != NULL){
		lines.push_back(line);
	}

	fclose(f);
}

void save_file_list(const char *fname, vector<string>& lines) {
	FILE *f = fopen(fname,"w");
	if(f <= 0) {
		return;
	}

	for(int i = 0; i < lines.size(); i++) {
		//printf(">>>>%s\n", lines[i].c_str());
		if(fputs(lines[i].c_str(), f) <= 0) {
			break;
		}
	}

	fclose(f);
}

typedef struct _serial_img_info {
	int is_video;
	int frame_count;
	int index;
} serial_img_info_t;

serial_img_info_t serial_img_info = {0, 0, 0};

void get_serial_img_info(int *index, int *is_video, int *frame_count) {
	*index = serial_img_info.index;
	*is_video = serial_img_info.is_video;
	*frame_count = serial_img_info.frame_count;
}

IplImage * get_serial_img(vector<string>& lines) {
	static int index = 0;
	//static CvCapture* capture = NULL;
	static CvCapture* capture = cvCaptureFromCAM(0);

	static IplImage *img = NULL;


	while(true) {
		if(capture != NULL) {
			img = cvQueryFrame(capture);
			serial_img_info.frame_count = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
			if(img == NULL) {
				cvReleaseCapture(&capture);
				capture = NULL;
			} else {
				return img;
			}
		}

		if(img != NULL) {//pre img is picture
			cvReleaseImage(&img);
			img = NULL;
		}

		if(index >= lines.size()) {
			return img;
		}

		
		serial_img_info.index = index++;
		char fname[LINE_BUFFER];
		if(sscanf(lines[serial_img_info.index].c_str(), "%s", fname) != 1) {
			continue;
		}
		//printf("fname:%s\n", fname);

		char *format = strrchr(fname, '.');
		if(format == NULL) {
			continue;
		}

		if(access(fname, F_OK) != 0) {
			continue;
		}

		if(strncasecmp(format, ".avi", strlen(".avi")) == 0) {
			serial_img_info.is_video = 1;
			capture = cvCreateFileCapture(fname);
			img = cvQueryFrame(capture);
			serial_img_info.frame_count = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		} else {
			serial_img_info.is_video = 0;
			img = cvLoadImage(fname, 1);
		}
		
		if(img != NULL) {
			break;
		}
	}

	return img;
}

int test_head_detect() {
	vector<string> lines;
	get_file_list("test_object_list.txt", lines);

	CvMemStorage* storage = 0;
	CvHaarClassifierCascade* cascade = 0;
	//cascade = (CvHaarClassifierCascade *)cvLoad("data2/haarcascades/cascade4.xml", 0, 0, 0);
	cascade = cvLoadHaarClassifierCascade("data2/data/cascade.xml", cvSize(12,12));
	//cascade = cvLoadHaarClassifierCascade("data/haarcascades/haarcascade_frontalface_alt.xml", cvSize(12,12));

	if(!cascade)
	{
		printf("ERROR: Could not load classifier cascade\n");
	}
	storage = cvCreateMemStorage(0);

	const CvScalar colors[] = {
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};

	int video_play_switch = 0;

	IplImage *img;
	cvNamedWindow("test_head_detect", 0);
	while((img = get_serial_img(lines)) != NULL) {
		double scale = 1.0;
		IplImage *gray = cvCreateImage(cvSize(img->width,img->height), 8, 1);
		IplImage *small_img = cvCreateImage(
				cvSize(cvRound(img->width/scale), cvRound(img->height/scale)),
				8, 1
				);
		cvCvtColor(img, gray, CV_BGR2GRAY);
		cvResize(gray, small_img, CV_INTER_LINEAR);
		cvEqualizeHist(small_img, small_img);
		cvClearMemStorage(storage);

		double t = (double)cvGetTickCount();
		CvSeq* faces = cvHaarDetectObjects(
				small_img, cascade, storage,
				1.1, 2, 0,//CV_HAAR_DO_CANNY_PRUNING,
				cvSize(0, 0)
				);
		t = (double)cvGetTickCount() - t;
		printf("detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.));
		for(int i = 0; i < (faces ? faces->total : 0); i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(faces, i);
			cvRectangle(
					img,
					cvPoint(r->x * scale, r->y * scale),
					cvPoint((r->x + r->width) * scale, (r->y + r->height) * scale),
					colors[i%8], 1, 8
				   );
			/*
			   CvPoint center;
			   int radius;
			   center.x = cvRound((r->x + r->width*0.5)*scale);
			   center.y = cvRound((r->y + r->height*0.5)*scale);
			   radius = cvRound((r->width + r->height)*0.25*scale);
			   cvCircle( img, center, radius, colors[i%8], 1, 8, 0 );
			   */
		}

		cvShowImage("test_head_detect", img);
		//cvReleaseImage(&img);
		cvReleaseImage(&gray);
		cvReleaseImage(&small_img);

		int index = 0;
		int is_video = 0;
		int frame_count = 0;
		get_serial_img_info(&index, &is_video, &frame_count);

		int delay = 0;
		if((is_video != 0) && (video_play_switch != 0)) {
			delay = 30;
		}
		/////////////////////////////////////////
		delay = 30;

		char c = (char)cvWaitKey(delay);
		if(c == 'q') {
			break;
		} else if(c == 's') {
			video_play_switch = ~video_play_switch;
		}

	}
	cvDestroyWindow("test_head_detect");
}

typedef struct _test_make_object{
	IplImage* img;
	vector<CvRect> rects;
} test_make_object_t;

void test_make_object_on_mouse(int event, int x, int y, int flags, void* param) {
	static int valid = 0;
	static CvPoint origin;
	static CvRect select_rect;
	static IplImage *img_op = NULL;
	test_make_object_t *pdata = (test_make_object_t *)param;

	if(img_op == NULL) {
		img_op = cvCloneImage(pdata->img);
		if(img_op == NULL) {
			return;
		}
	}

	for(int i = 0; i < pdata->rects.size(); i++) {
		CvRect rect = pdata->rects[i];
		cvRectangle(
				img_op,
				cvPoint(rect.x, rect.y),
				cvPoint(
					rect.x + rect.width,
					rect.y + rect.height
				       ), 
				color_tab[1], 1, CV_AA, 0
			   );
	}

	switch(event)
	{
		case CV_EVENT_MOUSEMOVE:
			if(flags & CV_EVENT_FLAG_LBUTTON) {
				CvPoint now = cvPoint(x, y);
				cvRectangle(img_op, origin, now, 
						color_tab[0], 1, CV_AA, 0);
				cvShowImage("test_make_object", img_op);
			}
			break;
		case CV_EVENT_LBUTTONDOWN:
			origin = cvPoint(x, y);
			break;
		case CV_EVENT_LBUTTONUP:
			{
				CvPoint now = cvPoint(x, y);
				cvRectangle(
						img_op,
						origin,
						now, 
						color_tab[0], 1, CV_AA, 0
					   );
				cvShowImage("test_make_object", img_op);
				get_select_rect_by_2_point(&origin, &now, &select_rect);
				valid = 1;
			}
			break;
		case CV_EVENT_RBUTTONUP:
			{
				if(valid != 1) {
					return;
				}
				valid = 0;

				cvRectangle(
						img_op,
						cvPoint(select_rect.x, select_rect.y),
						cvPoint(
							select_rect.x + select_rect.width,
							select_rect.y + select_rect.height
						       ), 
						color_tab[1], 1, CV_AA, 0
					   );
				cvShowImage("test_make_object", img_op);
				pdata->rects.push_back(select_rect);
			}
			break;
	}
	cvReleaseImage(&img_op);
	img_op = NULL;
}

int test_make_object() {
	vector<string> lines;
	vector<string> video_lines;
	get_file_list("flist_out.txt", lines);

	cvNamedWindow("test_make_object", 1 );

	test_make_object_t data;
	cvSetMouseCallback("test_make_object", test_make_object_on_mouse, &data);

	IplImage *img;

	int reset = 0;
	int video_play_switch = 0;

	while((reset != 0) || ((img = get_serial_img(lines)) != NULL)) {
		data.img = img;
		data.rects.clear();

		int index = 0;
		int is_video = 0;
		int frame_count = 0;
		get_serial_img_info(&index, &is_video, &frame_count);

		char line[LINE_BUFFER];
		memset(line, 0, LINE_BUFFER);
		vector<string> line_info;

		strncpy(line, lines[index].c_str(), LINE_BUFFER);
		split_string(line, line_info);

		int line_info_size = line_info.size();
		if(line_info_size > 0) {
			//printf("%s:%s", line_info[0].c_str(), is_video != 0 ? "video" : "picture");
			//if(is_video != 0) {
			//	printf("(%d)", frame_count);
			//}
			//printf("\n");

			int rect_count = 0;
			if(line_info_size > 1) {
				if(sscanf(line_info[1].c_str(), "%d", &rect_count) == 1) {
					if(
						((line_info_size - 2) / 4 > 0)
						&& ((line_info_size - 2) % 4 == 0)
					) {
					} else {
						rect_count = 0;
					}
				}
			}

			if(is_video != 0) {
				rect_count = 0;
			}

			if(reset != 0) {
				reset = 0;
				rect_count = 0;
			}

			if(rect_count > 0) {
				for(int i = 2; i < line_info_size; i += 4) {
					int x, y, w, h;
					if(
						(sscanf(line_info[i + 0].c_str(), "%d", &x) == 1)
						&& (sscanf(line_info[i + 1].c_str(), "%d", &y) == 1)
						&& (sscanf(line_info[i + 2].c_str(), "%d", &w) == 1)
						&& (sscanf(line_info[i + 3].c_str(), "%d", &h) == 1)
					) {
						//printf("%d, %d, %d, %d\n", x, y, w, h);
						data.rects.push_back(cvRect(x, y, w, h));
					}
				}
			}
		}

		IplImage *img_op = cvCloneImage(img);
		for(int i = 0; i < data.rects.size(); i++) {
			CvRect rect = data.rects[i];
			cvRectangle(
					img_op,
					cvPoint(rect.x, rect.y),
					cvPoint(
						rect.x + rect.width,
						rect.y + rect.height
					       ), 
					color_tab[1], 1, CV_AA, 0
				   );
		}
		cvShowImage("test_make_object", img_op);
		cvReleaseImage(&img_op);
		img_op = NULL;

		int delay = 0;
		if((is_video != 0) && (video_play_switch != 0)) {
			delay = 30;
		}

		char c = (char)cvWaitKey(delay);

		if(c == 'q') {
			break;
		} else if(c == 'w'){
			int offset = 0;
			int free = LINE_BUFFER;
			int rect_count = data.rects.size();
			offset += snprintf(line + offset, free -= offset, "%s", line_info[0].c_str());
			if(is_video != 0) {
				offset += snprintf(line + offset, free -= offset, "_");
				if(access(line, F_OK) != 0) {
					mkdir(line, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				}
				offset += snprintf(line + offset, free -= offset, "/%010d.jpg", frame_count);
				cvSaveImage(line, img);
			}

			if(rect_count != 0) {
				offset += snprintf(line + offset, free -= offset, " %d", rect_count);
			}
			for(int i = 0; i < rect_count; i++) {
				offset += snprintf(line + offset, free -= offset, " %d %d %d %d",
						data.rects[i].x,
						data.rects[i].y,
						data.rects[i].width,
						data.rects[i].height
						);
			}
			offset += snprintf(line + offset, free -= offset, "\n");

			if(is_video != 0) {
				video_lines.push_back(line);
			} else {
				lines[index] = line;
			}
		} else if(c == 'd') {
			int offset = 0;
			int free = LINE_BUFFER;
			offset += snprintf(line + offset, free -= offset, "%s", line_info[0].c_str());
			offset += snprintf(line + offset, free -= offset, "\n");
			lines[index] = line;
		} else if(c == 'r') {
			reset = 1;
		} else if(c == 's') {
			video_play_switch = ~video_play_switch;
		}
	}
	cvDestroyWindow("test_make_object");
	save_file_list("flist_out.txt", lines);
	if(video_lines.size() != 0) {
		save_file_list("flist_out.new.txt", video_lines);
	}
}

int test_resize_samples() {
	vector<string> lines;
	vector<string> out_lines;
	get_file_list("negdata.txt", lines);
	IplImage *img;
	IplImage* sample_img = cvCreateImage(cvSize(12, 12), 8, 3);
	int sample_index = 0;
	test_make_object_t data;
	while(img = get_serial_img(lines)) {
		data.img = img;
		data.rects.clear();
	
		int index = 0;
		int is_video = 0;
		int frame_count = 0;
		get_serial_img_info(&index, &is_video, &frame_count);

		char line[LINE_BUFFER];
		memset(line, 0, LINE_BUFFER);
		vector<string> line_info;

		strncpy(line, lines[index].c_str(), LINE_BUFFER);
		split_string(line, line_info);

		int line_info_size = line_info.size();
		if(line_info_size > 0) {
			//printf("%s:%s", line_info[0].c_str(), is_video != 0 ? "video" : "picture");
			//if(is_video != 0) {
			//	printf("(%d)", frame_count);
			//}
			//printf("\n");

			int rect_count = 0;
			if(line_info_size > 1) {
				if(sscanf(line_info[1].c_str(), "%d", &rect_count) == 1) {
					if(
						((line_info_size - 2) / 4 > 0)
						&& ((line_info_size - 2) % 4 == 0)
					) {
					} else {
						rect_count = 0;
					}
				}
			}

			if(is_video != 0) {
				rect_count = 0;
			}

			if(rect_count > 0) {
				for(int i = 2; i < line_info_size; i += 4) {
					int x, y, w, h;
					if(
						(sscanf(line_info[i + 0].c_str(), "%d", &x) == 1)
						&& (sscanf(line_info[i + 1].c_str(), "%d", &y) == 1)
						&& (sscanf(line_info[i + 2].c_str(), "%d", &w) == 1)
						&& (sscanf(line_info[i + 3].c_str(), "%d", &h) == 1)
					) {
						printf("%d, %d, %d, %d\n", x, y, w, h);
						data.rects.push_back(cvRect(x, y, w, h));
					}
				}
			} else {
				//printf("%d, %d, %d, %d\n", 0, 0, img->width, img->height);
				data.rects.push_back(cvRect(0, 0, img->width, img->height));
			}
		}

		for(int i = 0; i < data.rects.size(); i++) {
			CvRect rect = data.rects[i];
			IplImage* small = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
			cvSetImageROI(img, cvRect(rect.x, rect.y, rect.width, rect.height));
			cvCopy(img, small);
			cvResetImageROI(img);
			cvResize(small, sample_img, CV_INTER_LINEAR);

			int offset = 0;
			int free = LINE_BUFFER;
			offset += snprintf(line + offset, free -= offset, "samples_out");
			if(access(line, F_OK) != 0) {
				mkdir(line, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			}
			offset += snprintf(line + offset, free -= offset, "/%010d.jpg", sample_index++);
			cvSaveImage(line, small);
			offset += snprintf(line + offset, free -= offset, "\n");
			out_lines.push_back(line);
			printf("%s\n", line);

			cvReleaseImage(&small);
		}
	}
	save_file_list("samples_out.txt", out_lines);

	return 0;
}

void drawOptFlowMap(const CvMat* flow, CvMat* cflowmap, int step, double scale, CvScalar color) {
	int x, y;
	for( y = 0; y < cflowmap->rows; y += step)
		for( x = 0; x < cflowmap->cols; x += step)
		{
			CvPoint2D32f fxy = CV_MAT_ELEM(*flow, CvPoint2D32f, y, x);
			cvLine(cflowmap, cvPoint(x,y), cvPoint(cvRound(x+fxy.x), cvRound(y+fxy.y)),
					color, 1, 8, 0);
			cvCircle(cflowmap, cvPoint(x,y), 2, color, -1, 8, 0);
		}
}

int test_optical_flow() {
	//CvCapture* capture = cvCreateFileCapture("test.avi");
	CvCapture* capture = cvCaptureFromCAM(0);
	CvMat* prevgray = 0, *gray = 0, *flow = 0, *cflow = 0;

	if(!capture) {
		printf("open camera error!\n");
		return -1;
	}


	cvNamedWindow("test_optical_flow", 1);

	int firstFrame = 1;
	for(;;)
	{
		IplImage* frame = cvQueryFrame(capture);
		if(!frame)
			break;
		if(!gray)
		{
			gray = cvCreateMat(frame->height, frame->width, CV_8UC1);
			prevgray = cvCreateMat(gray->rows, gray->cols, gray->type);
			flow = cvCreateMat(gray->rows, gray->cols, CV_32FC2);
			cflow = cvCreateMat(gray->rows, gray->cols, CV_8UC3);
		}
		cvCvtColor(frame, gray, CV_BGR2GRAY);

		if( !firstFrame )
		{
			cvCalcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			cvCvtColor(prevgray, cflow, CV_GRAY2BGR);
			drawOptFlowMap(flow, cflow, 16, 1.5, CV_RGB(0, 255, 0));
			cvShowImage("test_optical_flow", cflow);
		} else {
			firstFrame = 0;
		}
		if(cvWaitKey(30)>=0)
			break;
		{
			CvMat* temp;    
			CV_SWAP(prevgray, gray, temp);
		}
	}

	cvReleaseMat(&gray);
	cvReleaseMat(&prevgray);
	cvReleaseMat(&flow);
	cvReleaseMat(&cflow);
	cvReleaseCapture(&capture);

	cvDestroyWindow("test_optical_flow");
	return 0;
}

int main(int argc, char** argv) {
	//test_color_table();
	//test_seclect_region();
	//test_find_object_in_image();
	test_optical_flow();
	//test_filter2d();
	//test_smooth();
	//test_people_detect();
	//test_backproject();
	//test_open_close();
	//test_face_detect();
	//test_head_detect();
	//test_make_object();
	//test_resize_samples();
	return 0;
}
