// NeOperate.cpp: implementation of the CNeOperate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeOperate.h"

#include "..\SevFrame.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeOperate::CNeOperate()
{
	m_hEvent = NULL;
	m_bIsRead = FALSE;
	m_bIsLogin = FALSE;
}

CNeOperate::~CNeOperate()
{
	Disconnect();
}

UINT CNeOperate::Connect(string strNeIP, UINT iPort/* = 1400*/, ULONG ulNeID/* = 0*/)
{
	m_strNeIP = strNeIP;
	m_uiPort = iPort;
	m_obTcpClient.SetNeID(ulNeID);
	CreateRspThread();
	UINT iErr = m_obTcpClient.NeCreateConn(m_strNeIP.c_str(), m_uiPort);

	//�ȴ����ӳɹ�
	int iLoop = 0;
	while(m_obTcpClient.NeIsConn() == FALSE && iLoop < 20)
	{
		Sleep(30);
		iLoop ++;
	}
	
	return iErr;
}

void CNeOperate::Disconnect()
{
	if ( NULL != m_hEvent)
	{
		::ResetEvent(m_hEvent);
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
	
	m_obTcpClient.NeClose();
	CloseRsp();
}

UINT CNeOperate::SendCmd(string strCmd, string& strRsp, UINT uiTimeout/* = 14000*/, BOOL bEvent/* = TRUE*/)
{
	if(m_obTcpClient.NeIsConn() == TRUE)
	{
		string strCmdLog = "\r\n" + strCmd + "\r\n";
		g_obFileLog.WriteLog(strCmdLog.c_str());
		m_bIsEvent = bEvent;
		if (bEvent == TRUE)
		{
			//Ϊ�������� ���ȴ�����
  			if (NULL == m_hEvent)
  			{
				m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			}

			m_strRsp = "";
			m_bIsRead = TRUE;
			strCmd +=  "\r\n";
			m_obTcpClient.NeSend((void*)strCmd.c_str(), strCmd.length());

			DWORD iErr = WaitForSingleObject(m_hEvent, uiTimeout);////INFINITE
			if(WAIT_TIMEOUT == iErr)
			{
				strRsp = "Cmd Timeout!\r\n";
				m_bIsRead = FALSE;
				return 2;
			}
			strRsp = m_strRsp;
			m_strRsp = "";
			m_bIsRead = FALSE;
			return 0;
		}
		else
		{
			strCmd +=  "\r\n";
			m_strRsp = "";
			m_bIsRead = TRUE;
			m_obTcpClient.NeSend((void*)strCmd.c_str(), strCmd.length());
			return 0;
		}
	}
	strRsp = "No connect NE !!\r\n";
	return 1;
}

// UINT CNeOperate::SendCmd(string strCmd, string& strRsp, UINT uiTimeout/* = 14000*/)
// {
// 	if(m_obTcpClient.NeIsConn() == TRUE)
// 	{
// 		strCmd +=  "\r\n";
// 		m_strRsp = "";
// 		m_bIsRead = TRUE;
// 		m_obTcpClient.NeSend((void*)strCmd.c_str(), strCmd.length());
// 		return 0;
// 	}
// 
// 	return 1;
// }

void CNeOperate::CreateRspThread()
{
	unsigned threadID;
	m_bRspOpen = TRUE;
	m_hRspThread = (HANDLE)_beginthreadex( NULL, 0, &FunRspThread, this, 0, &threadID );
	//�ȴ��߳�����
	Sleep(200);
}

unsigned CNeOperate::FunRspThread( void* _this )
{
	CNeOperate* pNeOperate = (CNeOperate*)_this;
	while(pNeOperate->m_bRspOpen)
	{
		CComNeMsg* pComNeMsg;
		pNeOperate->m_obTcpClient.ReadMsgPopLock(&pComNeMsg);
		if (pComNeMsg == NULL)
		{
			Sleep(10);
			continue;
		}

		int iSize = pComNeMsg->m_obComNeHead.GetLen();
		if ( iSize < 4 )
		{
			pComNeMsg->EmptyMsg();
			delete pComNeMsg;
			continue;
		}
		//����HFCP��
		//PHFCP_PROT_HEADER
		// ���ǰ�ĸ��ֽڲ���@SBS������HFCP֡,�ɹ�����0
		char* pMsg = pComNeMsg->GetMsg();
		string strHeadTag = "";
		strHeadTag.insert(0, pMsg, 4);
		if (strHeadTag == HFCP_KEY_WORD)
		{
			pNeOperate->AnalyseHFCP( pMsg, iSize );
// 			static UINT ss = 0;
// 			if (ss % 16 == 0)
// 			{
// 				Sleep(2);
// 				ss = 0;
// 			}
// 			ss++;
		}
		else
		{
			//�ڴ˴��������
			int iSign = -1;
			BOOL bIsEve = FALSE;
			iSign = pComNeMsg->GetMsg()[iSize - 3];
		
			if (iSign == 0 && pNeOperate->m_bIsRead == TRUE)
			{
				pNeOperate->m_strRsp += pComNeMsg->GetMsg();
				if (pNeOperate->m_bIsEvent == TRUE)
				{
					bIsEve = TRUE;
					//::SetEvent(pNeOperate->m_hEvent);
				}
				else
				{
					CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
					pSevFrame->m_obTcpServer.Send(pNeOperate->m_strRsp.c_str(), CMD_SEND_NE_CMD, pNeOperate->m_strRsp.length() );
				}
				pNeOperate->m_bIsRead = FALSE;
			}
			if (iSign == 1 && pNeOperate->m_bIsRead == TRUE)
			{
				pNeOperate->m_strRsp += pComNeMsg->GetMsg();
// 				if (pNeOperate->m_strRsp.length() > 500000)
// 				{
// 					if (pNeOperate->m_bIsEvent == TRUE)
// 					{
// 						bIsEve = TRUE;
// 						//::SetEvent(pNeOperate->m_hEvent);
// 					}
// 					else
// 					{
// 						CSevFrame* pSevFrame = CSevFrame::GetSrvFrame();
// 						pSevFrame->m_obTcpServer.Send(pNeOperate->m_strRsp.c_str(), CMD_SEND_NE_CMD, pNeOperate->m_strRsp.length() );
// 						pNeOperate->m_strRsp = "";
// 					}
// 					pNeOperate->m_bIsRead = FALSE;
// 				}
			}


			string strTmpMsg = pComNeMsg->GetMsg();

			if (strTmpMsg.find("LOGNEW  : Last log time") != string::npos || strTmpMsg.find("LOGIN  : Last log time") != string::npos)
			{
				pNeOperate->m_bIsLogin = TRUE;
			}
			else if ( strTmpMsg.find("Not login") != string::npos || strTmpMsg.find("EVENT_SM_LOGOUT") != string::npos ||
				strTmpMsg.find("LOGOUT EVENT") != string::npos )
			{
				pNeOperate->m_bIsLogin = FALSE;
			}
			
			//������д������־
			//OutputDebugString(strTmpMsg.c_str());
			g_obFileLog.WriteLog(strTmpMsg.c_str());
			if(bIsEve == TRUE)
			{
				::SetEvent(pNeOperate->m_hEvent);
			}
			//Sleep(3);
		}

		pComNeMsg->EmptyMsg();
		delete pComNeMsg;

	}

	_endthreadex( 0 );
	return 0;
}


void CNeOperate::CloseRsp()
{
	if( m_bRspOpen == TRUE )
	{
		m_bRspOpen = FALSE;
		WaitForSingleObject(m_hRspThread, INFINITE);
		CloseHandle( m_hRspThread );
		//CloseHandle( m_hEvent );
	}
}

void CNeOperate::GetHostIP( vector<string>& vStrIP )
{
	m_obTcpClient.GetHostIP(vStrIP);
}




//-----------HFCP����------------
void CNeOperate::UploadFile(string strNEFileAllName, string strLocalFileAllName)
{	
	if (m_osFile.is_open())
	{
		m_osFile.close();
	}

	m_obHfcpDataCfg.Clear();
	m_obHfcpDataCfg.SetNeFileName( strNEFileAllName.c_str() );
	m_obHfcpDataCfg.SetLocalFileName( strLocalFileAllName.c_str() );

	m_osFile.open(strLocalFileAllName.c_str(), ios::app | ios::binary | ios::ate );

	//1.���������ļ�����
	m_obTcpClient.UploadSingleFile(m_obHfcpDataCfg);

	DWORD iErr = m_EventUpload.WaitEvent(10000);
	if(WAIT_TIMEOUT == iErr)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: �����ļ����Դ���.\r\n");
	}

	if(m_obHfcpDataCfg.m_uFileSize == 0)
	{
		m_obTcpClient.SendRequestEnd(m_obHfcpDataCfg);
		return;
	}


	//2.�ȴ��õ�Ҫ���ص��ļ����԰�
// 	int iLoop = 500;
// 	while (m_obHfcpDataCfg.m_uFileSize == 0 && iLoop > 0)
// 	{
// 		Sleep(40);
// 		iLoop--;
// 	}
// 
// 	if (iLoop == 0)
// 	{
// 		m_osFile.close();
// 		g_obFileLog.WriteLog("\r\nUpload file Error: �����ļ����Դ���.\r\n");
// 		return;
// 	}

	//3.�����������ݰ�
	UINT uLoopCount = 0;
	UINT uPackCount = m_obHfcpDataCfg.GetPackCount();
	m_obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_DATA);
	while( TRUE )
	{
		int iSize = uPackCount - m_obHfcpDataCfg.GetPackNo();

		if (iSize == 0)
		{
			break;
		}
		
		if (iSize > m_obHfcpDataCfg.GetPackMaxCount())
		{
			iSize = m_obHfcpDataCfg.GetPackMaxCount(); 
		}

		int iErr = 0;
		UINT uPackNo = 0;
		for (UINT i = 0; i < iSize; i++ )
		{
			uPackNo = m_obHfcpDataCfg.GetPackNo();
			m_obHfcpDataCfg.SetPackNo( uPackNo + 1 );
			iErr = m_obTcpClient.SendRequestData(m_obHfcpDataCfg);
		}

		int iLoop = 100;
		while (m_obHfcpDataCfg.m_usRspPackNo + 2  < uPackNo && iLoop > 0)
		{
			Sleep(20);
			iLoop --;
		}

		if (iLoop == 0)
		{
			g_obFileLog.WriteLog("\r\nUpload file Error: �ȴ��������ݰ���ʱ.\r\n");
		}
	}

	iErr = m_EventUpload.WaitEvent(10000);
	if(WAIT_TIMEOUT == iErr)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: �ȴ���������ʱ.\r\n");
	}
}


