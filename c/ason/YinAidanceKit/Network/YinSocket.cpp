// YinSocket.cpp: implementation of the CYinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YinSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYinSocket::CYinSocket()
{
	//StartupWSA();
}

CYinSocket::~CYinSocket()
{
	//StopWSA();
}

UINT CYinSocket::StartupWSA()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int iErr;

	wVersionRequested = MAKEWORD( 2, 2 );

	iErr = WSAStartup( wVersionRequested, &wsaData );
	if ( iErr != 0 ) 
	{
		return iErr;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return SOCKET_ERROR; 
	}

	iErr = WSAGetLastError();
	return iErr;	
}

UINT CYinSocket::StopWSA()
{
	WSACleanup();
	UINT uErr = WSAGetLastError();
	return uErr;
}



UINT CYinSocket::CreateSocket(UINT uSockType/* = SOCK_STREAM*/, UINT uProtoType/* = IPPROTO_TCP*/)
{
	//inet_addr(pAddr)
	//type 为SOCK_STREAM = TCP， SOCK_DGRAM = UDP
	m_Socket = socket(AF_INET, uSockType, uProtoType);
//	WSAPROTOCOL_INFO obProtoInfo;
//	memset(&obProtoInfo, 0x00, sizeof(WSAPROTOCOL_INFO) );
//	m_Socket = WSASocket( AF_INET, SOCK_STREAM, NULL, NULL, NULL, WSA_FLAG_MULTIPOINT_D_LEAF	);
	UINT uErr = WSAGetLastError();
	return uErr;
}


UINT CYinSocket::Bind(UINT uPort, const char* pIP)
{
	m_sAddr.sin_family = AF_INET;
	m_sAddr.sin_port =  ntohs(uPort);
	m_sAddr.sin_addr.s_addr = inet_addr(pIP);//10.167.232.197
	//sAddr.sin_zero
	bind(m_Socket, (sockaddr*)&m_sAddr, sizeof(sockaddr));//m_sAddr.7
	UINT uErr = WSAGetLastError();
	return uErr;
}

UINT CYinSocket::Listen()
{ 
	listen(m_Socket, SOMAXCONN);
	UINT uErr = WSAGetLastError();
	return uErr;
}

UINT CYinSocket::Accept()
{
	int iAddr = sizeof(sockaddr);
	SOCKET obCltSocket = accept(m_Socket, (sockaddr*)&m_sAddr, &iAddr);
	UINT uErr = WSAGetLastError();
	if (uErr != 0 ) return uErr;

//	m_vCltSocker.push_back( obCltSocket );
	return obCltSocket;
}


UINT CYinSocket::Recv(SOCKET reSocker, string& strBuf)
{
	int iSize = strBuf.size();
	recv(reSocker, (char*)strBuf.c_str(), iSize, 0);

	UINT uErr = WSAGetLastError();
	return uErr;
}

UINT CYinSocket::Connect(const char* pSerIP, UINT uPort)
{
	m_sAddr.sin_family = AF_INET;
	m_sAddr.sin_port =  ntohs(uPort);
	m_sAddr.sin_addr.s_addr = inet_addr(pSerIP);
	int iStatus = connect(m_Socket, (sockaddr*)&m_sAddr, sizeof(sockaddr));
	if (iStatus == SOCKET_ERROR)
	{
		closesocket(m_Socket);
	}
	return iStatus;
}


UINT CYinSocket::GetHostName(string& strName)
{ 
	strName.resize(MAX_PATH, '\0');
	return gethostname( (char*)strName.data(), MAX_PATH ); 
}


UINT CYinSocket::GetHostIP( vector<string>& vHostIP )
{
	string strName;
	GetHostName(strName);
	HOSTENT* pHostEnt = gethostbyname(strName.c_str());
	UINT uErr = WSAGetLastError();
	if (uErr != 0 ) return uErr;
	
	for ( int i=0; i<pHostEnt->h_addrtype; i++)
	{
		string strLenAddr = pHostEnt->h_addr_list[i];
		u_long uInetAddr;
		memcpy(&uInetAddr, strLenAddr.c_str(), pHostEnt->h_length);
		in_addr inAddr;
		inAddr.s_addr = uInetAddr;
		string strIP = inet_ntoa(inAddr);
		vHostIP.push_back(strIP);
	}
	return uErr;
}



