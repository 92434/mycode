

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：regexp.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时17分07秒
 *   修改日期：2017年07月14日 星期五 12时18分01秒
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
	char error_buffer[EBUFLEN];
	size_t len;
	int error;
	int i;

	regex_t re;
	regmatch_t sub_matched[SUBSLEN];

#if defined(ENABLE_INFO)
	printf("content: %s\n", content.c_str());
	printf("pattern: \"%s\"\n", pattern.c_str());
#endif

	if(pattern.size() == 0) {
		return matched_groups;
	}

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

	error = regexec(&re, content.c_str(), (size_t)SUBSLEN, sub_matched, 0);

	if(error != 0) {
		if (error == REG_NOMATCH) {
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

	regfree(&re);

	return matched_groups;
}
