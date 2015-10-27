// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0C5861B6_0C20_47F4_94CB_1DADB2819F4F__INCLUDED_)
#define AFX_STDAFX_H__0C5861B6_0C20_47F4_94CB_1DADB2819F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#include <Windows.h>
//È¥µô4089 /opt:noref
#pragma warning(disable: 4786) 
#include   <afx.h>

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <process.h>

#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

#include ".\\PublicModule\\FileLog.h"
extern CFileLog		g_obFileLog;

void Split(string& str, string strChars, vector<string>& vStr);
BOOL Str2HexNeId(string strNeId, UINT& ulNeID);
UINT CalcCheckSum(const char* pszData,UINT uLen);

string TrimRight(const string& str, const string& strChar = " ");
string TrimLeft(const string& str, const string& strChar = " ");
string Trim(const string& str, const string& strChar = " ");

//string function
string ToUpper(string str);
string ToLower(string str);

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0C5861B6_0C20_47F4_94CB_1DADB2819F4F__INCLUDED_)
