#include<stdio.h>

extern "C"
int test_args(int argc, char **argv) {
	int i;

	printf("argc:%d\n", argc);
	for(i = 0; i < argc; i++) {
		printf("%d:%s ", i, argv[i]);
	}
	printf("\n");
	return 0;
}

typedef struct {
	const char *name;
	int age;
} st_t;

st_t xiaofei = {.name = "xiaofei", .age = 18};
extern "C"
const st_t *test_char_p() {
	return &xiaofei;
}
