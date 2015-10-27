// SevFrame.cpp: implementation of the CSevFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SevFrame.h"
#include "FaceSvrHead.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSevFrame* CSevFrame::_this = NULL;

CSevFrame::CSevFrame()
{
	_this = this;
	//1.初始化Socket环境
	CTcpServer::StartupWSA();

	//3.创建PyEx对象
//	ObPythonEx;
	#if PYTHON_EX
	m_obPythonEx.CreatePython();
	#endif
}

CSevFrame::~CSevFrame()
{
	//1.释放Python
	#if PYTHON_EX
	m_obPythonEx.ReleasePython();
	#endif
	//2.停止socket环境
	CTcpServer::StopWSA();
}

void CSevFrame::CreateSrv(UINT uPort)
{
	m_obTcpServer.CreateServer(uPort, "0.0.0.0");
	m_obTcpServer.AcceptAndResv();

	CreateRspThread();
}

void CSevFrame::CloseSrv()
{
	m_NeOperate.CloseRsp();
	//1.关闭服务端
	m_obTcpServer.Close();
}


void CSevFrame::Run()
{
	//1.开始消息循环
	while(TRUE)
	{
		CComNeMsg* pPackMsg = NULL;
		m_obTcpServer.ReadPack(&pPackMsg);
		if (pPackMsg == NULL)
		{
			Sleep(100);
			continue;
		}
		
		UINT iCmdType = pPackMsg->m_obFaceHeader.usCmdType;
		UINT iSize = pPackMsg->GetTrulyLen();
		char* pMsg = pPackMsg->GetMsg();

		//cout<<pMsg<<"\r\n";

		BOOL bIsCmdType = TRUE;
		switch (iCmdType)
		{
		case CMD_PY_SET_MOD_ARGS://设置Python模块参数
			{
				m_obTcpServer.PopPack();
				#if PYTHON_EX
				m_obPythonEx.SetArgs(pMsg);
				#endif
			}
			break;
		case CMD_PY_EXE_MOD://执行Python模块
			{
				m_obTcpServer.PopPack();
				cout<<"ExePyModule="<<pMsg<<"\r\n";
				#if PYTHON_EX
				m_obPythonEx.ExePyModule(pMsg);
				#endif
				m_obTcpServer.Send("", CMD_PY_EXE_MOD, 0);
			}
			break;
		case CMD_PY_EXE_MOD_RESULT://设置Python模块,并得到结果
			{
				m_obTcpServer.PopPack();
				string strResult;
				#if PYTHON_EX	
				m_obPythonEx.GetPyResult(pMsg, strResult);
				#endif

				m_obTcpServer.Send(strResult.c_str(), CMD_PY_EXE_MOD_RESULT, strResult.length());
			}
			break;
		default:
			{
				bIsCmdType = FALSE;
			}
			break;
		}
		
		if (TRUE == bIsCmdType)
		{	
			delete pPackMsg;
		}

		Sleep(30);
	}
}



void CSevFrame::CreateRspThread()
{
	unsigned threadID;
	m_bRspOpen = TRUE;
	m_hRspThread = (HANDLE)_beginthreadex( NULL, 0, &FunRspThread, this, 0, &threadID );
	//等待线程运行
	Sleep(200);
}

