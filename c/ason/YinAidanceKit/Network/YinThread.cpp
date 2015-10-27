// YinThread.cpp: implementation of the CYinThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YinThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYinThread::CYinThread()
{
	m_isOpen = FALSE;
}

CYinThread::~CYinThread()
{
	Close();
}

void CYinThread::Create(FunName RunFun, void* Para)
{
	m_Fun = RunFun;
	m_Para = Para;
	unsigned threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &Fun, this, 0, &threadID );
	Sleep(200);
}


unsigned CYinThread::Fun( void* _this )
{
	CYinThread* pYinThread = (CYinThread*)_this;
	pYinThread->m_isOpen = TRUE;
	while(pYinThread->m_isOpen == TRUE)
	{
		pYinThread->m_Fun(pYinThread->m_Para);
	}
	return 0;
}


void CYinThread::Close()
{
	if ( TRUE == m_isOpen )
	{
		m_isOpen = FALSE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle( m_hThread );
		m_Fun = NULL;
		m_Para = NULL;
	}
}