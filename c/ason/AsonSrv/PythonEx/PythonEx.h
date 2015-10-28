// PythonEx.h: interface for the CPythonEx class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_PYTHONEX_H__5C135EED_BD68_4412_8177_2520B25D4961__INCLUDED_)
#define AFX_PYTHONEX_H__5C135EED_BD68_4412_8177_2520B25D4961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef _DEBUG
#define  PYTHON_EX	1
#include <afxmt.h>
//#include "UIThread.h"
#include "..\YinEvent.h"

class CPythonEx
{
public:
	CPythonEx();
	virtual ~CPythonEx();

//	void SendMSG();

	UINT CreatePython();
	void ReleasePython();

	void SetArgs(string strArgs);
	UINT ExePyModule(const char* pModName);
	void ExePyModuleAsy(const char* pModName);

	UINT GetPyResult(char* pModName, string& strRelust);

	//异步执行PY脚本
 //	void CreateProcessThread();
 //	static unsigned __stdcall FunProcessThread( void* _this );
 //	void CloseProcess();
	
 	BOOL m_bThreadOpen;
 	HANDLE m_hThread;

	//消息互斥
 	queue<string> m_qStrModule;
 	CCriticalSection m_obCSection;

private:
//	CUIThread   m_obUiThread;

public:
	string m_strArgs;
	string m_strNeRelust;

	CYinEvent m_DialogEvent;
	string m_strFilePath;
};

#else
#define  PYTHON_EX	0
#endif 

#endif // !defined(AFX_PYTHONEX_H__5C135EED_BD68_4412_8177_2520B25D4961__INCLUDED_)


