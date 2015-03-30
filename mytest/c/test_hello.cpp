#include "stdio.h"
#include "stdlib.h"

void hello(int i1, int i2) {

	int ii1 = i1;
	int ii2 = i2;
	char a[]={'h','e','l','l','o','!','\n',0x00};
	printf(a);
	printf("%d,%d\n", ii1, ii2);
}

int main()
{
	hello(1,1);
	return 0;
}
