#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<linux/videodev2.h>

/*摄像头开关*/

//#define XXXX_150


/*摄像头参数id列表*/
#define BRIGHTNESS_ID 0x00980900
#define CONTRAST_ID 0x00980901
#define SATURATION_ID 0x00980902
#define HUE_ID 0x00980903
#define WHITE_BALANCE_TEMP_AUTO_ID 0x0098090c
#define GAMMA_ID 0x00980910
#define POWER_LINE_FREQUENCY_ID 0x00980918
#define WHITE_BALANCE_TEMP_ID 0x0098091a
#define SHARPNESS_ID 0x0098091b
#define BACKLIGHT_COMPENSATION_ID 0x0098091c
#define EXPOSURE_AUTO_ID 0x009a0901
#define EXPOSURE_ABSOLUTE_ID 0x009a0902
#define EXPOSURE_AUTO_PRIORITY_ID 0x009a0903


struct v4l2_capability cap;
struct v4l2_queryctrl qctrl;
struct v4l2_control ctrl;
struct v4l2_format Format;
struct v4l2_streamparm Stream_Parm;


void get_v4l2_info(int fd)
{
	printf("************************Get format info********************************\n");
	Format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(fd, VIDIOC_G_FMT, &Format) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[width:%d]\t[pixelformat:%d]\n", Format.fmt.pix.width, Format.fmt.pix.height);
	printf(">:[format:%d]\t[field:%d]\n", Format.fmt.pix.pixelformat, Format.fmt.pix.field);
	printf(">:[bytesperline:%d]\t[sizeimage:%d]\n", Format.fmt.pix.bytesperline, Format.fmt.pix.sizeimage);
	printf(">:[colorspace:%d]\n", Format.fmt.pix.colorspace);
	printf("***********************************************************************\n");
	printf("\n");

	printf("************************Get Stream_Parm********************************\n");
	Stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(fd, VIDIOC_G_PARM, &Stream_Parm) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Frame rate:%u] [%u]\n", Stream_Parm.parm.capture.timeperframe.numerator, Stream_Parm.parm.capture.timeperframe.denominator);
	printf(">:[capability:%d] [capturemode:%d]\n", Stream_Parm.parm.capture.capability, Stream_Parm.parm.capture.capturemode);
	printf(">:[extendemode:%d] [readbuffers:%d]\n", Stream_Parm.parm.capture.extendedmode, Stream_Parm.parm.capture.readbuffers);
	printf("***********************************************************************\n");
	printf("\n");

	printf("*************************Get Exposure Auto*****************************\n");
	ctrl.id = EXPOSURE_AUTO_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:Get Exposure Auto:[%d]\n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

	printf("*************************Get Exposure Absolute*****************************\n");
	ctrl.id = EXPOSURE_ABSOLUTE_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:Get Exposure Absolute:[%d]\n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

	printf("***************************Get Exposure Auto Priority************************\n");
	ctrl.id = EXPOSURE_AUTO_PRIORITY_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get Exposure Auto Priority:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");
#ifdef XXXX_150
	printf("***************************Get Gain value************************\n");
	ctrl.id = 0x00980913;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get Gain value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");
#endif
	printf("***************************Get white balance auto value************************\n");
	ctrl.id = WHITE_BALANCE_TEMP_AUTO_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get white balance auto value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

	printf("***************************Get white balance value************************\n");
	ctrl.id = WHITE_BALANCE_TEMP_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get white balance value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

	printf("***************************Get brightness value************************\n");
	ctrl.id = BRIGHTNESS_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get brightness value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

	printf("***************************Get contrast value************************\n");
	ctrl.id = CONTRAST_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get contrast value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");


	printf("***************************Get saturation value************************\n");
	ctrl.id = SATURATION_ID;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf(">:[Get saturation value:%d] \n", ctrl.value);
	printf("***********************************************************************\n");
	printf("\n");

}

