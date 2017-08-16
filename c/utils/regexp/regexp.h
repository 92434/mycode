

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *   
 *   文件名称：regexp.h
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 10时06分43秒
 *   修改日期：2017年06月26日 星期一 17时49分31秒
 *   描    述：
 *
 *================================================================*/
#ifndef _REGEXP_H
#define _REGEXP_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <vector>
#include <string>
#include <regex.h>
#include <stdio.h>

#define SUBSLEN 10 /* 匹配子串的数量 */
#define EBUFLEN 128 /* 错误消息buffer长度 */
#define BUFLEN 1024 /* 匹配到的字符串buffer长度 */

class regexp
{
public:
	regexp();
	~regexp();

	std::vector<std::string> match(std::string string, std::string pattern);
};
#endif //REGEXP_H
