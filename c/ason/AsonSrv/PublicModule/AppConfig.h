// AppConfig.h: interface for the CAppConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPCONFIG_H__32B1AB47_663B_4D16_9AB4_2D2556E6D526__INCLUDED_)
#define AFX_APPCONFIG_H__32B1AB47_663B_4D16_9AB4_2D2556E6D526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  INI_FILE_NAME		"\\Config\\App.ini"

class CAppConfig  
{
public:
	CAppConfig();
	virtual ~CAppConfig();

	void WriteUserName(string strName);
	void WriteLocalIp(string strLocalIp);
	void WriteNeIp(string strNeIp);

	string GetUserName();
	string GetLocalIp();
	string GetNeIp();

private:
	string m_strIniPath;

};

#endif // !defined(AFX_APPCONFIG_H__32B1AB47_663B_4D16_9AB4_2D2556E6D526__INCLUDED_)