//����0Ϊ�ɹ�
int CNeOperate::AnalyseHFCP(char* pMsg, int iSize)
{
	HFCP_PROT_HEADER obHeader;    
	USHORT usHeadLen = sizeof(HFCP_PROT_HEADER);
	memset(&obHeader, 0x00, usHeadLen);

	//ȡ����ϢЭ���ͷ
    memcpy(&obHeader, pMsg, usHeadLen);
    obHeader.ToHostOrder(); 
	//��Ϣ��
	char* pBody = pMsg + usHeadLen;
	int nRtn = -1;
	
    switch(obHeader.usPackType)
    {
        // ��Ԫ��Ӧ���ļ����ؿ�ʼ
	case PROT_SFTM_MEMFILE_UP_LOAD_START_ACK:
		nRtn = DoSingleUploadStartRsp(obHeader, pBody);
		break;

        // ��Ԫ��Ӧ���ļ����ؿ�ʼ
	case PROT_SFTM_MEMFILE_DOWN_LOAD_START_ACK:
		nRtn = DoSingleDownloadStartRsp(obHeader, pBody);
		break;
		
        // ��Ԫ��Ӧ �������ݰ�
    case PROT_SFTM_MEMFILE_UP_LOAD_DATA_ACK:
		nRtn = DoSingleUploadDataRsp(obHeader, pBody);
		break;
		
        // ��Ԫ��Ӧ �������ݰ�
    case PROT_SFTM_MEMFILE_DOWN_LOAD_DATA_ACK:
		nRtn = DoSingleDownloadDataRsp(obHeader, pBody);
		break;
        
        // ��Ԫ��Ӧ ���ļ����ؽ���
    case PROT_SFTM_MEMFILE_UP_LOAD_END_ACK:
		nRtn = DoSingleUploadEndRsp(obHeader, pBody);
		break;
        
		// ��Ԫ��Ӧ ���ļ����ؽ���
    case PROT_SFTM_MEMFILE_DOWN_LOAD_END_ACK:
		nRtn = DoSingleDownloadEndRsp(obHeader, pBody);
		break;
		
		/* BEGIN: Added by dengxuefeng 00119018, 2009/12/28 */
		//���������ļ���ʼ������Ӧ֡
    case PROT_SFTM_MEMFILE_BATCH_UPLOAD_START_ACK:
		//		nRtn = DoBatchFileUploadStartRsp(FileCopyInfo, struHeader, pszBody);
        break;
        
		//�������󵥸��ļ����ؿ�ʼ֡    
    case PROT_SFTM_MEMFILE_SINGLE_UPLOAD_START:
        //�յ���Ԫ�ϱ��ļ����ؿ�ʼ֡��ʼ
        //�·�0x42�����ڴ��ļ���ʼ����֡
		//        nRtn = DoNeSingleUploadRsp(FileCopyInfo, struHeader, pszBody);
        break;
        
		//�����������������ļ���������֡
    case PROT_SFTM_MEMFILE_BATCH_UPLOAD_END:        
		//       nRtn = DoNeBatchUploadEndRsp(FileCopyInfo, struHeader, pszBody);
        break;
		
        
		/* END:   Added by dengxuefeng 00119018, 2009/12/28 */
    default:
		nRtn = ERRORCODE_FAILED;
		//��¼��־
		
    }
    return nRtn;
}


