#include<stdio.h>

extern "C"
int main(int argc, char **argv) {
	int i;

	printf("argc:%d\n", argc);
	printf("argv:", argc);
	for(i = 0; i < argc; i++) {
		printf("%d:%s ", i, argv[i]);
	}
	printf("\n");
	return 0;
}
