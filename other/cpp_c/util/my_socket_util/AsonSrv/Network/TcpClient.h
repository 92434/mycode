// TcpClient.h: interface for the CTcpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPCLIENT_H__C6916946_39C9_4FE8_99EB_3AFA5FECCC72__INCLUDED_)
#define AFX_TCPCLIENT_H__C6916946_39C9_4FE8_99EB_3AFA5FECCC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "YinSocket.h"
#include "ComNeMsg.h"
#include "HfcpDataCfg.h"

#include "..\CriticalSection.h"
//#include <afxmt.h>

/*/////////////////////////////////////////////////////////////////////////
//  CTcpClient m_obTcpClient;
//	m_obTcpClient.CreateSocket();				//�����ͻ���socket
//	m_obTcpClient.Connect("127.0.0.1", 54321);	//���ӷ�����
//  m_obTcpClient.CreateClientThread();			//���������߳�
//	
//	m_obTcpClient.Close();						//�ر�����
//
/////////////////////////////////////////////////////////////////////////*/

//������Ҫ���⴦���TU�û�������
#define TU_LOGIN ":login:\"TU\",*\"password\"*"
#define TU_LOGNEW ":lognew:\"TU\",*\"password\"*"

#define OS_LOOP_NUM		4

class CTcpClient : public CYinSocket
{
public:
	CTcpClient();
	virtual ~CTcpClient();

	//MML------���ϳ�
	UINT NeSend(void* pData, USHORT usDataLen, USHORT usCmd = LANCMD_CMDLN);

	//HFCP-----���ϳ�
	int UploadSingleFile(CHfcpDataCfg& obHfcpDataCfg);
	int SendRequestData(CHfcpDataCfg& obHfcpDataCfg);
	int SendRequestEnd(CHfcpDataCfg& obHfcpDataCfg);

	int SendDownLoadFileBegin(CHfcpDataCfg& obHfcpDataCfg);
	int SendDownLoadData(CHfcpDataCfg& obHfcpDataCfg);
	int SendDownLoadEnd(CHfcpDataCfg& obHfcpDataCfg);
	
//	BOOL IsTuUserCmd(const char* pszCmd);
	UINT NeCreateConn(const char* pszIP, UINT iPort);
	void NeClose();

//	ULONG NeGetNeID(){ return m_ulNeID; }
	BOOL NeIsConn(){ return m_bIsConn; }

	void SetNeID(ULONG ulNeID ){ m_ulNeID = ulNeID; }
	ULONG GetNeID(){ return m_ulNeID; }


//	HANDLE NeGetMutes(){ return m_hMutex; }
//	void NeSetEvent(HANDLE hEvent){  m_hEvent = hEvent; }

	void WriteMsgLock(CComNeMsg* pNeMsg);
	void ReadMsgPopLock(CComNeMsg** ppNeMsg);
	
public:
	queue<CComNeMsg*> m_qComNeMsg;

private:
	BOOL m_bIsConn;
	ULONG m_ulNeID;
	UINT m_nOsLoopNum;

	//��Ϣ����
	queue<CComNeMsg*> m_qComRsp;
	CYinCriticalSection m_obCSection;

private:
	void ProcessConnRspPacket(CComNeMsg* pNEMsg);//����������Ϣ
	void ProcessNELoopRspPacket(CComNeMsg* pNeMsg); //����ػ���Ϣ
	void SendOsLoop();

protected:
	 virtual void OnCltRecve(CHAR* pMsg, int iSize);
	 virtual void OnErrorInfo(UINT iErrID);
	 virtual void OnIdly();

};

#endif // !defined(AFX_TCPCLIENT_H__C6916946_39C9_4FE8_99EB_3AFA5FECCC72__INCLUDED_)
