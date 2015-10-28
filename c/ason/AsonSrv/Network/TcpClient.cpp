// TcpClient.cpp: implementation of the CTcpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TcpClient.h"
#include "HfcpDefine.h"

#include <TIME.H>

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpClient::CTcpClient()
{
	m_bIsConn = FALSE;
	m_ulNeID = 0;
	m_nOsLoopNum = 0;
}

CTcpClient::~CTcpClient()
{

}


void CTcpClient::OnCltRecve(CHAR* pMsg, int iSize)
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
		if (bIsHead == FALSE && iCount - iSign < sizeof(CComNeHead))
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
			pComNeMsg->EmptyMsg();
			memcpy( &pComNeMsg->m_obComNeHead, cBuf + iSign,  sizeof(CComNeHead) );
			iSign += sizeof(CComNeHead);
			pComNeMsg->m_obComNeHead.ToHostOrder();
			bIsHead = TRUE;
		}

		if ( bIsHead == TRUE)
		{
			if (bIsbody == FALSE)
			{	
				pComNeMsg->CreateBuf(pComNeMsg->m_obComNeHead.GetLen());
				bIsbody = TRUE;
			}
			int iSurplus = pComNeMsg->GetMsgLen() - pComNeMsg->GetTrulyLen();
			if( iCount - iSign >= iSurplus)
			{
				pComNeMsg->AddMsg(cBuf + iSign, iSurplus);
				iSign += iSurplus;
				bIsHead = FALSE;
				bIsbody = FALSE;
				m_qComNeMsg.push(pComNeMsg);
				//m_vComNeMsg.push_back(pComNeMsg);
				//obComNeMsg.EmptyMsg();
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


void CTcpClient::OnErrorInfo(UINT iErrID)
{
	switch (iErrID)
	{
		case 0:				//服务端正常关闭socket
			{
				m_bIsConn = FALSE;	
			}
		break;
		case WSAECONNABORTED://客户端 socket被关闭    --客户端自己退出
			{
				m_bIsConn = FALSE;
			}
		break;
		case WSAENOTSOCK:	//服务端socket没开启
			{
				m_bIsConn = FALSE;
			}
		break;
		case WSAECONNRESET:	//服务端socket强制关闭
			{
				m_bIsConn = FALSE;
			}
		break;
		default:
			{
				int a = 0;
			}
		break;
	}
}

void CTcpClient::OnIdly()
{
	if (m_qComNeMsg.empty() == true)
	{
		//Sleep(100);
		return;
	}
	
	CComNeMsg* pComNeMsg = m_qComNeMsg.front();
	
	//按照协议处理命令字的响应
	USHORT uCmd = pComNeMsg->m_obComNeHead.GetCmd();
	switch(uCmd)
	{
	case LANCMD_LIMIT:
		return ;
		
	case LANCMD_INFO:
	case LANCMD_CMDLN:
		{
			//网元响应消息，由m_pComNETask提交给消息分发模块
			//1、对CComNEMsg预处理，保证SrcNeId有效
// 			pComNeMsg->m_obComNeHead.SetSrcNeId(pComNeMsg->m_obComNeHead.GetSrcNeId() & 0XFFFFFF);
// 			
// 			if ( (pComNeMsg->m_obComNeHead.GetSrcNeId() & 0xFF0000) == 0 ) 
// 			{
// 				//SrcNeId是一个无效的网元ID，并且为0,而DstNeId却包含一个有效的网元ID
// 				//重新设置网元ID
// 				pComNeMsg->m_obComNeHead.SetSrcNeId(pComNeMsg->m_obComNeHead.GetDstNeId() & 0xFFFFFF);
// 			}
			
			//2、只记录到队列中，在其他线程中处理，提高接收效率
			WriteMsgLock(pComNeMsg);
		}
		break;
		
	case LANCMD_CON_RSP:	//连接响应
		{
			ProcessConnRspPacket(pComNeMsg);
		}
		break;
		
	case LANCMD_NELOOP:		//环回数据包响应
		{
			ProcessNELoopRspPacket(pComNeMsg);
		}
		break;
		
	case LANCMD_OSLOOP:
		{
			//收到环回帧，计数器清零
			m_nOsLoopNum = 0;
		}
		break;
	default:
		break;
	}

	//销毁所有非发送命令的包
	if ( !(LANCMD_CMDLN == uCmd || LANCMD_INFO == uCmd) )
	{
		pComNeMsg->EmptyMsg();
		if(pComNeMsg != NULL)
		{
			delete pComNeMsg;
			pComNeMsg = NULL;
		}
	}
	m_qComNeMsg.pop();
	
	//定时发送环回帧
	static UINT nTime = 0;
	nTime = nTime + 1;
	if (nTime > 2000)
	{
		SendOsLoop();
		nTime = 0;
	}
}

void CTcpClient::ProcessNELoopRspPacket(CComNeMsg* pNeMsg)
{
	pNeMsg->m_obComNeHead.ToNetOrder();
	Send((const char *)&pNeMsg->m_obComNeHead, sizeof(CComNeHead));
	pNeMsg->m_obComNeHead.ToHostOrder();
}

void CTcpClient::SendOsLoop()
{
	CComNeHead msghead;

	if ( NeIsConn() )
	{
		m_nOsLoopNum++;
	
		if ( m_nOsLoopNum > OS_LOOP_NUM)
		{
			//连发四个环回帧都没有收到环回帧，说明连接有问题，断开连接
			NeClose();
			m_nOsLoopNum = 0;
		}
		else
		{
			msghead.SetDstNeId(GetNeID());
			msghead.SetCmd(LANCMD_OSLOOP);
			msghead.SetLen(0);
			msghead.ToNetOrder();
			Send((const char *)&msghead, sizeof(CComNeHead));
		}
	}
}


// void CTcpClient::ProcessPushMsg(CComNeMsg* pNeMsg)
// {
// 	int iSize = pNeMsg->m_obComNeHead.GetLen();
// 
// 	int iSign = -1;
// 	if (iSize >= 4)
// 	{
// 		iSign = pNeMsg->GetMsg()[iSize - 3];
// 	}

	
// 	if (iSign == 0)
// 	{
// 		::SetEvent(m_hEvent);
// 	}
	
// 	if (strcmp( pNeMsg->GetMsg(), "Not login") // todo 14
// 		||strcmp( pNeMsg->GetMsg(), "*EVENT_SM_LOGOUT*")
// 		||strcmp( pNeMsg->GetMsg(), "*LOGOUT EVENT*"))
// 	{
// 		//MML网元未登录
// 		m_bIsLogin = FALSE;
// 	}
	//pNEMsg的后四位byte是消息头，如果倒数第3个byte的第2个bit为1，则表示是事件上报
//	CInitializeCriticalSection
//	CCriticalSection ssd;
//	ssd.m_sect
//}

void CTcpClient::ProcessConnRspPacket(CComNeMsg* pNEMsg)
{	
	if ( pNEMsg->GetMsgLen() < 4 ) 
	{
		return;
	}
	
	ULONG ulFlag;
	ulFlag = *((ULONG*)(pNEMsg->GetMsg()));
	ulFlag = ntohl(ulFlag);
	
	switch(ulFlag)
	{
	case 0:
		{
			//表明连接ID合法, 连接取得成功
			//m_ulNeID = pNEMsg->m_obComNeHead.GetSrcNeId();
			m_bIsConn = TRUE;
		}
		break;
	case 2:
	case 1:
		//表明连接未成功，连接ID不合法，需要重新生成连接ID申请连接
		m_bIsConn = FALSE;
		USHORT usCmd = LANCMD_CON_NEW;
		ULONG ulConnectId[2];
		time_t  tm;
		ulConnectId[0] = time(&tm) + 1;
		ulConnectId[1] = time(&tm) + 2;
		NeSend(ulConnectId, 2*sizeof(ULONG), usCmd);
	break;
	}
}



//---MML协议处理----------------------------------------------------------
UINT CTcpClient::NeSend(void* pData, USHORT usDataLen, USHORT usCmd/* = LANCMD_CMDLN*/)
{
	if ( pData == NULL )
	{
		return 0 ;
	}
	
	CComNeHead msghead;
	msghead.SetDstNeId(m_ulNeID);  //用户自己输入,网元ID
	msghead.SetLen(usDataLen);
	
    //增加对 TU 用户的特殊处理
//     if (IsTUUserCmd((char*)pData))
//     {
//         msghead.SetCmd(LANCMD_TUONLY);
//     }
//     else
//     {
        msghead.SetCmd(usCmd);
//    }
	
	msghead.ToNetOrder();
	
	
	//先组合到一起再发送出去，比用两次发送出去速度快很多
	char *pBuf = new char[sizeof(CComNeHead) + usDataLen];
	memcpy(pBuf, &msghead, sizeof(CComNeHead));
	memcpy(pBuf + sizeof(CComNeHead), pData, usDataLen);
	
	int ret;
	ret = Send(pBuf, sizeof(CComNeHead) + usDataLen);
	
	delete [] pBuf;	
	return ret;
}

// BOOL CTcpClient::IsTuUserCmd(const char* pszCmd)
// {
//     if (NULL == pszCmd)
//     {
//         return false;
//     }
// 	
//     // 判断是否包含 TU 用户的 ":login" 或 ":lognew" 命令
//     if (::strcmp(pszCmd, TU_LOGIN)
//         || ::strcmp(pszCmd, TU_LOGNEW))
//     {
//         return TRUE;
//     }
// 	
//     return FALSE;
// }

UINT CTcpClient::NeCreateConn(const char* pszIP, UINT iPort)
{
	CreateSocket();
	UINT iErr = Connect(pszIP, iPort);
	if (iErr == 0)
	{
		CreateTcpClientThread();

		ULONG ulConnectId[2];
		time_t  tm;
		ulConnectId[0] = time(&tm) + 1;
		ulConnectId[1] = time(&tm) + 2;
		return NeSend(ulConnectId, 2*sizeof(ULONG), LANCMD_CON_NEW);	
	}
	return 0;
}


void CTcpClient::NeClose()
{
	Close();
	m_bIsConn = FALSE;

	while(TRUE) 
	{
		CComNeMsg* pComNeMsg = NULL; 
		ReadMsgPopLock(&pComNeMsg);
		if (pComNeMsg == NULL)
		{
			break;
		}
		delete pComNeMsg;
	} 
}



void CTcpClient::WriteMsgLock(CComNeMsg* pNeMsg)
{
	m_obCSection.Lock();
	m_qComRsp.push(pNeMsg);
	m_obCSection.Unlock();
}

void CTcpClient::ReadMsgPopLock(CComNeMsg** ppNeMsg)
{
	m_obCSection.Lock();
	if (true != m_qComRsp.empty())
	{
		CComNeMsg* pComNeMsg = m_qComRsp.front();
		*ppNeMsg = pComNeMsg;
		m_qComRsp.pop();
	}
	else
	{
		*ppNeMsg = NULL;
	}
	m_obCSection.Unlock();
}



//-----HFCP协议处理-----------------------------------

int CTcpClient::UploadSingleFile(CHfcpDataCfg& obHfcpDataCfg)
{
	string& strNEFileAllName = obHfcpDataCfg.GetNeFileName();
	string& strLocalFileAllName = obHfcpDataCfg.GetLocalFileName();
	int nNELen = strNEFileAllName.length();			// 网元侧文件全路径长度
    int nLocalLen = strLocalFileAllName.length();	// 本地侧文件全路径长度
	if ((0 >= nNELen || BRBASE_FILENAME_MAX <= nNELen) ||
        (0 >= nLocalLen || BRBASE_FILENAME_MAX <= nLocalLen))
    {
		g_obFileLog.WriteLog("\r\nUpload file Error: File name most length.\r\n");
        return -1;
    }

	UINT uDataLen = BRBASE_FILENAME_MAX * 2;// 两个256字节分别用来保存网元侧文件路径和本地文件路径
    UINT ulSize = sizeof(HFCP_PROT_HEADER) + uDataLen;// HFCP头部加上数据部分
    char* pszPacket = new char[ulSize];
    // 内存分配失败，返回内存失败错误码
    if (NULL == pszPacket)
    {
		g_obFileLog.WriteLog("\r\nUpload file Error: Create memory fail.\r\n");
        return -2;
    }
    memset(pszPacket, 0, ulSize);

	PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
    char* pszBody = pszPacket + sizeof(HFCP_PROT_HEADER);

    // 构造数据包包头
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));// 向包头前四个字节中拷贝@SBS
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_START);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = HFCP_SERIAL_NUM_ZERO;
    pHeader->usResult = HFCP_PACK_SUCCESS;
    pHeader->usEndTag = HFCP_END_TAG;
    pHeader->uDataLen = uDataLen;
    pHeader->uBufAddr = 0x00;
    pHeader->uBufLen = 0x00;
	
    // 拷贝网元侧文件名
    memcpy(pszBody, strNEFileAllName.c_str(), nNELen);
	
    // 拷贝本地文件名
    memcpy(pszBody+BRBASE_FILENAME_MAX, strLocalFileAllName.c_str(), nLocalLen);
	
    // 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();


	// 写入包体检验和
    UINT uCheckSum = CalcCheckSum(pszBody,uDataLen);
    pHeader->uBodyCheckSum = ntohl(uCheckSum);
	
    // 写入包头检验和
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = ntohl(uCheckSum);

	int iErr = NeSend(pszPacket, ulSize);

	delete[] pszPacket;
    pszPacket = NULL;

	return iErr;
}


