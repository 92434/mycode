// NeOperate.h: interface for the CNeOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEOPERATE_H__BB8AA316_9188_47BE_A58A_0FE87A666032__INCLUDED_)
#define AFX_NEOPERATE_H__BB8AA316_9188_47BE_A58A_0FE87A666032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcpClient.h"
#include "HfcpDefine.h"
#include "..\YinEvent.h"

#define WM_MSG_UPLOAD_FILE	 (WM_USER+101)

#include <fstream>
using namespace std;


class CNeOperate  
{
public:
	CNeOperate();
	virtual ~CNeOperate();

	//网元命令操作
	UINT Connect(string strNeIP, UINT iPort = 1400, ULONG ulNeID = 0);
	void Disconnect();
	UINT SendCmd(string strCmd, string& strRsp, UINT uiTimeout = 30000, BOOL bEvent = TRUE);

	//网元文件操作
	void UploadFile(string strNEFileAllName, string strLocalFileAllName);
	void DownloadFile(string strLocalFileAllName, string strNEFileAllName);

	void SetNeID( UINT ulNeID ){ m_obTcpClient.SetNeID(ulNeID); }
	
	BOOL IsConn(){ return m_obTcpClient.NeIsConn();}
	BOOL IsLogin(){ return m_bIsLogin; }
	
	void GetHostIP( vector<string>& vStrIP );
public:
	void CNeOperate::CreateRspThread();
	static unsigned __stdcall FunRspThread( void* _this );
	void CloseRsp();

	BOOL m_bRspOpen;
	HANDLE m_hRspThread;

	HANDLE m_hEvent;
	BOOL  m_bIsEvent;

	string m_strRsp;
	BOOL   m_bIsRead;

	BOOL m_bIsLogin;
private:
	CYinEvent	m_EventUpload;
	string	m_strNeIP;
	UINT    m_uiPort;
	CTcpClient	m_obTcpClient;

	//HFCP处理
	int  AnalyseHFCP(char* pMsg, int iSize);
	UINT DoSingleUploadStartRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	UINT DoSingleUploadDataRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	UINT DoSingleUploadEndRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	bool CheckPacket(HFCP_PROT_HEADER& obHeader,char* pszBody);

	UINT DoSingleDownloadStartRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	UINT DoSingleDownloadDataRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	UINT DoSingleDownloadEndRsp(HFCP_PROT_HEADER obHeader, char* pBody);
	int  SendSameDownloadPack();

	CHfcpDataCfg m_obHfcpDataCfg;
	ofstream m_osFile;
	ifstream m_isFile;

};

#endif // !defined(AFX_NEOPERATE_H__BB8AA316_9188_47BE_A58A_0FE87A666032__INCLUDED_)
