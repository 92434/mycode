// NeOper.cpp: implementation of the CNeOper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\YinAidanceKit.h"
#include "..\MainFrm.h"
#include "..\YinAidanceKitView.h"
#include "..\OutputView.h"
#include "NeOper.h"
#include "..\..\AsonSrv\FaceSvrHead.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeOper::CNeOper()
{
	m_bIsNeConn = FALSE;
	m_bIsNeLogin = FALSE;
	
	//HANDLE hProcess = GetCurrentProcess();//get current process
    //TerminateProcess(pi.hProcess,0);         //close process
}

CNeOper::~CNeOper()
{
	CloseRsp();
}

void CNeOper::CreateRspThread()
{
	unsigned threadID;
	m_bRspOpen = FALSE;
	m_hRspThread = (HANDLE)_beginthreadex( NULL, 0, &FunRspThread, this, 0, &threadID );
	//等待线程运行
	Sleep(200);
}

unsigned CNeOper::FunRspThread( void* _this )
{
	CNeOper* pNeOper = (CNeOper*)_this;
	pNeOper->m_bRspOpen = TRUE;
	while(pNeOper->m_bRspOpen)
	{
		CComNeMsg* pComNeMsg = NULL;
		pNeOper->m_obNeClient.ReadAndPopPack(&pComNeMsg);
		if (pComNeMsg == NULL)
		{
			Sleep(10);
			continue;
		}

		string strHeadTag = "";
		strHeadTag.insert(0, pComNeMsg->m_obFaceHeader.szHeader, 4);
		if ( PACK_KEY_WORD != strHeadTag )
		{
			delete pComNeMsg;
			continue;
		}

		char* pMsg = pComNeMsg->GetMsg();
		UINT uSize = pComNeMsg->GetTrulyLen();

		switch (pComNeMsg->m_obFaceHeader.usCmdType)
		{
		case CMD_CONNECT_NE:
			{
				pNeOper->m_bIsNeConn = *((BOOL*)pMsg);
			}
			break;
		case CMD_DISCONNECT_NE://断开网元回应消息
			{
				pNeOper->m_EventDesconn.SetEvent();	
			}
			break;
		case CMD_SEND_NE_CMD://得到网元命令执行结果
			{
				pNeOper->m_strResult.resize(uSize, 0x00);
				memcpy((char*)pNeOper->m_strResult.c_str(), pMsg, uSize);
				pNeOper->m_EventSendCmd.SetEvent();
			}
			break;
		case CMD_HOST_IP_GROUP://得到主机IP组
			{
				pNeOper->m_strIpGroup.resize(uSize+1, 0x00);
				memcpy((char*)pNeOper->m_strIpGroup.c_str(), pMsg, uSize);
				pNeOper->m_EventGetIP.SetEvent();
			}
			break;
		case CMD_NE_IP_GROUP://得到所有网关网元IP
			{
				string strGwAllIp;
				strGwAllIp.resize(uSize+1, 0x00);
				memcpy((char*)strGwAllIp.c_str(), pMsg, uSize);
				Split(strGwAllIp, "\n", pNeOper->m_vStrGwAllIp);
				
				pNeOper->m_EventGwAllIp.SetEvent(); 
			}
			break;
		case CMD_HEART_THROB://得到心跳包
			{
				if (uSize == sizeof(BOOL)*2)
				{
					pNeOper->m_bIsNeConn = *(BOOL*)pMsg;
					pNeOper->m_bIsNeLogin = *(BOOL*)(pMsg + sizeof(BOOL));
				}
			}
			break;
		case  CMD_PY_EXE_MOD:
			{
				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
				pWnd->PostMessage(MSG_MENU_ENABLE, 0, 1);
			}
			break;
		case CMD_PY_EXE_MOD_RESULT://得到Py模块执行后返回的结果
			{
				string strRes;
				strRes.resize(uSize, 0x00);
				memcpy((char*)strRes.c_str(), pMsg, uSize);
				pNeOper->m_strPyResult = strRes.c_str();
				pNeOper->m_EventPyResult.SetEvent();
			}
			break;
		case CMD_PY_DEBUG:
			{
				string strRes;
				strRes.resize(uSize + 1, 0x00);
				memcpy((char*)strRes.c_str(), pMsg, uSize);

			
 				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
 				CYinAidanceKitView*  pOutputDebug = (CYinAidanceKitView*)pWnd->m_pEditView;
 				pOutputDebug->AddTextDebug((char*)strRes.c_str());
			}
			break;
		case CMD_PY_PRINT:
			{
				string strRes;
				strRes.resize(uSize + 1, 0x00);
				memcpy((char*)strRes.c_str(), pMsg, uSize);

				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
				COutputView* pOutputView = (COutputView*)pWnd->m_pOutputView;
				pOutputView->AddTextPrint((char*)strRes.c_str());
			}
			break;
		case CMD_PY_DEBUG_CLEAR:
			{
				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
 				CYinAidanceKitView*  pOutputDebug = (CYinAidanceKitView*)pWnd->m_pEditView;
				pOutputDebug->ClearTextDebug();
			}
			break;
		case CMD_PY_PRINT_CLEAR:
			{
				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
				COutputView* pOutputView = (COutputView*)pWnd->m_pOutputView;
				pOutputView->ClearTextPrint();
			}
			break;
		case CMD_FILE_DIALOG:
			{
				string strFilter = pMsg;
				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
				string strFileName = pWnd->FileDialog(strFilter);
				pNeOper->m_obNeClient.Send(strFileName.c_str(), CMD_FILE_DIALOG, strFileName.length() + 1);
			}
			break;
		case CMD_FOLDER_DIALOG:
			{
				CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
				string strPath = pWnd->FolderDialog();
				pNeOper->m_obNeClient.Send(strPath.c_str(), CMD_FOLDER_DIALOG, strPath.length() + 1);
			}
			break;
		}


		//在这里写命令日志
		//OutputDebugString(strTmpMsg.c_str());
		Sleep(3);

		delete pComNeMsg;
	}

	_endthreadex( 0 );
	return 0;
}


