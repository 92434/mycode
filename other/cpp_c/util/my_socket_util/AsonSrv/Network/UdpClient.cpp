// UdpClient.cpp: implementation of the UdpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UdpClient.h"
#include "..\SevFrame.h"
#include "TcpServer.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUdpClient::CUdpClient()
{

}

CUdpClient::~CUdpClient()
{
	
}

void CUdpClient::CreateClient()
{
	CreateSocket(SOCK_DGRAM, IPPROTO_UDP);//IPPROTO_IP
	CreateUdpClientThread();
}

void CUdpClient::GetAllGipGroup(string strGIp, string& strIpGroup)
{
	Close();
	CreateClient();
	m_vStrIpGroup.clear();
	Bind(15000, strGIp.c_str() );
	Sleep(20);
	SendTo( "255.255.255.255", 1500, strNeSearchKey1, sizeof(strNeSearchKey1) );
	SendTo( "255.255.255.255", 1500, strNeSearchKey2, sizeof(strNeSearchKey2) );

	int iCount = 0;
	int iNextSize = 0;
	do
	{
		iNextSize = iCount;
		Sleep(100);
		iCount = m_vStrIpGroup.size();
	}while(iNextSize != iCount);

	int i;
	for (i=0; i<m_vStrIpGroup.size(); i++ )
	{
		strIpGroup = strIpGroup + m_vStrIpGroup[i] + "\n";
	}

	strIpGroup = TrimRight(strIpGroup, "\n");
	m_vStrIpGroup.clear();
	Close();
}


void CUdpClient::OnCltRecve(CHAR* pMsg, int iSize)
{
	CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
	
	if ( NULL != pSevFrame )
	{
		if ( memcmp(pMsg, strNeSearchKey1, sizeof(strNeSearchKey1)) && memcmp(pMsg, strNeSearchKey2, sizeof(strNeSearchKey2)) )
		{
			char* strIp = inet_ntoa( m_sAddrCli.sin_addr );
			m_vStrIpGroup.push_back(strIp);
		}
	}

// 	if ( NULL != m_pTcpSrv )
// 	{
//  		if ( memcmp(pMsg, strNeSearchKey1, sizeof(strNeSearchKey1)) && memcmp(pMsg, strNeSearchKey2, sizeof(strNeSearchKey2)) )
//  		{
// 			char* strIp = inet_ntoa( m_sAddrCli.sin_addr );
// 			m_pTcpSrv->Send(strIp, CMD_NE_IP_GROUP, strlen(strIp));
// 		}
// 	}
}


void CUdpClient::OnErrorInfo(UINT iErrID)
{
	switch (iErrID)
	{
	case 0:				//服务端正常关闭socket
		{
			int a = 0;
		}
		break;
	case WSAECONNABORTED://客户端 socket被关闭    --客户端自己退出
		{
			int a = 0;
		}
		break;
	case WSAENOTSOCK:	//服务端socket没开启
		{
			int a = 0;
		}
		break;
	case WSAECONNRESET:	//服务端socket强制关闭
		{
			int a = 0;
		}
		break;
	default:
		{
			int a = 0;
		}
		break;
	}
}


void CUdpClient::OnIdly()
{
	int a = 0;
}