int CTcpClient::SendRequestData(CHfcpDataCfg& obHfcpDataCfg)
{
	USHORT usPackNo = obHfcpDataCfg.GetPackNo();
	//请求数据包序号
	if (0 >= usPackNo)
    {
        return -1;
    }

	UINT ulSize = sizeof(HFCP_PROT_HEADER);
    char* pszPacket = new char[ulSize];// 为待发的HFCP帧分配内存，因为是向网元请求数据，所以请求帧的保体大小为0，即只有包头
    if (NULL == pszPacket)
    {
        return -2;
    }
    memset(pszPacket, 0, ulSize);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);

	// 构造数据包包头
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = usPackNo;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = (obHfcpDataCfg.GetPackNo() - 1) * obHfcpDataCfg.GetPackSize();
    pHeader->uBufLen     = obHfcpDataCfg.GetPackSize();

	UINT uPackCount = obHfcpDataCfg.GetPackCount();
	if (0 >= uPackCount)             // 处理文件大小为0的特殊情况
    {
        pHeader->uBufLen = 0;
    }
    else if (uPackCount == usPackNo) // 如果是文件最后一包
    {
        pHeader->uBufLen = obHfcpDataCfg.GetFileSize() - pHeader->uBufAddr;
    }

	// 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();
	// 写入包体校验和
	UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);

	// 写入包头校验和
	uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);

	int iErr = NeSend(pszPacket, ulSize);
	
	delete[] pszPacket;
    pszPacket = NULL;

	return iErr;
}


