// TcpServer.cpp: implementation of the CTcpServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TcpServer.h"
#include "..\FaceSvrHead.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpServer::CTcpServer()
{
	m_bIsCltConn = FALSE;
}

CTcpServer::~CTcpServer()
{

}



void CTcpServer::OnCltRecve(CHAR* pMsg, int iSize)
{
// 	if (iSize < 4)
// 	{
// 		return;
// 	}
// 	PFACE_SVR_HEADER pPackHeader = (PFACE_SVR_HEADER)pMsg;
// 
// 	if (pPackHeader->szHeader[0] != PACK_KEY_WORD[0] || pPackHeader->szHeader[1] != PACK_KEY_WORD[1] ||
// 		pPackHeader->szHeader[2] != PACK_KEY_WORD[2] || pPackHeader->szHeader[3] != PACK_KEY_WORD[3] ||
// 		pPackHeader->uBufLen + sizeof(FACE_SVR_HEADER) >  iSize)
// 	{
// 		return;
// 	}
// 
// 	CComNeMsg* pComNeMsg = new CComNeMsg();
// 	pComNeMsg->CreateBuf(pPackHeader->uBufLen);
// 	memcpy(&pComNeMsg->m_obFaceHeader, pPackHeader, sizeof(FACE_SVR_HEADER));
// 	pComNeMsg->AddMsg(pMsg+sizeof(FACE_SVR_HEADER), pPackHeader->uBufLen);
// 	WritePack(pComNeMsg);

	//2.MML包处理
	static BOOL bIsHead = FALSE;
	static BOOL bIsbody = FALSE;
	int iSign = 0;
	
	static int iPos = 0;
	
	static char cBuf[1240];
	memcpy(cBuf + iPos, pMsg, iSize);
	int iCount = iPos + iSize;
	
	static CComNeMsg* pComNeMsg;
	while (TRUE)
	{
		if (bIsHead == FALSE && iCount - iSign < sizeof(FACE_SVR_HEADER))
		{
			iPos = iCount - iSign;
			char cTmp[100];
			memcpy( cTmp, cBuf + iSign, iPos );
			memcpy( cBuf, cTmp, iPos );
			return;
		}
		
		if (bIsHead == FALSE)
		{
			pComNeMsg = new CComNeMsg();
			//pComNeMsg->EmptyMsg();
			memcpy( &pComNeMsg->m_obFaceHeader, cBuf + iSign,  sizeof(FACE_SVR_HEADER) );
			iSign += sizeof(FACE_SVR_HEADER);
			pComNeMsg->m_obFaceHeader.ToHostOrder();
			bIsHead = TRUE;
		}
		
		if ( bIsHead == TRUE)
		{
			if (bIsbody == FALSE)
			{	
				pComNeMsg->CreateBuf(pComNeMsg->m_obFaceHeader.uBufLen);
				bIsbody = TRUE;
			}
			int iSurplus = pComNeMsg->GetMsgLen() - pComNeMsg->GetTrulyLen();
			if( iCount - iSign >= iSurplus)
			{
				
				pComNeMsg->AddMsg(cBuf + iSign, iSurplus);
				iSign += iSurplus;
				bIsHead = FALSE;
				bIsbody = FALSE;
				WritePack(pComNeMsg);
				//OutputDebugString(pComNeMsg->GetMsg());
				//OutputDebugString("\r\n----\r\n");
				//m_qComNeMsg.push(pComNeMsg);
				cout<<"CmdType="<<pComNeMsg->m_obFaceHeader.usCmdType<<"\r\n";
				continue;
			}
			else
			{
				pComNeMsg->AddMsg(cBuf + iSign, iCount - iSign);
				iSign += iCount - iSign;
				iPos = 0;
				return;
			}	
		}
	}
}


void CTcpServer::OnErrorInfo(UINT iErrID)
{
	switch (iErrID)
	{
		case 0:				//服务端正常关闭socket
			{
				m_bIsCltConn = FALSE;	
			}
			break;
		case WSAECONNABORTED://客户端 socket被关闭    --客户端自己退出
			{
				m_bIsCltConn = FALSE;
			}
			break;
		case WSAENOTSOCK:	//服务端socket没开启
			{
				m_bIsCltConn = FALSE;
			}
			break;
		case WSAECONNRESET:	//服务端socket强制关闭
			{
				m_bIsCltConn = FALSE;
			}
			break;
		default:
			{
				int a = 0;
			}
		break;
	}
}

void CTcpServer::OnIdly()
{
	//是否断开了与网元的连接
	if (m_bIsCltConn)
	{
// 		BOOL bNeIsConn = m_pSrvFrame->m_NeOperate.IsConn();
// 		Send((char*)&bNeIsConn, CMD_NE_IS_CONN, sizeof(BOOL));
// 		Sleep(150);
// 	
// 		BOOL bNeIsLogin = m_pSrvFrame->m_NeOperate.IsLogin()
// 		Send((char*)&bNeIsLogin, CMD_NE_IS_LOGIN, sizeof(BOOL));	
	}
	
}



UINT CTcpServer::CreateServer(UINT iPort, const char * pSvrIP)
{
	m_bIsCltConn = FALSE;
	CreateSocket();
	Bind(iPort, pSvrIP);
	Listen();
	m_SvrSocket = m_Socket;
	return 0;
}


UINT CTcpServer::AcceptAndResv()
{
	SOCKET iCltSocket = Accept();
	if (iCltSocket == 0) return 0;

	closesocket(m_Socket);
	m_Socket = iCltSocket;
	
	CreateTcpClientThread();	
	m_bIsCltConn = TRUE;

	return 1;
}


UINT CTcpServer::Send(const char* strMsg, USHORT usCmdType, UINT iLen)
{
	m_CsSend.Lock();
	UINT uPackSize = iLen + sizeof(FACE_SVR_HEADER);
	CHAR* pBuf = new CHAR[uPackSize];
	PFACE_SVR_HEADER pFSHeader= (PFACE_SVR_HEADER)pBuf;
	memcpy(pFSHeader->szHeader, PACK_KEY_WORD, strlen(PACK_KEY_WORD));
	pFSHeader->usCmdType = usCmdType;
	pFSHeader->uBufLen = iLen;
	pFSHeader->uCheckSum = CalcCheckSum(strMsg, iLen);
	pFSHeader->ToNetOrder();
	memcpy(pBuf + sizeof(FACE_SVR_HEADER), strMsg, iLen);

	UINT uErr = CYinSocket::Send(pBuf, uPackSize);

	delete[] pBuf;
	m_CsSend.Unlock();
	//返回发送的长度
	return uErr;
}


void CTcpServer::Close()
{
	CYinSocket::Close();
	closesocket(m_SvrSocket);
}


void CTcpServer::WritePack(CComNeMsg* pNeMsg)
{
	m_CsPack.Lock();
	m_qClientPack.push(pNeMsg);
	m_CsPack.Unlock();
}


void CTcpServer::ReadPack(CComNeMsg** ppNeMsg)
{
	m_CsPack.Lock();
	if (true != m_qClientPack.empty())
	{
		CComNeMsg* pComNeMsg = m_qClientPack.front();
		*ppNeMsg = pComNeMsg;
	}
	else
	{
		*ppNeMsg = NULL;
	}
	m_CsPack.Unlock();
}

void CTcpServer::PopPack()
{
	m_CsPack.Lock();
	if (true != m_qClientPack.empty())
	{
		m_qClientPack.pop();
	}
	m_CsPack.Unlock();
}