void CNeOper::CloseRsp()
{
	m_obNeClient.CloseClt();
	if( m_bRspOpen == TRUE )
	{
		m_bRspOpen = FALSE;
		WaitForSingleObject(m_hRspThread, INFINITE);
		CloseHandle( m_hRspThread );
		//CloseHandle( m_hEvent );
	}

	CComNeMsg* pComNeMsg;
	m_obNeClient.ReadAndPopPack(&pComNeMsg);
	while (pComNeMsg != NULL)
	{
		delete pComNeMsg;
		m_obNeClient.ReadAndPopPack(&pComNeMsg);
		continue;
	}
}


BOOL CNeOper::CreateAndConnSrv(const char* pStrIP)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.lpTitle = "AsonSrv";
	if ( g_obAppCfg.GetShowConsole() == 0 )
	{
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW;
	}
	
	UINT uPort = 12500;
	CString strCmdLine;
	strCmdLine.Format("AsonSrv.dll %d",uPort);
 	CreateProcess(NULL, strCmdLine.LockBuffer(), NULL, NULL, FALSE, 0,
 		NULL, NULL, &si, &pi);
	
	//TerminateProcess(pi.hProcess, 0);
	Sleep(300);
	m_obNeClient.CreateConn("127.0.0.1", uPort);

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	CreateRspThread();

	//等待连接成功
	int iLoop = 0;
	while(IsSrvConn() == FALSE && iLoop < 100)
	{
		Sleep(30);
		iLoop ++;
	}

	return m_obNeClient.m_bIsConnSrv;
}


UINT CNeOper::Connect(string strNeIP, UINT iPort/* = 1400*/, ULONG ulNeID/* = 0*/)
{
	CONN_STRUCT obConnStruct;
	ZeroMemory(&obConnStruct, sizeof(CONN_STRUCT));
	memcpy(obConnStruct.strIP, strNeIP.c_str(), strNeIP.length());
	obConnStruct.iPort = iPort;
	obConnStruct.ulNeID = ulNeID;
	m_obNeClient.Send((char*)&obConnStruct, CMD_CONNECT_NE, sizeof(CONN_STRUCT));
	int iCount = 0;
	while( IsNeConn() == FALSE )
	{
		iCount ++;
		Sleep(30);
		if(iCount > 20)
		{
			return 1;
		}	
	}
	return 0;
}

