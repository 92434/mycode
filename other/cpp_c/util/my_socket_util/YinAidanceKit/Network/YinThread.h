// YinThread.h: interface for the CYinThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINTHREAD_H__C5DF44A7_3FEB_4C1C_8764_E584AA6645C5__INCLUDED_)
#define AFX_YINTHREAD_H__C5DF44A7_3FEB_4C1C_8764_E584AA6645C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include <process.h>
#endif // _MSC_VER > 1000

typedef UINT (*FunName)(void* _this);

class CYinThread  
{
public:
	CYinThread();
	virtual ~CYinThread();

	void Create(FunName RunFun, void* Para);
	void Close();

private:
	static unsigned __stdcall Fun( void* _this );

private:
	BOOL m_isOpen;
	HANDLE m_hThread;
	FunName m_Fun;
	void* m_Para;

};

#endif // !defined(AFX_YINTHREAD_H__C5DF44A7_3FEB_4C1C_8764_E584AA6645C5__INCLUDED_)
