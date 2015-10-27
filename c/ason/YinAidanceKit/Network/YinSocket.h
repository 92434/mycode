// YinSocket.h: interface for the CYinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINSOCKET_H__4CAF90AC_397B_44D4_BB1F_0B66798B81F5__INCLUDED_)
#define AFX_YINSOCKET_H__4CAF90AC_397B_44D4_BB1F_0B66798B81F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <process.h>

#include <string>
#include <vector>
#include <queue>
using namespace std;


class CYinSocket  
{
public:
	CYinSocket();
	virtual ~CYinSocket();
	//1.����WSP����
	static UINT StartupWSA();
	static UINT StopWSA();

	//2.����Socket  SOCK_STREAM = TCP�� SOCK_DGRAM = UDP
	UINT CreateSocket(UINT uSockType = SOCK_STREAM, UINT uProtoType = IPPROTO_TCP);

	//3.�
	UINT Bind(UINT uPort, const char* pIP);

	//4.����
	UINT Listen();

	//5.����
	UINT Accept();

	//6.����
	UINT Recv(SOCKET reSocker, string& strBuf);

	//7.����
	UINT Connect(const char* pSerIP, UINT uPort);

	//8.����
	UINT Send(const char* strMsg, UINT iSize);

	//9.���͵�
	UINT SendTo(const char* pSerIP, UINT uPort, const char* pBuffer, int iSize);


	//9.����
	UINT GetHostName(string& strName);
	UINT GetHostIP( vector<string>& vHostIP );

	//9.TCP_Client - �߳̽���
	void CreateTcpClientThread();
	static unsigned __stdcall FunTcpClient( void* _this );

	//10.UDP_Client - �߳̽���
	void CreateUdpClientThread();
	static unsigned __stdcall FunUdpClient( void* _this );

	//11.�ر�
	void Close();

	BOOL	m_isOpen;
protected:
	virtual void OnCltRecve(CHAR* pMsg, int iSize) = 0;
	virtual void OnErrorInfo(UINT iErrID) = 0;
	virtual void OnIdly(){};

protected:
	SOCKET m_Socket;
	sockaddr_in m_sAddr;   //����˵�ַ
	sockaddr_in m_sAddrCli;	// �ͻ��˵�ַ
private:

	HANDLE	m_hThread;
//	BOOL	m_isOpenSev;
//	HANDLE	m_hThreadSev;
	

};

#endif // !defined(AFX_YINSOCKET_H__4CAF90AC_397B_44D4_BB1F_0B66798B81F5__INCLUDED_)
