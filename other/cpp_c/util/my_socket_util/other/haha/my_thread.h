#ifndef __MY_THREAD_H_INCLUDE
#define __MY_THREAD_H_INCLUDE
#include <stdio.h>
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")

class my_thread
{
public:
	static void create_thread(LPVOID lpStartAddress, LPVOID lp_para);
};
#endif //#ifndef __MY_THREAD_H_INCLUDE