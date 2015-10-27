// SevFrame.h: interface for the CSevFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEVFRAME_H__DFB36BDD_7533_438D_8D9F_E6F5FD6D42A8__INCLUDED_)
#define AFX_SEVFRAME_H__DFB36BDD_7533_438D_8D9F_E6F5FD6D42A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Network/TcpServer.h"
#include "./Network/NeOperate.h"
#include "./Network/UdpClient.h"

#ifndef _DEBUG
#include "./PythonEx/PythonEx.h"
#define  ObPythonEx  CPythonEx m_obPythonEx
#else
#define  ObPythonEx  
#endif

class CSevFrame  
{
public:
	CSevFrame();
	virtual ~CSevFrame();

	static CSevFrame* GetSrvFrame() { return _this; }

	void CreateSrv(UINT uPort);
	void CloseSrv();

	void Run();

	BOOL IsCltConn(){ return m_obTcpServer.m_bIsCltConn; }

	string LocalCmd(const char* pCmd);


	static unsigned __stdcall FunRspThread( void* _this );
	void CloseRsp();

private:
	//���������߳�
	void CreateRspThread();
	
	BOOL m_bRspOpen;
	HANDLE m_hRspThread;

public:
	//����˶���
	CTcpServer m_obTcpServer;
	//������Ԫ��������
	CNeOperate m_NeOperate;

	//����UDP����
	CUdpClient	m_obUdpClient;

	ObPythonEx;

private:
	static CSevFrame* _this;

};

#endif // !defined(AFX_SEVFRAME_H__DFB36BDD_7533_438D_8D9F_E6F5FD6D42A8__INCLUDED_)
