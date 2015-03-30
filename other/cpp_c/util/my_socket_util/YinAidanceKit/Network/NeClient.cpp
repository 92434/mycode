// NeClient.cpp: implementation of the CNeClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\MainFrm.h"
#include "NeClient.h"

#include "../../AsonSrv/FaceSvrHead.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeClient::CNeClient()
{
	m_bIsConnSrv = FALSE;
}

CNeClient::~CNeClient()
{
	Close();
}



// void CNeClient::OnCltRecve(CHAR* pMsg, int iSize)
// {
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
// }

void CNeClient::OnCltRecve(CHAR* pMsg, int iSize)
{
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


void CNeClient::OnErrorInfo(UINT iErrID)
{

	switch (iErrID)
	{
		case 0:				//服务端正常关闭socket
			{
				m_bIsConnSrv = FALSE;	
			}
			break;
		case WSAECONNABORTED://客户端 socket被关闭    --客户端自己退出
			{
				m_bIsConnSrv = FALSE;
			}
			break;
		case WSAENOTSOCK:	//服务端socket没开启
			{
				m_bIsConnSrv = FALSE;
			}
			break;
		case WSAECONNRESET:	//服务端socket强制关闭
			{
				m_bIsConnSrv = FALSE;
			}
			break;
		default:
			{
				int a = 0;
			}
		break;
	}
}

void CNeClient::OnIdly()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->OnRun();
}


UINT CNeClient::CreateConn(const char* pszIP, UINT iPort)
{
	CreateSocket();
	UINT iErr = Connect(pszIP, iPort);
	if (iErr == 0)
	{
		CreateTcpClientThread();
		m_bIsConnSrv = TRUE;
	}
	return iErr;
}


UINT CNeClient::Send(const char* strMsg, USHORT usCmdType, UINT iLen)
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



void CNeClient::WritePack(CComNeMsg* pNeMsg)
{
	m_CsPack.Lock();
	m_qClientPack.push(pNeMsg);
	m_CsPack.Unlock();
}


void CNeClient::ReadAndPopPack(CComNeMsg** ppNeMsg)
{
	m_CsPack.Lock();
	if (true != m_qClientPack.empty())
	{
		CComNeMsg* pComNeMsg = m_qClientPack.front();
		*ppNeMsg = pComNeMsg;
		m_qClientPack.pop();
	}
	else
	{
		*ppNeMsg = NULL;
	}
	m_CsPack.Unlock();
}


void CNeClient::CloseClt()
{
	m_bIsConnSrv = FALSE;
	Close();
}
