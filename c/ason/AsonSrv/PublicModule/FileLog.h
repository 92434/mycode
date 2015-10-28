// FileLog.h: interface for the CFileLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELOG_H__40990119_94D2_44C7_8849_DE173AB1325D__INCLUDED_)
#define AFX_FILELOG_H__40990119_94D2_44C7_8849_DE173AB1325D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "..\CriticalSection.h"

class CFileLog  
{
public:
	CFileLog();
	virtual ~CFileLog();
	BOOL WriteLog(const char* strBuf);
	BOOL CreateLog(/*LPCTSTR lpFileFullPath*//* = NULL*/);

private:
	BOOL IsFileOccur();

private:
	DWORD  m_dwLength;
	string m_strLogPath;
	ofstream m_oStr;

	CYinCriticalSection m_cs;
};

#endif // !defined(AFX_FILELOG_H__40990119_94D2_44C7_8849_DE173AB1325D__INCLUDED_)
