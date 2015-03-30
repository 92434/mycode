// TcpServer.h: interface for the CTcpServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSERVER_H__27A74FA7_681C_4EF5_973B_1990C1410022__INCLUDED_)
#define AFX_TCPSERVER_H__27A74FA7_681C_4EF5_973B_1990C1410022__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "YinSocket.h"
#include "..\CriticalSection.h"
#include "ComNeMsg.h"

class CTcpServer : public CYinSocket
{
public:
	CTcpServer();
	virtual ~CTcpServer();

	UINT CreateServer(UINT iPort, const char * pSvrIP);
	UINT AcceptAndResv();
	void Close();

	UINT Send(const char* strMsg, USHORT usCmdType, UINT iLen);

	void WritePack(CComNeMsg* pNeMsg);
	void ReadPack(CComNeMsg** ppNeMsg);
	void PopPack();

	BOOL m_bIsCltConn;	 //是否有客户端连接
private:
	CYinCriticalSection m_CsPack;
	CYinCriticalSection m_CsSend;
	queue<CComNeMsg*> m_qClientPack;



protected:
	SOCKET m_SvrSocket;

protected:
	virtual void OnCltRecve(CHAR* pMsg, int iSize);
	virtual void OnErrorInfo(UINT iErrID);
	virtual void OnIdly();

};

#endif // !defined(AFX_TCPSERVER_H__27A74FA7_681C_4EF5_973B_1990C1410022__INCLUDED_)