void CYinSocket::CreateTcpClientThread()
{
	unsigned threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &FunTcpClient, this, 0, &threadID );
	//等待线程运行
	Sleep(200);
}

unsigned CYinSocket::FunTcpClient( void* _this )
{
	CYinSocket* pYinSocket = (CYinSocket*)_this;
	pYinSocket->m_isOpen = TRUE;
	fd_set  fdR; 
	timeval timeout = {0, 10};

//	int iMode = 1; //0:阻塞   1:非阻塞
//	ioctlsocket(pYinSocket->m_Socket, FIONBIO, (u_long FAR*)&iMode);//非阻塞设置

	while (pYinSocket->m_isOpen == TRUE)
	{
		FD_ZERO(&fdR); 
		FD_SET(pYinSocket->m_Socket, &fdR); 
		int iErr = select(pYinSocket->m_Socket + 1, &fdR, NULL, NULL, &timeout);
		if (iErr == 0)  //0：等待超时，没有可读写或错误的文件
		{
			pYinSocket->OnIdly();
			continue;
		}

		static char buffer[1024] = "\0";
		if ( iErr > 0 && FD_ISSET(pYinSocket->m_Socket, &fdR) )	
		{
			int iSize = recv(pYinSocket->m_Socket, buffer, 1024, 0);

			if ( iSize > 0)
			{
				pYinSocket->OnCltRecve(buffer, iSize);
				pYinSocket->OnIdly();
			}
			else //负值：socket错误
			{
				pYinSocket->m_isOpen = FALSE;
				UINT uErr = WSAGetLastError();	
				if (WSAENOTSOCK == uErr)
				{
					uErr = WSAECONNABORTED;//客户端正常关闭
				}
				pYinSocket->OnErrorInfo(uErr);
				
			}
		}
		else   //负值：select错误
		{
			pYinSocket->m_isOpen = FALSE;
 			UINT uErr = WSAGetLastError();
 			pYinSocket->OnErrorInfo(uErr);
		}
	}
	closesocket(pYinSocket->m_Socket);
	//_endthreadex( 0 );
	return 0;
}


void CYinSocket::CreateUdpClientThread()
{
	unsigned threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &FunUdpClient, this, 0, &threadID );
	//等待线程运行
	Sleep(200);
}



// unsigned CYinSocket::FunUdpClient( void* _this )
// {
//  	CYinSocket* pYinSocket = (CYinSocket*)_this;
// 	int len = sizeof(pYinSocket->m_sAddrCli);
// 	
// 	fd_set  fdR;
// 	timeval timeout = {0, 20};
// 	//	int iMode = 1; //0:阻塞   1:非阻塞
// 	//	ioctlsocket(pYinSocket->m_Socket, FIONBIO, (u_long FAR*)&iMode);//非阻塞设置
// 	while (pYinSocket->m_isOpen == TRUE)
// 	{
// 	//	pYinSocket->m_sAddr.sin_addr.s_addr = inet_addr("10.167.232.115");//129.9.10.166
// 		FD_ZERO(&fdR); 
// 		FD_SET(pYinSocket->m_Socket, &fdR); 
// 		int iErr = select(pYinSocket->m_Socket + 1, &fdR, NULL, NULL, &timeout);
// 		if (iErr == 0)  //0：等待超时，没有可读写或错误的文件
// 		{
// 			pYinSocket->OnIdly();
// 			continue;
// 		}
// 		
// 		static char buffer[1024] = "\0";
// 		if ( iErr > 0 && FD_ISSET(pYinSocket->m_Socket, &fdR) )	
// 		{
// 			int iSize = recvfrom(pYinSocket->m_Socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&pYinSocket->m_sAddrCli, &len);
// 		
// 		//	char* ppf =	inet_ntoa( pYinSocket->m_sAddrCli.sin_addr );
// 				
// 			if ( iSize > 0)
// 			{
// 				pYinSocket->OnCltRecve(buffer, iSize);
// 				memset(&pYinSocket->m_sAddrCli, 0, sizeof(pYinSocket->m_sAddrCli));	 
// 			}
// 			else //负值：socket错误
// 			{
// 				pYinSocket->m_isOpen = FALSE;
// 				UINT uErr = WSAGetLastError();	
// 				if (WSAENOTSOCK == uErr)
// 				{
// 					uErr = WSAECONNABORTED;//客户端正常关闭
// 				}
// 				pYinSocket->OnErrorInfo(uErr);
// 				
// 			}
// 		}
// 		else   //负值：select错误   
// 		{
// 			pYinSocket->m_isOpen = FALSE;
// 			UINT uErr = WSAGetLastError();	
// 			pYinSocket->OnErrorInfo(uErr);
// 		}
// 	}
// 	closesocket(pYinSocket->m_Socket);
// 	_endthreadex( 0 );
// 	return 0;
// }



