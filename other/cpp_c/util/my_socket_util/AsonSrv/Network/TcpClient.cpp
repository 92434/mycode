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
	//2.MML������
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
		case 0:				//����������ر�socket
			{
				m_bIsConn = FALSE;	
			}
		break;
		case WSAECONNABORTED://�ͻ��� socket���ر�    --�ͻ����Լ��˳�
			{
				m_bIsConn = FALSE;
			}
		break;
		case WSAENOTSOCK:	//�����socketû����
			{
				m_bIsConn = FALSE;
			}
		break;
		case WSAECONNRESET:	//�����socketǿ�ƹر�
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
	
	//����Э�鴦�������ֵ���Ӧ
	USHORT uCmd = pComNeMsg->m_obComNeHead.GetCmd();
	switch(uCmd)
	{
	case LANCMD_LIMIT:
		return ;
		
	case LANCMD_INFO:
	case LANCMD_CMDLN:
		{
			//��Ԫ��Ӧ��Ϣ����m_pComNETask�ύ����Ϣ�ַ�ģ��
			//1����CComNEMsgԤ������֤SrcNeId��Ч
// 			pComNeMsg->m_obComNeHead.SetSrcNeId(pComNeMsg->m_obComNeHead.GetSrcNeId() & 0XFFFFFF);
// 			
// 			if ( (pComNeMsg->m_obComNeHead.GetSrcNeId() & 0xFF0000) == 0 ) 
// 			{
// 				//SrcNeId��һ����Ч����ԪID������Ϊ0,��DstNeIdȴ����һ����Ч����ԪID
// 				//����������ԪID
// 				pComNeMsg->m_obComNeHead.SetSrcNeId(pComNeMsg->m_obComNeHead.GetDstNeId() & 0xFFFFFF);
// 			}
			
			//2��ֻ��¼�������У��������߳��д�����߽���Ч��
			WriteMsgLock(pComNeMsg);
		}
		break;
		
	case LANCMD_CON_RSP:	//������Ӧ
		{
			ProcessConnRspPacket(pComNeMsg);
		}
		break;
		
	case LANCMD_NELOOP:		//�������ݰ���Ӧ
		{
			ProcessNELoopRspPacket(pComNeMsg);
		}
		break;
		
	case LANCMD_OSLOOP:
		{
			//�յ�����֡������������
			m_nOsLoopNum = 0;
		}
		break;
	default:
		break;
	}

	//�������зǷ�������İ�
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
	
	//��ʱ���ͻ���֡
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
			//�����ĸ�����֡��û���յ�����֡��˵�����������⣬�Ͽ�����
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
// 		//MML��Ԫδ��¼
// 		m_bIsLogin = FALSE;
// 	}
	//pNEMsg�ĺ���λbyte����Ϣͷ�����������3��byte�ĵ�2��bitΪ1�����ʾ���¼��ϱ�
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
			//��������ID�Ϸ�, ����ȡ�óɹ�
			//m_ulNeID = pNEMsg->m_obComNeHead.GetSrcNeId();
			m_bIsConn = TRUE;
		}
		break;
	case 2:
	case 1:
		//��������δ�ɹ�������ID���Ϸ�����Ҫ������������ID��������
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