unsigned CSevFrame::FunRspThread( void* _this )
{
	CSevFrame* pSevFrame = (CSevFrame*)_this;
	CTcpServer& obTcpSrv = pSevFrame->m_obTcpServer;
	CNeOperate& obNeOper = pSevFrame->m_NeOperate;
	while(pSevFrame->m_bRspOpen)
	{
		CComNeMsg* pPackMsg = NULL;
		obTcpSrv.ReadPack(&pPackMsg);
		if (pPackMsg == NULL)
		{
			if (pSevFrame->IsCltConn() == FALSE)
			{
				pSevFrame->CloseSrv();
				exit(0);
			}
			Sleep(100);
			continue;
		}
		
		UINT iCmdType = pPackMsg->m_obFaceHeader.usCmdType;
		UINT iSize = pPackMsg->GetTrulyLen();
		char* pBuf = pPackMsg->GetMsg();
		BOOL bIsCmdType = TRUE;
		switch (iCmdType)
		{
		case CMD_CONNECT_NE://连接网元
			{
				PCONN_STRUCT pConnStruct = (PCONN_STRUCT)pBuf;
				cout<<"IP="<<pConnStruct->strIP<<"\r\n";
				cout<<"Port="<<pConnStruct->iPort<<"\r\n";
				cout<<"NeID="<<pConnStruct->ulNeID<<"\r\n";
				
				obNeOper.Connect(pConnStruct->strIP, pConnStruct->iPort, pConnStruct->ulNeID);
				BOOL bIsConn = obNeOper.IsConn();
				obTcpSrv.Send((char*)&bIsConn, CMD_CONNECT_NE, sizeof(BOOL));
			}
			break;
		case CMD_DISCONNECT_NE://断开网元
			{
				obNeOper.Disconnect();
				obTcpSrv.Send("", CMD_DISCONNECT_NE, 0);
			}
			break;
		case CMD_SEND_NE_CMD://发送网元命令
			{
				UINT uTimeout = *(UINT*)pBuf;
				cout<<"Timeout="<<uTimeout<<"\r\n";
				string strCmd;
				strCmd.resize(iSize - sizeof(UINT) + 1, 0x00);
				memcpy((char*)strCmd.c_str(), pBuf + sizeof(UINT)  ,iSize - sizeof(UINT));
				string strRelult;
				cout<<"CMD="<<strCmd.c_str()<<"\r\n";
				obNeOper.SendCmd(strCmd.c_str(), strRelult, uTimeout, FALSE);
				//cout<<"Result="<<strRelult.c_str()<<"\r\n";
				//obTcpSrv.Send(strRelult.c_str(), CMD_SEND_NE_CMD, strRelult.length());
			}
			break;
		case CMD_HEART_THROB://发送心跳
			{
				BOOL bGroup[2];
				bGroup[0] = obNeOper.IsConn();
				bGroup[1] = obNeOper.IsLogin();
				obTcpSrv.Send((char*)bGroup, CMD_HEART_THROB, sizeof(BOOL)*2);	
			}
			break;
		case CMD_HOST_IP_GROUP://得到主机IP
			{
				string strIpGroup;
				vector<string> vStrHostIP;
				obNeOper.GetHostIP(vStrHostIP);
				int iSize = vStrHostIP.size();
				for (int i=0; i<iSize; i++)
				{
					strIpGroup += vStrHostIP[i] + ";";
				}
				if (iSize != 0)
				{
					strIpGroup.erase(strIpGroup.length() - 1);
				}
				obTcpSrv.Send(strIpGroup.c_str(), CMD_HOST_IP_GROUP, strIpGroup.length());
			}
			break;
		case CMD_NE_IP_GROUP://得到所有网关网元IP
			{
				string strLocalIp;
				strLocalIp.resize(iSize + 1, 0x00);
				memcpy((char*)strLocalIp.c_str(), pBuf, iSize);
				//1.查找网元IP
				string strAllNeIp;
				pSevFrame->m_obUdpClient.GetAllGipGroup(strLocalIp.c_str(), strAllNeIp);
				pSevFrame->m_obTcpServer.Send(strAllNeIp.c_str(), CMD_NE_IP_GROUP, strAllNeIp.length());
			}
			break;
		case CMD_EXIT_SERVER:
			{
				pSevFrame->CloseSrv();
				exit(0);
			}
			break;
		case CMD_NE_ID:
			{
				UINT* pNeID = (UINT*)pBuf;
				obNeOper.SetNeID(*pNeID);
			}
			break;
		case CMD_FILE_DIALOG:
			{
				//obNeOper. //m_FileDialogEvent.SetEvent();
				#if PYTHON_EX	
				pSevFrame->m_obPythonEx.m_strFilePath = pBuf;
				pSevFrame->m_obPythonEx.m_DialogEvent.SetEvent();
				#endif
			}
			break;
		case CMD_FOLDER_DIALOG:
			{
				//obNeOper. //m_FileDialogEvent.SetEvent();
				#if PYTHON_EX	
				pSevFrame->m_obPythonEx.m_strFilePath = pBuf;
				pSevFrame->m_obPythonEx.m_DialogEvent.SetEvent();
				#endif
			}
			break;
		default:
			{
				bIsCmdType = FALSE;
			}
			break;
		}
		
		if (TRUE == bIsCmdType)
		{	
			obTcpSrv.PopPack();
			delete pPackMsg;
		}

		Sleep(30);
	}
	
	//_endthreadex( 0 );
	return 0;
}


void CSevFrame::CloseRsp()
{
	if( m_bRspOpen == TRUE )
	{
		m_bRspOpen = FALSE;
		WaitForSingleObject(m_hRspThread, INFINITE);
		CloseHandle( m_hRspThread );
		//CloseHandle( m_hEvent );
	}
}


string CSevFrame::LocalCmd(const char* pCmd)
{
	string strTmpCmd = pCmd;
	string strCmd = ToLower(pCmd);

	//clear
	if (strCmd == "clear")
	{
		m_obTcpServer.Send("", CMD_PY_PRINT_CLEAR, 0);//清除Debug信息
		return "";
	} 
	//disconnect
	if (strCmd == "disconnect")
	{
		m_NeOperate.Disconnect();
		return "";
	}
	//connect
	int iStart = strCmd.find("connect ");
	if ( iStart == 0)
	{
		string strIP = strCmd.substr(8);
		//cout<<"strIP="<<strIP.c_str()<<"\r\n";
		m_NeOperate.Disconnect();
		m_NeOperate.Connect(strIP);
		if(m_NeOperate.IsConn() == TRUE)
		{
			return "Connect succeed!!!\r\n";
		}
		return "Connect faild!!!\r\n";	
	}
	//tone #9-40028:lognew:"yinwen","@ywh12345"
	iStart = strCmd.find("tone #");
	if ( iStart == 0)
	{
		int iIdPos = strCmd.find(":", 6);
		if (iIdPos < 6) return "Cmd error!\r\n";
		
		string strNeID = strCmd.substr( 6, iIdPos - 6);
		string strLogNew = strTmpCmd.substr( iIdPos );//strCmd.substr( iIdPos );
		
		UINT ulNeID;
		BOOL bErr = Str2HexNeId(strNeID, ulNeID);
		if (bErr == FALSE) return "Cmd error!\r\n";
		
		m_NeOperate.SetNeID(ulNeID);
		
		string strRsp;

		m_NeOperate.SendCmd(strLogNew, strRsp);


		return strRsp;
	}
	
	return "Error Cmd!!!\r\n\r\n";
}