void CNeOper::ExitSrv()
{
	m_obNeClient.Send("", CMD_EXIT_SERVER, 0);
}

//网元文件操作
// void CNeOper::UploadFile(string strNEFileAllName, string strLocalFileAllName)
// {
// 
// }
// 
// void CNeOper::DownloadFile(string strLocalFileAllName, string strNEFileAllName)
// {
// 
// }
// 
// void CNeOper::SetNeID( UINT ulNeID )
// { 
// 	m_obTcpClient.SetNeID(ulNeID); 
// }


void CNeOper::SendHeart()
{
	m_obNeClient.Send("", CMD_HEART_THROB, 0);	
}

void CNeOper::SetNeID( UINT ulNeID )
{
	m_obNeClient.Send((char*)&ulNeID, CMD_NE_ID, sizeof(UINT));	
}

void CNeOper::SendAllGip(string strGip, vector<string>& vStrGwAllIp)
{
	m_obNeClient.Send(strGip.c_str(), CMD_NE_IP_GROUP, strGip.length());
	m_EventGwAllIp.WaitEvent();
	vStrGwAllIp = m_vStrGwAllIp;
	m_vStrGwAllIp.clear();
}

// BOOL IsLogin()
// { 
// 	return m_bIsLogin; 
// }

UINT CNeOper::GetHostIP( vector<string>& vStrIP )
{
	m_strIpGroup = "";
	m_obNeClient.Send("", CMD_HOST_IP_GROUP, 0);
	DWORD iErr = m_EventGetIP.WaitEvent(3000);
	if(WAIT_TIMEOUT == iErr || m_strIpGroup == "")
	{
		return 1;
	}
	
	Split(string(m_strIpGroup.c_str()), ";", vStrIP);
	return 0;
}


UINT CNeOper::SendCmd(string strCmd, string& strRsp, UINT uiTimeout/* = 20000*/)
{
	m_strResult = "";
	UINT uSize = sizeof(UINT) + strCmd.length();
	char* pBufCmd = new char[uSize];
	memcpy(pBufCmd, &uiTimeout, sizeof(UINT));
	memcpy(pBufCmd + sizeof(UINT), strCmd.c_str(), strCmd.length());
	m_obNeClient.Send(pBufCmd, CMD_SEND_NE_CMD, uSize);
	delete[] pBufCmd;

	DWORD iErr = m_EventSendCmd.WaitEvent(uiTimeout);
	if(WAIT_TIMEOUT == iErr)
	{
		strRsp = "Cmd Timeout!";
		return 1;
	}
	strRsp = m_strResult;
	m_strResult = "";
	return 0;	
}

void CNeOper::Disconnect(UINT uTimeout/* = 10000*/)
{
	m_obNeClient.Send("", CMD_DISCONNECT_NE, 0);
	m_EventDesconn.WaitEvent(uTimeout);
}


UINT CNeOper::PyGetResult(char* pModName, string& strRelust, UINT uTimeout/* = 30000*/)
{
	m_strPyResult = "";
	UINT iErr = m_obNeClient.Send( pModName, CMD_PY_EXE_MOD_RESULT, strlen(pModName) + 1 );
	m_EventPyResult.WaitEvent(uTimeout);
	strRelust = m_strPyResult;
	m_strPyResult = "";
	return iErr;
}

void CNeOper::PySetArgs(string strArgs)
{
	m_obNeClient.Send(strArgs.c_str(), CMD_PY_SET_MOD_ARGS, strArgs.length() + 1);
}

void CNeOper::PyExeModule(const char* pModName)
{
	m_obNeClient.Send(pModName, CMD_PY_EXE_MOD, strlen(pModName) + 1);
}