int CTcpClient::SendRequestEnd(CHfcpDataCfg& obHfcpDataCfg)
{
	UINT ulSize = sizeof(HFCP_PROT_HEADER);
    char* pszPacket = new char[ulSize];// 申请内存
    if (NULL == pszPacket)
    {
        return -1;
    }
    memset(pszPacket, 0, ulSize);
    HFCP_PROT_HEADER* pHeader = (HFCP_PROT_HEADER*)pszPacket;
	
    // 构造数据包包头
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_END);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = 0x00;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = 0x00;
    pHeader->uBufLen     = 0x00;
	
    // 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();

	// 写入包体校验和
    UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
    // 写入包头校验和
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);
	
    // 将数据包发送出去
    int iErr = NeSend(pszPacket, ulSize);
	
    delete[] pszPacket;
    pszPacket = NULL;

	return iErr;
}


int CTcpClient::SendDownLoadFileBegin(CHfcpDataCfg& obHfcpDataCfg)
{
	string& strNEFileAllName = obHfcpDataCfg.GetNeFileName();
	string& strLocalFileAllName = obHfcpDataCfg.GetLocalFileName();
	int nNELen = strNEFileAllName.length();			// 网元侧文件全路径长度
    int nLocalLen = strLocalFileAllName.length();	// 本地侧文件全路径长度
	if ((0 >= nNELen || BRBASE_FILENAME_MAX <= nNELen) ||
        (0 >= nLocalLen || BRBASE_FILENAME_MAX <= nLocalLen))
    {
        return -1;
    }
	
	UINT uDataLen = BRBASE_FILENAME_MAX * 2;// 两个256字节分别用来保存网元侧文件路径和本地文件路径
    UINT ulSize = sizeof(HFCP_PROT_HEADER) + uDataLen;// HFCP头部加上数据部分
    char* pszPacket = new char[ulSize];
    // 内存分配失败，返回内存失败错误码
    if (NULL == pszPacket)
    {
        return -2;
    }
	
    memset(pszPacket, 0, ulSize);
	char* pszBody = pszPacket + sizeof(HFCP_PROT_HEADER);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
	
	//构造数据包包头
	memcpy(pHeader,HFCP_KEY_WORD,strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_DOWN_LOAD_START);
	pHeader->usPackType  = obHfcpDataCfg.GetSendCmd();
	pHeader->usSerialNum = HFCP_SERIAL_NUM_ZERO;
	pHeader->usResult    = HFCP_PACK_SUCCESS;
	pHeader->usEndTag    = HFCP_END_TAG;
	pHeader->uDataLen    = uDataLen;
	pHeader->uBufAddr    = 0x00;
	pHeader->uBufLen     = 0x00;
	
	// 拷贝网元侧文件名
    memcpy(pszBody, strNEFileAllName.c_str(), nNELen);
	
    // 拷贝本地文件名
    memcpy(pszBody + BRBASE_FILENAME_MAX, strLocalFileAllName.c_str(), nLocalLen);
	
	   // 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();
	
	// 写入包体检验和
    UINT uCheckSum = CalcCheckSum(pszBody,uDataLen);
    pHeader->uBodyCheckSum = ntohl(uCheckSum);
	
    // 写入包头检验和
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = ntohl(uCheckSum);
	
	int iErr = NeSend(pszPacket, ulSize);
	
	delete[] pszPacket;
    pszPacket = NULL;
	return iErr;
}


