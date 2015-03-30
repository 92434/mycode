#include "my_thread.h"

void my_thread::create_thread( LPVOID lpStartAddress, LPVOID lp_para )
{
	DWORD d_thread_id;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lp_para, 0, &d_thread_id);
}