UINT CNeOperate::DoSingleUploadStartRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_UP_LOAD_START)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: PROT_SFTM_MEMFILE_UP_LOAD_START.\r\n");
		return 1;
	}

	// �����ͷ�Ͱ���
    bool blnRtn = CheckPacket(obHeader, pBody);
	if (blnRtn == false)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: Check pack header and pack body error.\r\n");
		return 2;
	}

	USHORT usResult = obHeader.usResult;
	if (HFCP_PACK_SUCCESS != usResult)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: Upload Start error.\r\n");
		return 3;
	}

	int ddd = obHeader.uDataLen;
	int fg = obHeader.uBufLen;
	//��ȡ�ļ���С��Ϣ,
    UINT* puFileSize = (UINT*)pBody;
    UINT uFileSize  = ntohl(*puFileSize);
	
	m_obHfcpDataCfg.SetFileSize(uFileSize);
	

	m_EventUpload.SetEvent();
	//�����������ݰ���ָ��ͬʱ���Ͱ�����
// 	m_obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_DATA);
// 	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() + 1 );
// 	m_obTcpClient.SendRequestData(m_obHfcpDataCfg);
//	SendSameUploadPack();
	
	return 0;
}


UINT CNeOperate::DoSingleUploadDataRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_UP_LOAD_DATA)
	{
		return 1;
	}

	
	bool blnRtn = CheckPacket(obHeader, pBody);
    if (!blnRtn) // У�����
    {
        // ����������ȷ�����ط����ݣ���֮���ȴ���ʱ�ط�
       	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() - 1 );
		//�����������ݰ�
		m_obTcpClient.SendRequestData(m_obHfcpDataCfg);
        return 3;
    }

	UINT uRtrvLen = (obHeader.usSerialNum - 0x01) * m_obHfcpDataCfg.GetPackSize();//lint -save -e613 
    // �Ѿ����յ����ݼ��ϴ����յ����ݴӳ��Ȳ��ܴ����ļ��ڴ��ܳ���
	UINT uFactLen = uRtrvLen + obHeader.uDataLen;
    if ( uFactLen > m_obHfcpDataCfg.GetFileSize())
    {
        //��֤�������ݲ�������ڴ�Խ��
        return 2;
    }

	if( obHeader.usResult != HFCP_PACK_SUCCESS)
	{
		g_obFileLog.WriteLog("\r\nUpload file Error: �������ݴ���.\r\n");
		return 3;
	}

	if (uFactLen == m_obHfcpDataCfg.GetFileSize())
	{	
		// ���͵��ļ����ؽ���
        m_obTcpClient.SendRequestEnd(m_obHfcpDataCfg);
	}
	else
	{
		//�����������ݰ�
		//m_obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_DATA);
		//m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() + 1 );
		//m_obTcpClient.SendRequestData(m_obHfcpDataCfg);
	//	if (m_obHfcpDataCfg.GetPackNo() == obHeader.usSerialNum)
	//	{
//		SendSameUploadPack();
	//	}
		m_obHfcpDataCfg.m_usRspPackNo = obHeader.usSerialNum;
	}

	m_osFile.write(pBody, obHeader.uDataLen);
	//m_osFile.flush();

	return 0;
}


