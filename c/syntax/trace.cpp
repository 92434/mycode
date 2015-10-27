#include <iostream>
using namespace std;


#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
void my_trace()
{
	int size = 16;
	void * array[16];
	int stack_num = backtrace(array, size);
	char ** stacktrace = backtrace_symbols(array, stack_num);
	for (int i = 0; i < stack_num; ++i)
	{
		printf("%s\n", stacktrace[i]);
	}
	free(stacktrace);
	printf("%s\n%s\n", __PRETTY_FUNCTION__, __FUNCTION__);
}
void func1(){
	my_trace(); 
	printf("%s\n%s\n", __PRETTY_FUNCTION__, __FUNCTION__);
}

void func2(){
	func1();
	printf("%s\n%s\n", __PRETTY_FUNCTION__, __FUNCTION__);
}

int main(){
	func2();
	printf("%s\n%s\n", __PRETTY_FUNCTION__, __FUNCTION__);
}