int CTcpClient::SendDownLoadData(CHfcpDataCfg& obHfcpDataCfg)
{
	USHORT usPackNo = obHfcpDataCfg.GetPackNo();
	//发送数据包序号
	if (0 >= usPackNo)
    {
        return -1;
    }

	UINT uPackSize = obHfcpDataCfg.GetPackSize();
	if ( usPackNo == obHfcpDataCfg.GetPackCount() )
	{
		uPackSize = obHfcpDataCfg.GetFileSize() %  uPackSize;
	}
	
	UINT ulSize = sizeof(HFCP_PROT_HEADER) + uPackSize;
    char* pszPacket = new char[ulSize];// 为待发的HFCP帧分配内存，因为是向网元请求数据，所以请求帧的保体大小为0，即只有包头
    if (NULL == pszPacket)
    {
        return -2;
    }
    memset(pszPacket, 0, ulSize);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
	
	// 构造数据包包头
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd( PROT_SFTM_MEMFILE_DOWN_LOAD_DATA );
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = usPackNo;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_CONTINUE_TAG;
    pHeader->uDataLen    = uPackSize;
    pHeader->uBufAddr    = (obHfcpDataCfg.GetPackNo() - 1) * obHfcpDataCfg.GetPackSize();
    pHeader->uBufLen     = uPackSize;
	
	char* pszBady = pszPacket + sizeof(HFCP_PROT_HEADER);
	//得到文件数据
	memcpy(pszBady, obHfcpDataCfg.GetMemFilePack(), uPackSize);
	//得到包体校验和
	UINT uCheckSum = CalcCheckSum(pszBady, uPackSize);//obHfcpDataCfg.GetPackSize()
	UINT uPackCount = obHfcpDataCfg.GetPackCount();
	if (0 >= uPackCount)             // 处理文件大小为0的特殊情况
    {
		pHeader->usEndTag = HFCP_END_TAG;
        pHeader->uBufLen = 0;
		pHeader->uDataLen = 0;
		uCheckSum = 0x00;
    }
    else if (uPackCount == usPackNo) // 如果是文件最后一包
    {
		pHeader->usEndTag = HFCP_END_TAG;
        pHeader->uBufLen = obHfcpDataCfg.GetFileSize() - pHeader->uBufAddr;
	//	pHeader->uDataLen = pHeader->uBufLen;

	//	uCheckSum = CalcCheckSum(pszPacket + sizeof(HFCP_PROT_HEADER), uPackSize);
    }
	

	
	// 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();

	// 写入包体校验和
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
	// 写入包头校验和
	uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);
	
	int iErr = NeSend(pszPacket, ulSize);
	
	delete[] pszPacket;
    pszPacket = NULL;
	
	return iErr;
}


int CTcpClient::SendDownLoadEnd(CHfcpDataCfg& obHfcpDataCfg)
{
	UINT ulSize = sizeof(HFCP_PROT_HEADER);
    char* pszPacket = new char[ulSize];// 申请内存
    if (NULL == pszPacket)
    {
        return -1;
    }
    memset(pszPacket, 0, ulSize);
    HFCP_PROT_HEADER* pHeader = (HFCP_PROT_HEADER*)pszPacket;
	
    // 构造数据包包头
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_DOWN_LOAD_END);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = 0x00;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = 0x00;
    pHeader->uBufLen     = 0x00;
	
    // 将包头相关内容转换成网络字节
    pHeader->ToNetOrder();
	
	// 写入包体校验和
    UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
    // 写入包头校验和
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);
	
    // 将数据包发送出去
    int iErr = NeSend(pszPacket, ulSize);
	
    delete[] pszPacket;
    pszPacket = NULL;
	
	return iErr;
}