void set_v4l2_para(fd, value)
{

	printf("Set format………………………\n");
	memset(&Format, 0, sizeof(struct v4l2_format));
	Format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	Format.fmt.pix.width =  320;
	Format.fmt.pix.height = 240;
	Format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	Format.fmt.pix.field = (enum v4l2_field)1;

	if(ioctl(fd, VIDIOC_S_FMT, &Format) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");

	printf("Set Stream_Parm……………………\n");
	Stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	Stream_Parm.parm.capture.timeperframe.denominator = 30;
	Stream_Parm.parm.capture.timeperframe.numerator = 1;

	if(ioctl(fd, VIDIOC_S_PARM, &Stream_Parm) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#if 0
	printf("Set Exposure Auto Type…………………\n");
	ctrl.id = 0x009a0901;
	ctrl.value = 3;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#endif
#if 0
	printf("Set Exposure value………………\n");
	ctrl.id = 0x009a0902;
	ctrl.value = 312;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#endif
#if 1
	printf("Set Exposure Auto Priority ………………\n");
	ctrl.id = 0x009a0903;
	ctrl.value = 2;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#endif
#ifdef XXXX_150
	printf("Set Gain value……………………\n");
	ctrl.id = 0x00980913;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#endif
#if 1
	printf("Set white balance Temperature Auto……………………\n");
	ctrl.id = WHITE_BALANCE_TEMP_AUTO_ID;
	ctrl.value = value;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
#endif
	printf("Set white balance Temperature……………………\n");
	ctrl.id = WHITE_BALANCE_TEMP_ID;
	ctrl.value = value;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");

	printf("Set brightness value……………………\n");
	ctrl.id = BRIGHTNESS_ID;
	ctrl.value = value;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");

	printf("Set contrast value………………\n");
	ctrl.id = CONTRAST_ID;
	ctrl.value = value;

	if(ioctl(fd, VIDIOC_S_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");

	printf("Set saturation value…………………\n");
	ctrl.id = SATURATION_ID;
	ctrl.value = value;

	if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	sleep(1);
	printf("\n");
	printf("**********************All Set Success*********************\n");
}

void main()
{
	int fd = -1;
	int rel = -1;
	int i;
	fd = open("/dev/video0", O_RDWR, 0);

	if(fd == -1) {
		perror("fd");
		exit(EXIT_FAILURE);
	}

	rel = ioctl(fd, VIDIOC_QUERYCAP, &cap);

	if(rel == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	printf("*********************Querycap Success**************************\n");

	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
		printf("*****************capture is on*****************************\n");
	}

	if(cap.capabilities & V4L2_CAP_STREAMING) {
		printf("*****************Stream is on******************************\n");
	}

	printf("\n");
	printf("\n");
	printf("*************************Capabilities**************************\n");
	printf(">:[Driver:%s]\n>:[card:%s]\n>:[bus_info:%s]\n>:[version:%d]\n",
		   cap.driver, cap.card, cap.bus_info, cap.version);
	printf("**************************************************************\n");
	printf("\n");
	printf("\n");

	for(i = V4L2_CID_BASE; i < V4L2_CID_LASTP1; i++) {
		qctrl.id = i;

		if(ioctl(fd, VIDIOC_QUERYCTRL, &qctrl) == 0) {
			ctrl.id = qctrl.id;

			if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) < 0) {
				perror("**********************get ctrl failed**************\n");
			}

			printf(">:[name:%s]\t[id:%08x]\n", qctrl.name, qctrl.id);
			printf(">:[type:%d]\n", qctrl.type);
			printf(">:[min:%d]\t[max:%d]\n", qctrl.minimum, qctrl.maximum);
			printf(">:[value:%d]\t[stp:%d]\n", ctrl.value, qctrl.step);
			printf(">:[default_value:%d]\n", qctrl.default_value);
			printf("\n");
		}
	}

	printf("------------------------------------------------------------------\n");
	printf(">:0x%08x\n", V4L2_CID_LASTP1);

	for(i = 0x009a0900; i < 0x009a091a; i++) {
		qctrl.id = i;

		if(ioctl(fd, VIDIOC_QUERYCTRL, &qctrl) == 0) {
			ctrl.id = qctrl.id;

			if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) < 0) {
				perror("**********************get ctrl failed**************\n");
			}

			printf(">:[name:%s]\t[id:%08x]\n", qctrl.name, qctrl.id);
			printf(">:[type:%d]\n", qctrl.type);
			printf(">:[min:%d]\t[max:%d]\n", qctrl.minimum, qctrl.maximum);
			printf(">:[value:%d]\t[stp:%d]\n", ctrl.value, qctrl.step);
			printf(">:[default_value:%d]\n", qctrl.default_value);
			printf("\n");
		}
	}

#if 0
	printf("---------------------------------2--------------------------------\n");

	for(i = 0x08000000; i < 0x08000010; i++) {
		qctrl.id = i;

		if(ioctl(fd, VIDIOC_QUERYCTRL, &qctrl) == 0) {
			ctrl.id = qctrl.id;

			if(ioctl(fd, VIDIOC_G_CTRL, &ctrl) < 0) {
				perror("**********************get ctrl failed**************\n");
			}

			printf(">:[name:%s]\t[id:%08x]\n", qctrl.name, qctrl.id);
			printf(">:[type:%d]\n", qctrl.type);
			printf(">:[min:%d]\t[max:%d]\n", qctrl.minimum, qctrl.maximum);
			printf(">:[value:%d]\t[stp:%d]\n", ctrl.value, qctrl.step);
			printf(">:[default_value:%d]\n", qctrl.default_value);
			printf("\n");
		}
	}

#endif

	get_v4l2_info(fd);

	set_v4l2_para(fd, 30);

}
