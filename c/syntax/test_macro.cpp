#include <iostream>
#include <stdio.h>
using namespace std;

#define my_print0(cp, var...) printf(cp, ##var)

#define my_print1(cp, var...) my_print0(cp, ##var)

#define cond_print(cp, ...) do { \
	if(p == 0) { \
		my_print0("cond--0"cp, __VA_ARGS__); \
	} else { \
		my_print0("cond--1"cp, __VA_ARGS__); \
	} \
} while(0)

#define my_print2(cp, ...) cond_print(cp, __VA_ARGS__)

#define _file __FILE__
#define _func __func__
#define _line __LINE__

#define print_pos() do { \
	printf("%s:%s:%d\n", _file, _func, _line); \
} while(0)

static int p = 1;
static char xiaofei[] = "xiaofei";

int main()
{
	my_print0("xiaofei %d\n", 3);
	my_print1("xiaofei\n");
	my_print0("xiaofei %d\n", 3);

	my_print0("xiaofei %s\n", XIAOFEI);

	my_print2("xiaofei %s\n", XIAOFEI);

	print_pos();
	print_pos();
	print_pos();
	return 0;
}