//---MMLЭ�鴦��----------------------------------------------------------
UINT CTcpClient::NeSend(void* pData, USHORT usDataLen, USHORT usCmd/* = LANCMD_CMDLN*/)
{
	if ( pData == NULL )
	{
		return 0 ;
	}
	
	CComNeHead msghead;
	msghead.SetDstNeId(m_ulNeID);  //�û��Լ�����,��ԪID
	msghead.SetLen(usDataLen);
	
    //���Ӷ� TU �û������⴦��
//     if (IsTUUserCmd((char*)pData))
//     {
//         msghead.SetCmd(LANCMD_TUONLY);
//     }
//     else
//     {
        msghead.SetCmd(usCmd);
//    }
	
	msghead.ToNetOrder();
	
	
	//����ϵ�һ���ٷ��ͳ�ȥ���������η��ͳ�ȥ�ٶȿ�ܶ�
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
//     // �ж��Ƿ���� TU �û��� ":login" �� ":lognew" ����
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



//-----HFCPЭ�鴦��-----------------------------------

int CTcpClient::UploadSingleFile(CHfcpDataCfg& obHfcpDataCfg)
{
	string& strNEFileAllName = obHfcpDataCfg.GetNeFileName();
	string& strLocalFileAllName = obHfcpDataCfg.GetLocalFileName();
	int nNELen = strNEFileAllName.length();			// ��Ԫ���ļ�ȫ·������
    int nLocalLen = strLocalFileAllName.length();	// ���ز��ļ�ȫ·������
	if ((0 >= nNELen || BRBASE_FILENAME_MAX <= nNELen) ||
        (0 >= nLocalLen || BRBASE_FILENAME_MAX <= nLocalLen))
    {
		g_obFileLog.WriteLog("\r\nUpload file Error: File name most length.\r\n");
        return -1;
    }

	UINT uDataLen = BRBASE_FILENAME_MAX * 2;// ����256�ֽڷֱ�����������Ԫ���ļ�·���ͱ����ļ�·��
    UINT ulSize = sizeof(HFCP_PROT_HEADER) + uDataLen;// HFCPͷ���������ݲ���
    char* pszPacket = new char[ulSize];
    // �ڴ����ʧ�ܣ������ڴ�ʧ�ܴ�����
    if (NULL == pszPacket)
    {
		g_obFileLog.WriteLog("\r\nUpload file Error: Create memory fail.\r\n");
        return -2;
    }
    memset(pszPacket, 0, ulSize);

	PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
    char* pszBody = pszPacket + sizeof(HFCP_PROT_HEADER);

    // �������ݰ���ͷ
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));// ���ͷǰ�ĸ��ֽ��п���@SBS
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_START);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = HFCP_SERIAL_NUM_ZERO;
    pHeader->usResult = HFCP_PACK_SUCCESS;
    pHeader->usEndTag = HFCP_END_TAG;
    pHeader->uDataLen = uDataLen;
    pHeader->uBufAddr = 0x00;
    pHeader->uBufLen = 0x00;
	
    // ������Ԫ���ļ���
    memcpy(pszBody, strNEFileAllName.c_str(), nNELen);
	
    // ���������ļ���
    memcpy(pszBody+BRBASE_FILENAME_MAX, strLocalFileAllName.c_str(), nLocalLen);
	
    // ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();


	// д���������
    UINT uCheckSum = CalcCheckSum(pszBody,uDataLen);
    pHeader->uBodyCheckSum = ntohl(uCheckSum);
	
    // д���ͷ�����
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
	//�������ݰ����
	if (0 >= usPackNo)
    {
        return -1;
    }

	UINT ulSize = sizeof(HFCP_PROT_HEADER);
    char* pszPacket = new char[ulSize];// Ϊ������HFCP֡�����ڴ棬��Ϊ������Ԫ�������ݣ���������֡�ı����СΪ0����ֻ�а�ͷ
    if (NULL == pszPacket)
    {
        return -2;
    }
    memset(pszPacket, 0, ulSize);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);

	// �������ݰ���ͷ
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = usPackNo;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = (obHfcpDataCfg.GetPackNo() - 1) * obHfcpDataCfg.GetPackSize();
    pHeader->uBufLen     = obHfcpDataCfg.GetPackSize();

	UINT uPackCount = obHfcpDataCfg.GetPackCount();
	if (0 >= uPackCount)             // �����ļ���СΪ0���������
    {
        pHeader->uBufLen = 0;
    }
    else if (uPackCount == usPackNo) // ������ļ����һ��
    {
        pHeader->uBufLen = obHfcpDataCfg.GetFileSize() - pHeader->uBufAddr;
    }

	// ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();
	// д�����У���
	UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);

	// д���ͷУ���
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
    char* pszPacket = new char[ulSize];// �����ڴ�
    if (NULL == pszPacket)
    {
        return -1;
    }
    memset(pszPacket, 0, ulSize);
    HFCP_PROT_HEADER* pHeader = (HFCP_PROT_HEADER*)pszPacket;
	
    // �������ݰ���ͷ
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_END);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = 0x00;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = 0x00;
    pHeader->uBufLen     = 0x00;
	
    // ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();

	// д�����У���
    UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
    // д���ͷУ���
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);
	
    // �����ݰ����ͳ�ȥ
    int iErr = NeSend(pszPacket, ulSize);
	
    delete[] pszPacket;
    pszPacket = NULL;

	return iErr;
}


