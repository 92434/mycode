

/*================================================================
 *   Copyright (C) 2017年07月12日 肖飞 All rights reserved
 *
 *   文件名称：ft_lib.c
 *   创 建 者：肖飞
 *   创建日期：2017年07月12日 星期三 11时54分55秒
 *   修改日期：2017年07月13日 星期四 11时14分57秒
 *   描    述：
 *
 *================================================================*/
#include "ft_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct _ft_device {
	char *buffer;
	int size;
	//int  (*get_image)(char *buffer, int len);
	//int  (*set_image)(char *buffer, int len);
	ft_printf_t ft_printf;
} ft_device_t;

static ft_device_t *ft_device;

int ft_lib_init()
{
	int ret = 0;

	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	ft_device = (ft_device_t *)malloc(sizeof(ft_device_t));

	if(ft_device != NULL) {
		bzero(ft_device, sizeof(ft_device_t));
	} else {
		ret = -1;
	}

	return ret;
}

int ft_lib_uninit()
{
	int ret = 0;

	if(ft_device == NULL) {
		return ret;
	}

	if(ft_device->buffer != NULL) {
		free(ft_device->buffer);
		ft_device->buffer = NULL;
	}

	free(ft_device);
	ft_device = NULL;

	return ret;
}

int ft_lib_set_log(ft_printf_t ft_printf)
{
	int ret = 0;

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	ft_device->ft_printf = ft_printf;

	return ret;
}

static int ft_printf(char *fmt, ...)
{
	int ret = 0;
	va_list ap;
	char buffer[1024];

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if(ft_device->ft_printf == NULL) {
		ret = -1;
		return ret;
	}

	va_start(ap, fmt);
	ret = vsnprintf(buffer, 1024, fmt, ap);
	va_end(ap);

	ret = ft_device->ft_printf(buffer);
	return ret;
}

int ft_get_image(char *buffer, int len)
{
	int ret = 0;

	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if((ft_device->buffer != NULL) && (ft_device->size >= len)) {
		memcpy((void *)buffer, (void *)ft_device->buffer, len);
	} else {
		ret = -1;
	}

	return ret;
}

int ft_set_image(const char *buffer, int len)
{
	int ret = 0;

	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if(ft_device->size < len) {
		if(ft_device->buffer != NULL) {
			free(ft_device->buffer);

			ft_device->size = 0;
			ft_device->buffer = NULL;
		}

		ft_device->buffer = (char *)malloc(len);
		ft_device->size = len;
	}


	if(ft_device->buffer != NULL) {
		memcpy((void *)ft_device->buffer, (void *)buffer, len);
	} else {
		ret = -1;
	}

	return ret;
}
