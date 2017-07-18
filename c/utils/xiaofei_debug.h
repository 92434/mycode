#ifndef _XIAOFEI_DEBUG_H
#define _XIAOFEI_DEBUG_H
#include <stdio.h>

//#define mydebug(format, args...) printf("[%s:%s:%d]:"format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## args)
#define mydebug(format, ...) printf("[%s:%s:%d]:"format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)
#define mydebug(format, ...) printf("[%s:%s:%d]:"format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)
#define myprintf(format, ...) printf(format, ## __VA_ARGS__)
#define myprintf_once(condition, format, ...) ({ \
	static int state = 0;\
	int myprintf_once_ret = 0; \
	\
	if(condition) { \
		if(state == 0) { \
			myprintf_once_ret = printf(format, ## __VA_ARGS__); \
			state = 1; \
		} else {} \
	} else { \
		state = 0; \
	} \
	myprintf_once_ret; \
})
#endif //#ifndef _XIAOFEI_DEBUG_H
