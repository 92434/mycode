#ifndef __MY_LOCK_OP_H_INCLUDE
#define __MY_LOCK_OP_H_INCLUDE
#include <stdio.h>
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")

class my_lock_op
{
public:
	static HANDLE create_lock( char *cp_event_name );
	static bool get_lock( HANDLE& h_lock );
	static bool release_lock( HANDLE& h_lock );
};
#endif //#ifndef __MY_LOCK_OP_H_INCLUDE