unsigned CYinSocket::FunUdpClient( void* _this )
{
 	CYinSocket* pYinSocket = (CYinSocket*)_this;
	pYinSocket->m_isOpen = TRUE;
	int len = sizeof(pYinSocket->m_sAddrCli);
	
	fd_set  fdR;
	timeval timeout = {0, 20};
	//	int iMode = 1; //0:阻塞   1:非阻塞
	//	ioctlsocket(pYinSocket->m_Socket, FIONBIO, (u_long FAR*)&iMode);//非阻塞设置
	while (pYinSocket->m_isOpen == TRUE)
	{
	//	pYinSocket->m_sAddr.sin_addr.s_addr = inet_addr("10.167.232.115");//129.9.10.166
		FD_ZERO(&fdR); 
		FD_SET(pYinSocket->m_Socket, &fdR); 
		int iErr = select(pYinSocket->m_Socket + 1, &fdR, NULL, NULL, &timeout);
		if (iErr == 0)  //0：等待超时，没有可读写或错误的文件
		{
			pYinSocket->OnIdly();
			continue;
		}
		
		static char buffer[1024] = "\0";
		if ( iErr > 0 && FD_ISSET(pYinSocket->m_Socket, &fdR) )	
		{
			int iSize = recvfrom(pYinSocket->m_Socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&pYinSocket->m_sAddrCli, &len);
		
		//	char* ppf =	inet_ntoa( pYinSocket->m_sAddrCli.sin_addr );
				
			if ( iSize > 0)
			{
				pYinSocket->OnCltRecve(buffer, iSize);
				memset(&pYinSocket->m_sAddrCli, 0, sizeof(pYinSocket->m_sAddrCli));	 
			}
			else //负值：socket错误
			{
				pYinSocket->m_isOpen = FALSE;
				UINT uErr = WSAGetLastError();	
				if (WSAENOTSOCK == uErr)
				{
					uErr = WSAECONNABORTED;//客户端正常关闭
				}
				pYinSocket->OnErrorInfo(uErr);
				
			}
		}
		else   //负值：select错误   
		{
			pYinSocket->m_isOpen = FALSE;
			UINT uErr = WSAGetLastError();	
			pYinSocket->OnErrorInfo(uErr);
		}
	}
	closesocket(pYinSocket->m_Socket);
	_endthreadex( 0 );
	return 0;
}

void CYinSocket::Close()
{
	if( m_isOpen == TRUE )
	{
		closesocket(m_Socket);
		m_isOpen = FALSE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle( m_hThread );
	}
}


UINT CYinSocket::Send(const char* strMsg, UINT iSize)
{
	return send(m_Socket, strMsg, iSize, 0);
}



UINT CYinSocket::SendTo(const char* pSerIP, UINT uPort, const char* pBuffer, int iSize)
{

// 	sockaddr_in m_sAddrd;
// 	m_sAddrd.sin_family = AF_INET;
// 	m_sAddrd.sin_port =  ntohs(1500);
// 	m_sAddrd.sin_addr.s_addr = inet_addr("10.167.232.115");//129.9.10.166
// //	int iStatus = connect(m_Socket, (sockaddr*)&m_sAddrd, sizeof(sockaddr));
// 	bind(m_Socket, (struct sockaddr*)&m_sAddrd, sizeof(sockaddr));


	int optval=1;
	setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST,(char FAR *)&optval,sizeof(optval));

	memset(&m_sAddr, 0x00, sizeof(m_sAddr));
	int len = sizeof(m_sAddr);
	m_sAddr.sin_family = AF_INET;
	m_sAddr.sin_port = htons(uPort);			///server的监听端口
	m_sAddr.sin_addr.s_addr = inet_addr(pSerIP);	///server的地址 
	int iErr = sendto(m_Socket, pBuffer, iSize, 0, (struct sockaddr*)&m_sAddr, len);


	return iErr;
}