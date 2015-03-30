// AppConfig.cpp: implementation of the CAppConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppConfig::CAppConfig()
{
	m_strIniPath.resize(MAX_PATH, 0x00);
	GetCurrentDirectory( MAX_PATH, (char*)m_strIniPath.c_str() );
	m_strIniPath = m_strIniPath.c_str() + string(INI_FILE_NAME);
}

CAppConfig::~CAppConfig()
{

}

void CAppConfig::WriteUserName(string strName)
{
	::WritePrivateProfileString( "USER_INI", "UserName", strName.c_str(), m_strIniPath.c_str());   
}

void CAppConfig::WritePwd(string strPwd)
{
	::WritePrivateProfileString( "USER_INI", "PassWord", strPwd.c_str(), m_strIniPath.c_str());   
}

void CAppConfig::WriteLocalIp(string strLocalIp)
{
	::WritePrivateProfileString( "USER_INI", "LocalIp", strLocalIp.c_str(), m_strIniPath.c_str());   
}

void CAppConfig::WriteNeIp(string strNeIp)
{
	::WritePrivateProfileString( "USER_INI", "NeIp", strNeIp.c_str(), m_strIniPath.c_str());   
}

string CAppConfig::GetUserName()
{
	string strName;
	strName.resize(MAX_PATH, 0x00);
	::GetPrivateProfileString( "USER_INI ", "UserName", "szhw", (char*)strName.c_str(), MAX_PATH, m_strIniPath.c_str() );   
	return strName.c_str();
}

string CAppConfig::GetPwd()
{
	string strPwd;
	strPwd.resize(MAX_PATH, 0x00);
	::GetPrivateProfileString( "USER_INI ", "PassWord", "", (char*)strPwd.c_str(), MAX_PATH, m_strIniPath.c_str() );   
	return strPwd.c_str();
}


string CAppConfig::GetLocalIp()
{
	string strLocalIp;
	strLocalIp.resize(MAX_PATH, 0x00);
	::GetPrivateProfileString( "USER_INI ", "LocalIp", "", (char*)strLocalIp.c_str(), MAX_PATH, m_strIniPath.c_str() );   
	return strLocalIp.c_str();
}

string CAppConfig::GetNeIp()
{
	string strNeIp;
	strNeIp.resize(MAX_PATH, 0x00);
	::GetPrivateProfileString( "USER_INI ", "NeIp", "", (char*)strNeIp.c_str(), MAX_PATH, m_strIniPath.c_str() );   
	return strNeIp.c_str();
}

UINT CAppConfig::GetShowConsole()
{  
	return ::GetPrivateProfileInt( "OTHER ", "ShowConsole", 0, m_strIniPath.c_str() ); 
}

void CAppConfig::WriteCfgInfo(string strSection, string strKeyword, string strValue)
{
	::WritePrivateProfileString( strSection.c_str(), strKeyword.c_str(), strValue.c_str(), m_strIniPath.c_str() );   
}


string CAppConfig::GetCfgInfo(string strSection, string strKeyword)
{
	string strValue;
	strValue.resize(MAX_PATH, 0x00);
	::GetPrivateProfileString( strSection.c_str(), strKeyword.c_str(), "", (char*)strValue.c_str(), MAX_PATH, m_strIniPath.c_str() );   
	return strValue.c_str();
}

