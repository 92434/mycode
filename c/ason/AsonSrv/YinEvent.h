// YinEvent.h: interface for the CYinEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINEVENT_H__B7D79EBC_EE7B_42B6_AC89_378C0238F7E4__INCLUDED_)
#define AFX_YINEVENT_H__B7D79EBC_EE7B_42B6_AC89_378C0238F7E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYinEvent  
{
public:
	CYinEvent();
	virtual ~CYinEvent();
	DWORD WaitEvent(UINT uTimeout = INFINITE);
	void SetEvent();

private:
	HANDLE m_hEvent;
};

#endif // !defined(AFX_YINEVENT_H__B7D79EBC_EE7B_42B6_AC89_378C0238F7E4__INCLUDED_)
