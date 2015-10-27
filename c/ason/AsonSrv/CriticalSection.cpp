// CriticalSection.cpp: implementation of the CCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CriticalSection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYinCriticalSection::CYinCriticalSection()
{
	InitializeCriticalSection(&m_cs); 

}

CYinCriticalSection::~CYinCriticalSection()
{
	DeleteCriticalSection(&m_cs);
}


void CYinCriticalSection::Lock()
{
	EnterCriticalSection(&m_cs); 
}

void CYinCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_cs); 
}