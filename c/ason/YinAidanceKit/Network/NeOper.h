// NeOper.h: interface for the CNeOper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEOPER_H__47B495ED_9436_4B56_97A6_D2529B40AD40__INCLUDED_)
#define AFX_NEOPER_H__47B495ED_9436_4B56_97A6_D2529B40AD40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NeClient.h"
#include "..\YinEvent.h"

#define  MSG_MENU_ENABLE      (WM_USER + 303)


class CNeOper  
{
public:
	CNeOper();
	virtual ~CNeOper();

	BOOL CreateAndConnSrv(const char* pStrIP);

	BOOL IsConnSrv(){ return m_obNeClient.m_bIsConnSrv; }	
	void ExitSrv();

	//Py命令操作
	void PyExeModule(const char* pModName);
	UINT PyGetResult(char* pModName, string& strRelust, UINT uTimeout = 15000);
	void PySetArgs(string strArgs);
	

	//网元命令操作
	UINT Connect(string strNeIP, UINT iPort = 1400, ULONG ulNeID = 0);
	void Disconnect(UINT uTimeout = 10000);
	UINT SendCmd(string strCmd, string& strRsp, UINT uiTimeout = 20000);

	//界面操作
	void SendHeart();
	
	//网元文件操作
	//void UploadFile(string strNEFileAllName, string strLocalFileAllName);
	//void DownloadFile(string strLocalFileAllName, string strNEFileAllName);
	
	void SetNeID( UINT ulNeID );
	
	BOOL IsNeConn(){ return m_bIsNeConn; }
	BOOL IsNeLogin(){ return m_bIsNeLogin; }
	BOOL IsSrvConn(){ return m_obNeClient.m_bIsConnSrv; }
	

	UINT GetHostIP( vector<string>& vStrIP );
	void SendAllGip(string strGip, vector<string>& vStrGwAllIp);



	void CreateRspThread();
	static unsigned __stdcall FunRspThread( void* _this );
	void CloseRsp();
	
	BOOL m_bRspOpen;
	HANDLE m_hRspThread;

public:
	BOOL m_bIsNeConn;
	BOOL m_bIsNeLogin;

public:
	CYinEvent	m_EventGetIP;
	string		m_strIpGroup;

	CYinEvent	m_EventSendCmd;
	string		m_strResult;

	CYinEvent	m_EventDesconn;

	CYinEvent	m_EventPyResult;
	string		m_strPyResult;

	CYinEvent	m_EventGwAllIp;
	vector<string>	m_vStrGwAllIp;

private:
	CNeClient	m_obNeClient;
};

#endif // !defined(AFX_NEOPER_H__47B495ED_9436_4B56_97A6_D2529B40AD40__INCLUDED_)
