// FileLog.cpp: implementation of the CFileLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//#include <direct.h> 
#include <shlwapi.h> 
#pragma comment(lib, "shlwapi.lib")  

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileLog::CFileLog()
{
	m_strLogPath.resize(MAX_PATH, 0x00);
	GetCurrentDirectory( MAX_PATH, (char*)m_strLogPath.c_str() );
	//cout<<m_strLogPath<<"\r\n";
	//::MessageBox(NULL, m_strLogPath.c_str(), "22", 0);
	//OutputDebugString(m_strLogPath.c_str());
	//mkdir(name); 
}

CFileLog::~CFileLog()
{
	if( m_oStr.is_open() )
	{
		m_oStr.close();
	}
}


BOOL CFileLog::WriteLog(const char* strBuf)
{
	m_cs.Lock();
	m_dwLength += strlen(strBuf);
	m_oStr<<strBuf;
    m_oStr.flush();
	if (m_dwLength > 1024000)
	{
		CreateLog();
	}
	m_cs.Unlock();
	return TRUE;
}

// BOOL CFileLog::IsFileOccur()
// {
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE hFind;
// 	hFind = FindFirstFile(_T("d:\\softist.txt"), &FindFileData);
// 	if (hFind == INVALID_HANDLE_VALUE) 
// 	{
// 		return FALSE;
// 		//AfxMessageBox(_T("文件不存在"));
// 	} 
// 	else 
// 	{
// 		//AfxMessageBox(_T("文件存在"));
// 		return TRUE;
// 		FindClose(hFind);
// 	}
// }


// 
// time_t FileTimeToTime_t(const FILETIME &ft)
// {
//     ULARGE_INTEGER ui;
//     ui.LowPart = ft.dwLowDateTime;
//     ui.HighPart = ft.dwHighDateTime;
//     return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
// }
// 
// 
// int GetDiffSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
// {
//     FILETIME fTime1 = { 0, 0 };
//     FILETIME fTime2 = { 0, 0 };
//     SystemTimeToFileTime(&t1, &fTime1);
//     SystemTimeToFileTime(&t2, &fTime2);
// 	
//     time_t tt1 = FileTimeToTime_t(fTime1);
//     time_t tt2 = FileTimeToTime_t(fTime2);
// 	
//     return (int)(tt2 - tt1);
// }
// 
// int GetDiffDays(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
// {
//     int diffSeconds = GetDiffSeconds(t1, t2);
//     return diffSeconds / (24 * 3600);
// }


BOOL CFileLog::CreateLog(/*LPCTSTR lpFileFullPath*//* = NULL*/)
{
	 if( m_oStr.is_open() )
	 {
		 m_oStr.close();
	 }

	string strLogPath = m_strLogPath.c_str();
	strLogPath = strLogPath + "\\Log";
	//1.判断Log 和 Info 文件夹是否存在
	if(! PathIsDirectory( strLogPath.c_str() ) )
	{
		::CreateDirectory(strLogPath.c_str(), NULL);
	}
	strLogPath = strLogPath + "\\Info";
	if(! PathIsDirectory( strLogPath.c_str() ) )
	{
		::CreateDirectory(strLogPath.c_str(), NULL);
	}

	//2.删除10天前得日志
// 	WIN32_FIND_DATA FileData; 
// 	HANDLE hSearch; 
// 	BOOL fFinished = TRUE; 
// 	string strLogPathFilde = strLogPath + "\\*.txt";
// 	cout<<strLogPathFilde.c_str()<<"\r\n";
// 	hSearch = FindFirstFile(strLogPathFilde.c_str() , &FileData);
// 
// 	if( hSearch == INVALID_HANDLE_VALUE )
// 	{
// 		fFinished = FALSE;
// 	}
// 
// 	vector<string> vStrPath;
// 	while(fFinished)
// 	{
// 		SYSTEMTIME stUTC, stCreate;
// 		FileTimeToSystemTime(&FileData.ftCreationTime, &stUTC);
// 		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stCreate);
// 
// 		SYSTEMTIME   NowTime; 
// 		GetLocalTime(&NowTime); 
// 		int iDay = GetDiffDays(NowTime, stCreate);
// 		cout<<iDay<<"\r\n";
// 		if (iDay > 10)
// 		{
// 			cout<<FileData.cFileName<<"\r\n";
// 			vStrPath.push_back(FileData.cFileName);
// 		}
// 
// 		fFinished = FindNextFile(hSearch, &FileData);
// 	}
// 
// 	for (int i=0; i<vStrPath.size(); i++)
// 	{
// 		string strTmpPath = strLogPath + "\\" + vStrPath[i];
// 		cout<<"g="<<strTmpPath.c_str()<<"\r\n";
// 		DeleteFile(strTmpPath.c_str());
// 	}
// 
// 	FindClose(hSearch);

	//2.创建打开文件
	m_dwLength = 0;

	string strNewFileName;
	strNewFileName.resize(28, 0x00);
	SYSTEMTIME   NowTime; 
	GetLocalTime(&NowTime);
	sprintf((char *)strNewFileName.c_str(), "%04d-%02d-%02d %02d%02d%02d%03d.txt", NowTime.wYear, NowTime.wMonth, NowTime.wDay, NowTime.wHour, NowTime.wMinute, NowTime.wSecond, NowTime.wMilliseconds);
	//cout<<strNewFileName.c_str()<<"\r\n";

	string strAllPath = strLogPath + "\\" + strNewFileName;
	m_oStr.open(strAllPath.c_str(), ios_base::app );
    if( ! m_oStr.is_open() ) return FALSE;
    return TRUE;
}