UINT CNeOperate::DoSingleUploadEndRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_UP_LOAD_END)
	{
		return 1;
	}

	bool blnRtn = CheckPacket(obHeader, pBody);
    if (!blnRtn)
    {
        // У�����
        m_obTcpClient.SendRequestEnd(m_obHfcpDataCfg);
        return 3;
    }

	USHORT usResult = obHeader.usResult;
    if (HFCP_PACK_SUCCESS != usResult)
    {
        return 4;
    }
	
//	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
// 	pWnd->PostMessage(WM_MSG_UPLOAD_FILE, 0, 0);

	m_osFile.close();
	m_EventUpload.SetEvent();
	return 0;
}

bool CNeOperate::CheckPacket(HFCP_PROT_HEADER& obHeader,char* pszBody)
{   
    UINT uCheckSum = 0;
    // У���ͷ
    uCheckSum = CalcCheckSum((char*)&obHeader, sizeof(HFCP_PROT_HEADER)
        - sizeof(obHeader.uHeaderCheckSum));
	
    bool blnRtn = (uCheckSum == obHeader.uHeaderCheckSum);
    if (!blnRtn || 0 == obHeader.uDataLen) //��ͷУ������û�а���
    {
        return blnRtn;
    }
	
    if (NULL == pszBody) //����Ϊ��
    {
        return false;
    }
	
    //У�����
    uCheckSum = CalcCheckSum(pszBody, obHeader.uDataLen);
    blnRtn = (uCheckSum == obHeader.uBodyCheckSum);
	
    return blnRtn;
}


