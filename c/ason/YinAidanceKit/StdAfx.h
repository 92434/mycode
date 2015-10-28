// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0D846BA3_04C5_4C45_8AC8_84BE9EFAB172__INCLUDED_)
#define AFX_STDAFX_H__0D846BA3_04C5_4C45_8AC8_84BE9EFAB172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable: 4786) 

#include <afxmt.h>

#include <string>
#include <vector>
#include <queue>
#include <list>
using namespace std;
//#include ".\\PublicModule\\FileLog.h"
#include ".\\PublicModule\\AppConfig.h"

//extern CFileLog		g_obFileLog;
extern CAppConfig	g_obAppCfg;

void Split(string& str, string strChars, vector<string>& vStr);
BOOL Str2HexNeId(string strNeId, UINT& ulNeID);
UINT CalcCheckSum(const char* pszData,UINT uLen);

string TrimRight(const string& str, const string& strChar = " ");
string TrimLeft(const string& str, const string& strChar = " ");
string Trim(const string& str, const string& strChar = " ");

#pragma comment(lib, "Version.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0D846BA3_04C5_4C45_8AC8_84BE9EFAB172__INCLUDED_)
