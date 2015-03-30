#include "my_lock_op.h"

HANDLE my_lock_op::create_lock( char *cp_event_name )
{
	HANDLE h_lock = CreateEvent( 
		NULL,         // default security attributes
		FALSE,         // auto-reset event
		TRUE,         // initial state is signaled
		cp_event_name  // object name
		);
	if(h_lock == NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
	}

	return h_lock;
}

bool my_lock_op::get_lock( HANDLE& h_lock )
{
	bool b_rtn = false;

	DWORD dwWaitResult;
	HANDLE hEvents[1]; 

	hEvents[0] = h_lock;  // thread's read event

	dwWaitResult = WaitForMultipleObjects( 
		1,            // number of handles in array
		hEvents,      // array of event handles
		TRUE,         // wait till all are signaled
		INFINITE);    // indefinite wait

	switch (dwWaitResult) 
	{
		// Both event objects were signaled.
	case WAIT_OBJECT_0: 
		b_rtn = true;
		break; 

		// An error occurred.
	default: 
		printf("Wait error: %d\n", GetLastError()); 
	}

	return b_rtn;
}

bool my_lock_op::release_lock( HANDLE& h_lock )
{
	bool b_rtn = false;
	HANDLE hEvents[1]; 

	hEvents[0] = h_lock;  // thread's read event

	// Set the read event to signaled.

	if (! SetEvent(hEvents[0]) ) 
	{ 
		printf("SetEvent failed (%d)\n", GetLastError());
	}
	else
	{
		b_rtn = true;
	}

	return b_rtn;
}
