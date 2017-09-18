

/*================================================================
 *   Copyright (C) 2017年09月18日 肖飞 All rights reserved
 *
 *   文件名称：test_rbtree.h
 *   创 建 者：肖飞
 *   创建日期：2017年09月18日 星期一 12时00分00秒
 *   修改日期：2017年09月18日 星期一 12时53分31秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_RBTREE_H
#define _TEST_RBTREE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "rbtree.h"

#ifdef __cplusplus
}
#endif

#define container_of(ptr, type, member) ({ \
	const typeof(((type *)0)->member) *__mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member));})

#endif //_TEST_RBTREE_H