void CNeOperate::DownloadFile(string strLocalFileAllName, string strNEFileAllName)
{
	//�ж��ļ��Ƿ����
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strLocalFileAllName.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	}

	DWORD dwFileSize = FindFileData.nFileSizeLow; 
	if (dwFileSize == 0)
	{
		FindClose(hFind); 
		return;
	}
	FindClose(hFind); 


	if (m_isFile.is_open())
	{
		m_isFile.close();
	}


	m_obHfcpDataCfg.Clear();
	m_obHfcpDataCfg.SetNeFileName( strNEFileAllName.c_str() );
	m_obHfcpDataCfg.SetLocalFileName( strLocalFileAllName.c_str() );

	m_obHfcpDataCfg.SetFileSize(dwFileSize);

	m_isFile.open(strLocalFileAllName.c_str(), ios::binary | ios::in );

	m_obTcpClient.SendDownLoadFileBegin(m_obHfcpDataCfg);
}


UINT CNeOperate::DoSingleDownloadStartRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_DOWN_LOAD_START)
	{
		return 1;
	}
	
	// �����ͷ�Ͱ���
    bool blnRtn = CheckPacket(obHeader, pBody);
	if (blnRtn == false)
	{
		return 2;
	}
	
	USHORT usResult = obHeader.usResult;
	if (HFCP_PACK_SUCCESS != usResult)
	{
		UINT* puError = (UINT*)pBody;
		UINT uError  = ntohl(*puError);
		//	m_obTcpClient.SendDownLoadFileBegin(m_obHfcpDataCfg);
		return 3;
	}


	m_isFile.read(m_obHfcpDataCfg.GetMemFilePack(), m_obHfcpDataCfg.GetPackSize());
	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() + 1 );
	m_obTcpClient.SendDownLoadData(m_obHfcpDataCfg);
