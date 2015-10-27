#include "stdio.h"

int main()
{
	char outs[]="hello word!\n";
	asm volatile(            //用asm volatile是害怕Gcc 自作主张的帮我给优化了。
			"jmp lab1 \n\t" 
			"lab0: \n\t"
			"popl %eax \n\t"
			"add $1,%eax \n\t"
			"jmp *%eax \n\t"
			"lab1: \n\t" 
			"call lab0 \n\t"
			"nop");
	asm volatile(
			"movl $0x0d,%%edx \n\t"  // 参数三：字符串长度
			"movl %0,%%ecx \n\t"  //参数二：要显示的字符串
			"movl $1,%%ebx \n\t"    //参数一：文件描述符(stdout) 
			"movl $4,%%eax \n\t"    //系统调用号(sys_write) 
			"int  $0x80 \n\t"       //调用内核功能
			//退出程序
			"movl $0,%%ebx \n\t"     //参数一：退出代码
			"movl $1,%%eax \n\t"     //系统调用号(sys_exit) 
			"int $0x80"         //调用内核功能
			:
			:"r"(outs)
			:"%eax","%ebx","%ecx","%edx");     
	return 0;
}
