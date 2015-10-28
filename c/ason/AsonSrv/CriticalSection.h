// CriticalSection.h: interface for the CCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSECTION_H__9F163D50_D5DD_442B_9C17_FAD7410D256C__INCLUDED_)
#define AFX_CRITICALSECTION_H__9F163D50_D5DD_442B_9C17_FAD7410D256C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYinCriticalSection  
{
public:
	CYinCriticalSection();
	virtual ~CYinCriticalSection();

	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_cs;
};

#endif // !defined(AFX_CRITICALSECTION_H__9F163D50_D5DD_442B_9C17_FAD7410D256C__INCLUDED_)