//	m_isFile.gcount();
//	SendSameDownloadPack();

	return 0;
}


UINT CNeOperate::DoSingleDownloadDataRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_DOWN_LOAD_DATA)
	{
		return 1;
	}
	
	bool blnRtn = CheckPacket(obHeader, pBody);
    if (!blnRtn) // У�����
    {
        // ����������ȷ�����ط����ݣ���֮���ȴ���ʱ�ط�
       	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() - 1 );
		//�����������ݰ�
		m_obTcpClient.SendDownLoadData(m_obHfcpDataCfg);
        return 3;
    }

	USHORT usResult = obHeader.usResult;
	
    if (HFCP_PACK_SUCCESS != usResult)
    {
		UINT* puError = (UINT*)pBody;
		UINT uError  = ntohl(*puError);
		if(HFCP_ERROR_PACKNO != uError)
		{
			return 2;
		}
	}

	
	if (m_obHfcpDataCfg.GetPackCount() == obHeader.usSerialNum - 1)
	{
		m_obTcpClient.SendDownLoadEnd(m_obHfcpDataCfg);
		return 0;
	}

// 	m_isFile.read(m_obHfcpDataCfg.GetMemFilePack(), m_obHfcpDataCfg.GetPackSize());
// 	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() + 1 );
// 	m_obTcpClient.SendDownLoadData(m_obHfcpDataCfg);
	SendSameDownloadPack();

	return 0;
}


UINT CNeOperate::DoSingleDownloadEndRsp(HFCP_PROT_HEADER obHeader, char* pBody)
{
	if (m_obHfcpDataCfg.GetSendCmd() != PROT_SFTM_MEMFILE_DOWN_LOAD_END)
	{
		return 1;
	}
	
	bool blnRtn = CheckPacket(obHeader, pBody);
    if (!blnRtn)
    {
        // У�����
        m_obTcpClient.SendRequestEnd(m_obHfcpDataCfg);
        return 3;
    }
	
	USHORT usResult = obHeader.usResult;
    if (HFCP_PACK_SUCCESS != usResult)
    {
        return 4;
    }

	m_isFile.close();

	m_obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_LOAD_STOP);
	return 0;

}



int CNeOperate::SendSameDownloadPack()
{
	UINT uPackCount = m_obHfcpDataCfg.GetPackCount();
	int iSize = uPackCount - m_obHfcpDataCfg.GetPackNo();
	
	if (iSize >= m_obHfcpDataCfg.GetPackMaxCount())
	{
		iSize = m_obHfcpDataCfg.GetPackMaxCount(); 
	}
	
	m_obHfcpDataCfg.SetSendCmd(PROT_SFTM_MEMFILE_UP_LOAD_DATA);
	int iErr = 0;
	for (UINT i = 0; i < iSize; i++ )
	{
		UINT uPackNo = m_obHfcpDataCfg.GetPackNo();
		m_obHfcpDataCfg.SetPackNo( uPackNo + 1 );
		//iErr = m_obTcpClient.SendRequestData(m_obHfcpDataCfg);
		if (iErr != -9)
		{
			m_isFile.read(m_obHfcpDataCfg.GetMemFilePack(), m_obHfcpDataCfg.GetPackSize());
		}
	//	m_obHfcpDataCfg.SetPackNo( m_obHfcpDataCfg.GetPackNo() + 1 );
		iErr = m_obTcpClient.SendDownLoadData(m_obHfcpDataCfg);
		if (iErr <= 0)
		{
			iErr = -9;
			m_obHfcpDataCfg.SetPackNo( uPackNo );
			i--;
			continue;
		}
	//	if ( i%4 == 0  )// == iSize
	//	{
			Sleep(10);
	//	}
	}
	
	return iErr;
}