int CTcpClient::SendDownLoadFileBegin(CHfcpDataCfg& obHfcpDataCfg)
{
	string& strNEFileAllName = obHfcpDataCfg.GetNeFileName();
	string& strLocalFileAllName = obHfcpDataCfg.GetLocalFileName();
	int nNELen = strNEFileAllName.length();			// ��Ԫ���ļ�ȫ·������
    int nLocalLen = strLocalFileAllName.length();	// ���ز��ļ�ȫ·������
	if ((0 >= nNELen || BRBASE_FILENAME_MAX <= nNELen) ||
        (0 >= nLocalLen || BRBASE_FILENAME_MAX <= nLocalLen))
    {
        return -1;
    }
	
	UINT uDataLen = BRBASE_FILENAME_MAX * 2;// ����256�ֽڷֱ�����������Ԫ���ļ�·���ͱ����ļ�·��
    UINT ulSize = sizeof(HFCP_PROT_HEADER) + uDataLen;// HFCPͷ���������ݲ���
    char* pszPacket = new char[ulSize];
    // �ڴ����ʧ�ܣ������ڴ�ʧ�ܴ�����
    if (NULL == pszPacket)
    {
        return -2;
    }
	
    memset(pszPacket, 0, ulSize);
	char* pszBody = pszPacket + sizeof(HFCP_PROT_HEADER);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
	
	//�������ݰ���ͷ
	memcpy(pHeader,HFCP_KEY_WORD,strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_DOWN_LOAD_START);
	pHeader->usPackType  = obHfcpDataCfg.GetSendCmd();
	pHeader->usSerialNum = HFCP_SERIAL_NUM_ZERO;
	pHeader->usResult    = HFCP_PACK_SUCCESS;
	pHeader->usEndTag    = HFCP_END_TAG;
	pHeader->uDataLen    = uDataLen;
	pHeader->uBufAddr    = 0x00;
	pHeader->uBufLen     = 0x00;
	
	// ������Ԫ���ļ���
    memcpy(pszBody, strNEFileAllName.c_str(), nNELen);
	
    // ���������ļ���
    memcpy(pszBody + BRBASE_FILENAME_MAX, strLocalFileAllName.c_str(), nLocalLen);
	
	   // ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();
	
	// д���������
    UINT uCheckSum = CalcCheckSum(pszBody,uDataLen);
    pHeader->uBodyCheckSum = ntohl(uCheckSum);
	
    // д���ͷ�����
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
	//�������ݰ����
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
    char* pszPacket = new char[ulSize];// Ϊ������HFCP֡�����ڴ棬��Ϊ������Ԫ�������ݣ���������֡�ı����СΪ0����ֻ�а�ͷ
    if (NULL == pszPacket)
    {
        return -2;
    }
    memset(pszPacket, 0, ulSize);
    PHFCP_PROT_HEADER pHeader = PHFCP_PROT_HEADER(pszPacket);
	
	// �������ݰ���ͷ
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
	//�õ��ļ�����
	memcpy(pszBady, obHfcpDataCfg.GetMemFilePack(), uPackSize);
	//�õ�����У���
	UINT uCheckSum = CalcCheckSum(pszBady, uPackSize);//obHfcpDataCfg.GetPackSize()
	UINT uPackCount = obHfcpDataCfg.GetPackCount();
	if (0 >= uPackCount)             // �����ļ���СΪ0���������
    {
		pHeader->usEndTag = HFCP_END_TAG;
        pHeader->uBufLen = 0;
		pHeader->uDataLen = 0;
		uCheckSum = 0x00;
    }
    else if (uPackCount == usPackNo) // ������ļ����һ��
    {
		pHeader->usEndTag = HFCP_END_TAG;
        pHeader->uBufLen = obHfcpDataCfg.GetFileSize() - pHeader->uBufAddr;
	//	pHeader->uDataLen = pHeader->uBufLen;

	//	uCheckSum = CalcCheckSum(pszPacket + sizeof(HFCP_PROT_HEADER), uPackSize);
    }
	

	
	// ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();

	// д�����У���
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
	// д���ͷУ���
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
    char* pszPacket = new char[ulSize];// �����ڴ�
    if (NULL == pszPacket)
    {
        return -1;
    }
    memset(pszPacket, 0, ulSize);
    HFCP_PROT_HEADER* pHeader = (HFCP_PROT_HEADER*)pszPacket;
	
    // �������ݰ���ͷ
    memcpy(pHeader, HFCP_KEY_WORD, strlen(HFCP_KEY_WORD));
	obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_DOWN_LOAD_END);
    pHeader->usPackType = obHfcpDataCfg.GetSendCmd();
    pHeader->usSerialNum = 0x00;
    pHeader->usResult    = HFCP_PACK_SUCCESS;
    pHeader->usEndTag    = HFCP_END_TAG;
    pHeader->uDataLen    = 0x00;
    pHeader->uBufAddr    = 0x00;
    pHeader->uBufLen     = 0x00;
	
    // ����ͷ�������ת���������ֽ�
    pHeader->ToNetOrder();
	
	// д�����У���
    UINT uCheckSum = 0x00;
    pHeader->uBodyCheckSum = htonl(uCheckSum);
	
    // д���ͷУ���
    uCheckSum = CalcCheckSum(pszPacket, sizeof(HFCP_PROT_HEADER) - sizeof(UINT));
    pHeader->uHeaderCheckSum = htonl(uCheckSum);
	
    // �����ݰ����ͳ�ȥ
    int iErr = NeSend(pszPacket, ulSize);
	
    delete[] pszPacket;
    pszPacket = NULL;
	
	return iErr;
}

