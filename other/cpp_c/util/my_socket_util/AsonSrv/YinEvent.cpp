// YinEvent.cpp: implementation of the CYinEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YinEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYinEvent::CYinEvent()
{
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

DWORD CYinEvent::WaitEvent(UINT uTimeout)
{
	//WAIT_TIMEOUT
	return WaitForSingleObject(m_hEvent, uTimeout);
}

void CYinEvent::SetEvent()
{
	::SetEvent( m_hEvent );
}


CYinEvent::~CYinEvent()
{
	CloseHandle( m_hEvent );
}
