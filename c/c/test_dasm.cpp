#include "stdio.h"
#include "stdlib.h"

int i = 0;

void a() {
	printf("%s\n", __func__);
	printf("%d\n", i++);
}

void b() {
	printf("%s\n", __func__);
	printf("%d\n", i++);
}

void c() {
	printf("%s\n", __func__);
	printf("%d\n", i++);
}

int main()
{
	printf("%p\n", (void *)main);
	a();
	b();
	c();
	return 0;
}
