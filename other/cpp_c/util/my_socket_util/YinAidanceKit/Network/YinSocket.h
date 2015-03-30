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
	//1.启动WSP环境
	static UINT StartupWSA();
	static UINT StopWSA();

	//2.创建Socket  SOCK_STREAM = TCP， SOCK_DGRAM = UDP
	UINT CreateSocket(UINT uSockType = SOCK_STREAM, UINT uProtoType = IPPROTO_TCP);

	//3.邦定
	UINT Bind(UINT uPort, const char* pIP);

	//4.监听
	UINT Listen();

	//5.接受
	UINT Accept();

	//6.接收
	UINT Recv(SOCKET reSocker, string& strBuf);

	//7.连接
	UINT Connect(const char* pSerIP, UINT uPort);

	//8.发送
	UINT Send(const char* strMsg, UINT iSize);

	//9.发送到
	UINT SendTo(const char* pSerIP, UINT uPort, const char* pBuffer, int iSize);


	//9.其他
	UINT GetHostName(string& strName);
	UINT GetHostIP( vector<string>& vHostIP );

	//9.TCP_Client - 线程接收
	void CreateTcpClientThread();
	static unsigned __stdcall FunTcpClient( void* _this );

	//10.UDP_Client - 线程接收
	void CreateUdpClientThread();
	static unsigned __stdcall FunUdpClient( void* _this );

	//11.关闭
	void Close();

	BOOL	m_isOpen;
protected:
	virtual void OnCltRecve(CHAR* pMsg, int iSize) = 0;
	virtual void OnErrorInfo(UINT iErrID) = 0;
	virtual void OnIdly(){};

protected:
	SOCKET m_Socket;
	sockaddr_in m_sAddr;   //服务端地址
	sockaddr_in m_sAddrCli;	// 客户端地址
private:

	HANDLE	m_hThread;
//	BOOL	m_isOpenSev;
//	HANDLE	m_hThreadSev;
	

};

#endif // !defined(AFX_YINSOCKET_H__4CAF90AC_397B_44D4_BB1F_0B66798B81F5__INCLUDED_)
