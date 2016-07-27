#include<stdio.h>

extern "C"
int test(int num) {
	printf("test %d\n", num);
}

extern "C"
int main(int argc, char *argv) {
	int i;

	printf("argc:%d\n", argc);
	printf("argv:%s\n", argv);
	test(1);
	return 0;
}
