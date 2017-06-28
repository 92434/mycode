

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：regexp.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 10时06分48秒
 *   修改日期：2017年06月28日 星期三 11时35分51秒
 *   描    述：
 *
 *================================================================*/
#include "regexp.h"
//#define ENABLE_INFO

regexp::regexp()
{
}

regexp::~regexp()
{
}

std::vector<std::string> regexp::match(std::string content, std::string pattern)
{
	std::vector<std::string> matched_groups;
	char error_buffer[EBUFLEN]; /* 存储错误消息 */
	size_t len;
	int error;
	int i;

	regex_t re; /* 存储编译好的正则表达式，正则表达式在使用之前要经过编译 */
	regmatch_t sub_matched[SUBSLEN]; /* 存储匹配到的字符串位置 */

#if defined(ENABLE_INFO)
	printf("content: %s\n", content.c_str());
	printf("pattern: \"%s\"\n", pattern.c_str());
#endif

	/* 编译正则表达式 */
	error = regcomp(&re, pattern.c_str(), REG_EXTENDED);

	if (error != 0) {
		len = regerror(error, &re, error_buffer, sizeof(error_buffer));
#if defined(ENABLE_INFO)
		printf("error: regcomp: %s\n", error_buffer);
#endif
		return matched_groups;
	}

#if defined(ENABLE_INFO)
	printf("Total has subexpression: %d\n", (int)re.re_nsub);
#endif

	/* 执行模式匹配 */
	error = regexec(&re, content.c_str(), (size_t)SUBSLEN, sub_matched, 0);

	if(error != 0) {
		if (error == REG_NOMATCH) { /* 没有匹配成功 */
			regfree(&re);
#if defined(ENABLE_INFO)
			printf("Sorry, no match ...\n");
#endif
		}

		len = regerror(error, &re, error_buffer, sizeof(error_buffer));

#if defined(ENABLE_INFO)
		printf("error: regexec: %*s\n", (int)len, error_buffer);
#endif

		return matched_groups;
	}

	/* 如果不是REG_NOMATCH并且没有其它错误，则模式匹配上 */
#if defined(ENABLE_INFO)
	printf("OK, has matched ...\n");
#endif

	for (int i = 0; i <= re.re_nsub; i++) {
		len = sub_matched[i].rm_eo - sub_matched[i].rm_so;
		std::string matched = content.substr(sub_matched[i].rm_so, len);

#if defined(ENABLE_INFO)
		if(i > 0) {
			printf("subexpression %d ", i);
		}

		printf ("begin: %d, len = %d ", (int)sub_matched[i].rm_so, (int)len);

		printf("match: %s\n", matched.c_str());
#endif

		matched_groups.push_back(matched);
	}

	regfree(&re); /* 用完了别忘了释放 */

	return matched_groups;
}
