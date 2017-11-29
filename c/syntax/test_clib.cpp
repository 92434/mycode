#include "stdio.h"
#include "stdlib.h"
int main()
{
	int a = 0, b = 0;
	printf("Please input two integer:\n");
	//scanf("%d:%n", &a, &b);     //a对应的输入将被忽略，
	//printf("a = %d, b = %d\n", a, b);
	char s[100] = {0};
	scanf("%[a-z]", s);
	if(*s) printf("%s\n", s);
	printf("**************************************************\n");
	int c = 3, d = 8;
	printf("%*d\n", c, d);                     //输入等价于printf("%3d",d);
	printf("%*d\n", d, c);                     //空(d-1)格，从d格开始输出，即宽度为d。
	getchar();
	
	char buffer[2];
	int ret = snprintf(buffer, 2, "%d", 123456789);
	printf("%s\n", buffer);
	return 0;
}
