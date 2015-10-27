// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "YinAidanceKit.h"

#include "YinAidanceKitDoc.h"
#include "MainFrm.h"
#include "OutputView.h"
#include "YinAidanceKitView.h"

#include "UserLoginDlg.h"
#include "AssistantDlg.h"
#include "AllCollecDlg.h"
#include "NePointDlg.h"

#include "AllGwInfoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
//ON_COMMAND(ID_MENU_FILE_UPLOAD, OnMenuTest)	//ID_MENU_TEST
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_LOGIN, OnMenuLogin)
	ON_COMMAND(ID_MENU_LOGOUT, OnMenuLogout)
	ON_UPDATE_COMMAND_UI(ID_MENU_LOGOUT, OnUpdateMenuLogout)
	ON_COMMAND(ID_NE_SUBSIDIARY, OnNeSubsidiary)
	ON_COMMAND(ID_MENU_ITEM_ALL_COLLEC, OnMenuItemAllCollec)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MENU_ITEM_ALL_COLLEC, OnUpdateMenuItemAllCollec)
	ON_WM_DESTROY()
	ON_COMMAND(ID_RUN_STOP, OnRunStop)
	ON_UPDATE_COMMAND_UI(ID_RUN_STOP, OnUpdateRunStop)
	ON_MESSAGE(MSG_MENU_ENABLE, OnMenuEnable)
	ON_COMMAND(ID_MENU_ITEM_ASONLOG_ANALYSE, OnMenuItemAsonlogAnalyse)
	ON_UPDATE_COMMAND_UI(ID_MENU_ITEM_ASONLOG_ANALYSE, OnUpdateMenuItemAsonlogAnalyse)
	ON_COMMAND(ID_MENU_ALL_GW_INFO, OnMenuAllGwInfo)
	ON_UPDATE_COMMAND_UI(ID_MENU_ALL_GW_INFO, OnUpdateMenuAllGwInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
// 	vector<string> vStrAgr;
// 	string strFlied = "stststsss";
// 	Split(strFlied, "s", vStrAgr);
// 	int a = vStrAgr.size();



	CYinSocket::StartupWSA();
	m_pEditView = NULL;
	m_pOutputView = NULL;
	m_bMenuEnable = TRUE;

	m_strNeID = "0";

}

CMainFrame::~CMainFrame()
{
	KillTimer( m_iHeartTimer );
// 	#if PYTHON_EX
// 	m_obPythonEx.ReleasePython();
// 	#endif
	CYinSocket::StopWSA();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//添加ToolBar按钮文字
	InitToolBar();

	CString dd = GetFileVer();

	return 0;
}

void CMainFrame::InitToolBar()
{
	CToolBarCtrl &ctrlToolBar = m_wndToolBar.GetToolBarCtrl();
	const int    iCount       = ctrlToolBar.GetButtonCount();

// 	CImageList SSD;
// 	SSD.Create(32, 24, ILC_COLOR32, 0, 4);
// 	CBitmap bm;
// 	bm.LoadBitmap(IDB_BITMAP_PYTHON);
// 	SSD.Add(&bm, RGB(0, 0, 0));
// 	HIMAGELIST hImageList = SSD.GetSafeHandle();
// 	m_wndToolBar.SetBitmap(hImageList);
	
	for (int i = 0; i < iCount; ++i)
	{
	   UINT nID    = 0;
	   UINT nStyle = 0;
	   int  iImage = 0;
	   m_wndToolBar.GetButtonInfo(i, nID, nStyle, iImage);
	   if (0 == nID)
	   {
		   continue;
	   }
   
	   switch (nID)
	   {
	   case ID_MENU_LOGIN:
			m_wndToolBar.SetButtonText(i, "Login");
	   		break;
	   case ID_MENU_LOGOUT:
			m_wndToolBar.SetButtonText(i, "Logout");
			break;
	   case ID_NE_SUBSIDIARY:
			m_wndToolBar.SetButtonText(i, "Script");
			//m_wndToolBar.SetButtonInfo(i, nID, nStyle, );
	   		break;
	   case ID_RUN_STOP:
		   m_wndToolBar.SetButtonText(i, "Stop");
	   	break;
	   }
	} 

	//m_wndToolBar.SetButtonText(1, "登出");

	CRect rcImage;
	CImageList *pImageList = ctrlToolBar.GetImageList();
	IMAGEINFO  infoImage;
	pImageList->GetImageInfo(0, &infoImage);
	rcImage = infoImage.rcImage;

	CRect temp;
    m_wndToolBar.GetItemRect(0,&temp);
    m_wndToolBar.SetSizes(CSize(temp.Width(), temp.Height()), CSize(rcImage.Width(), rcImage.Height()));

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	//开启服务
	StartServer();

	if (!m_obSplitterWnd.CreateStatic(this, 2, 1))
    {
        TRACE0("Failed to CreateStaticSplitter\n");
        return FALSE;
    }

	CRuntimeClass* pCmdRuntime  = RUNTIME_CLASS(COutputView);
    if (!m_obSplitterWnd.CreateView(0, 0,
        pCmdRuntime, CSize(0, 300), pContext))
    {
        TRACE0("Failed to create first pane\n");
        return FALSE;
    }

    if (!m_obSplitterWnd.CreateView(1, 0,
        pContext->m_pNewViewClass, CSize(0, 0), pContext))
    {
        TRACE0("Failed to create first pane\n");
        return FALSE;
    }
	//static_cast<>
 	m_pOutputView = (COutputView*)m_obSplitterWnd.GetPane(0,0);
	m_pEditView = (CYinAidanceKitView*)m_obSplitterWnd.GetPane(1,0);	
	
	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

}

// void CMainFrame::hh()
// {
// 	CCreateContext context;   //将文挡和视图相连 
// 	context.m_pCurrentDoc=pOldActiveView->GetDocument(); 
// 	pNewActiveView->Create(NULL, NULL, WS_BORDER|WS_CHILD , 
// 					   CFrameWnd::rectDefault, this, nForm, &context); 
// 
// 
// 	pNewActiveView->OnInitialUpdate(); 
// 	} 
// 	SetActiveView(pNewActiveView); 
// 
// }


void CMainFrame::OnMenuTest() 
{
//	m_obPythonEx.SendMSG();
	//1.上载示例
// 	m_obNeOperate.DownloadFile("c:\\text.txt", "/ofs1/hwx/YIN.TEX");
// 	return;

//	StartServer();	
//	ConnectAndLoginNe();
}


void CMainFrame::StartServer()
{
	KillTimer(m_iLoginTimer);
	KillTimer(m_iHeartTimer);

	m_obNeOper.ExitSrv();
	Sleep(100);

	m_obNeOper.CloseRsp();
	Sleep(100);

	m_obNeOper.CreateAndConnSrv("127.0.0.1");
	
	if(m_obNeOper.IsSrvConn() == FALSE)
	{
		MessageBox("连接服务出错，系统退出！", "系统错误", MB_ICONSTOP);
		OnClose();
		exit(0);
	}
	m_iHeartTimer = SetTimer(2, 1000, 0);
}


void CMainFrame::ConnectAndLoginNe()
{
	m_obNeOper.Disconnect();
	UINT ulNeID = 0;
	Str2HexNeId(m_strNeID, ulNeID);

	COutputView* pOutputView = (COutputView*)m_pOutputView;
	if (m_strNeIP == "")
	{
		pOutputView->AddTextPrint("Connect NE Faild!!!\r\n" );
		return;
	}
	if ( m_obNeOper.Connect(m_strNeIP, m_iPort, ulNeID) != 0)
	{	
		pOutputView->AddTextPrint("Connect NE Faild!!\r\n" );
		return;
	}
	
	CString strCmd;
	strCmd.Format(":lognew:\"%s\",\"%s\"", m_strName.c_str(), m_strPWD.c_str());
	ExeCmd(strCmd, FALSE);
	m_iLoginTimer = SetTimer(1, 2000, 0);
}


void CMainFrame::OnMenuTest1() 
{
	//2.下载示例
// 	m_obNeOperate.UploadFile("/ofs1/hwx/NE.INI", "c:\\text.txt");//mml/ddd.TXT    WDMBDCHK.INI 
// 	m_pOutputView->AddText("下载完成！！！！！");
// 	return;

	#if PYTHON_EX
//	m_obPythonEx.ExePyModule("yintest");
//	m_obPythonEx.ExePyModuleAsy("yintest");
	//m_obPythonEx.ExePyModule("NeAssistant.wen");
	#else
// 	string strRsp;
// 	m_obNeOperate.SendCmd(":cfg-get-phybd", strRsp);
// 	m_pOutputView->AddText((char*)strRsp.c_str());
	#endif
}

string CMainFrame::ExeCmd(const char* strCmd, BOOL isPrint/* = TRUE*/)
{
	CString strObCmd = strCmd;
	string strRsp;

	strObCmd.TrimRight();
	int iStart = strObCmd.Find("connect ");

	if (FALSE == m_obNeOper.IsNeConn() && strObCmd != "clear" && iStart != 0)
	{
		if (isPrint == TRUE)
		{
			strRsp = "No Connect NE!!!!\r\n";
			string strCmdTmp = strCmd;
			strCmdTmp += "\r\n";
			strRsp = strCmdTmp + strRsp;
			COutputView* pOutputView = (COutputView*)m_pOutputView;
			pOutputView->AddTextPrint((char*)strRsp.c_str());
		}
		return strRsp;
	}


	if (strCmd[0] == ':')
	{	
		if (strObCmd == ":logout")
		{
			KillTimer(m_iLoginTimer);
		}
// 		else if(strObCmd.Find(":lognew") == 0)
// 		{
// 			m_iLoginTimer = SetTimer(1, 2000, 0);
// 		}
		m_obNeOper.SendCmd(strObCmd.LockBuffer(), strRsp);
	} 
	else
	{
		strRsp = LocalCmd(strCmd);
	}

	if (isPrint == TRUE)
	{
		string strCmdTmp = strCmd;
		strCmdTmp += "\r\n";
		strRsp = strCmdTmp + strRsp;
		COutputView* pOutputView = (COutputView*)m_pOutputView;
		pOutputView->AddTextPrint((char*)strRsp.c_str());
	}
	return strRsp;
}

string CMainFrame::LocalCmd(const char* pCmd)
{
	CString strTmpCmd = pCmd;
	CString strCmd = pCmd;
	strCmd.MakeLower();
	//clear
	if (strCmd == "clear")
	{
		COutputView* pOutputView = (COutputView*)m_pOutputView;
		pOutputView->ClearTextPrint();
		return "";
	} 
	//disconnect
	if (strCmd == "disconnect")
	{
		m_obNeOper.Disconnect();
		return "";
	}
	//connect
	int iStart = strCmd.Find("connect ");
	if ( iStart == 0)
	{
		string strIP = strCmd.Mid(8);
		m_obNeOper.Disconnect();
		m_obNeOper.Connect(strIP);
		if(m_obNeOper.IsNeConn() == TRUE)
		{
			return "Connect succeed!\r\n";
		}
		return "Connect faild!\r\n";
	}
	//tone #9-40028:lognew:"yinwen","@ywh12345"
 	iStart = strCmd.Find("tone #");
 	if ( iStart == 0)
 	{
		int iIdPos = strCmd.Find(":", 6);
		if (iIdPos < 6) return "Cmd error!\r\n";
	
		string strNeID = strCmd.Mid( 6, iIdPos - 6);
		string strLogNew = strTmpCmd.Mid( iIdPos );//strCmd.Mid( iIdPos );

		UINT ulNeID;
		BOOL bErr = Str2HexNeId(strNeID, ulNeID);
		if (bErr == FALSE) return "Cmd error!\r\n";

		m_obNeOper.SetNeID(ulNeID);
		
		string strRsp;
		KillTimer(m_iLoginTimer);
		m_obNeOper.SendCmd(strLogNew, strRsp);
		UINT uCount = 0;
		while( uCount < 30 )
		{
			Sleep(100);
			if(m_obNeOper.IsNeConn() && m_obNeOper.IsNeLogin())
			{
				break;
			}
			uCount++;
		}
		m_iLoginTimer = SetTimer(1, 2000, 0);

 		return strRsp;
 	}

	return "Error Cmd!!\r\n\r\n";
}


void CMainFrame::OnMenuLogin() 
{
// 	m_obNeOper.CreateAndConnSrv("127.0.0.1");
// 	m_iHeartTimer = SetTimer(2, 1000, 0);
// 	m_obNeOper.Connect("129.9.68.35", 1400, 0);
	
	CUserLoginDlg obUserLoginDlg;
	int iResult = obUserLoginDlg.DoModal();
	if (iResult == IDOK)
	{
		m_obNeOper.Disconnect();
		UINT ulNeID = 0;
		Str2HexNeId(obUserLoginDlg.m_strNeID.LockBuffer(), ulNeID);
		if( m_obNeOper.Connect(obUserLoginDlg.m_strNeIP.LockBuffer(), obUserLoginDlg.m_iPort, ulNeID) != 0 )
		{
			COutputView* pOutputView = (COutputView*)m_pOutputView;
			pOutputView->AddTextPrint("Connect NE Faild!\r\n");
			return;
		}

		m_strName = obUserLoginDlg.m_strName.LockBuffer();
		m_strPWD = obUserLoginDlg.m_strPWD.LockBuffer();
		m_strNeID = obUserLoginDlg.m_strNeID.LockBuffer();
		m_strNeIP = obUserLoginDlg.m_strNeIP.LockBuffer();
		m_iPort = obUserLoginDlg.m_iPort;
		Sleep(100);
		UserLogin();
	}	
}

void CMainFrame::UserLogin()
{
	CString strLoginCmd;
	strLoginCmd.Format(":lognew:\"%s\",\"%s\"", m_strName.c_str(), m_strPWD.c_str());
	
	string strRsp;
	m_obNeOper.SendCmd(strLoginCmd.LockBuffer(), strRsp);
//	m_pOutputView->AddText((char*)strRsp.c_str());
	if (strRsp.find("LOGNEW  : Last log time") != string::npos || strRsp.find("LOGIN  : Last log time") != string::npos)
	{
		int iStart = strRsp.find("\n#");
		int iEnd = strRsp.find(":", iStart);

		string strNeID = strRsp.substr(iStart + 2, iEnd - iStart - 2);
		
		CString strPrintInfo;
		strPrintInfo.Format("Login NE%s success!!\r\n\r\n", strNeID.c_str());
		m_pOutputView->AddTextPrint(strPrintInfo.LockBuffer());
		SetMenuEnable(TRUE);
		m_iLoginTimer = SetTimer(1, 2000, 0);
	}
	else
	{
		m_pOutputView->AddTextPrint("Account logout!!\r\n\r\n");
		//m_pEditView->m_obBottomDlg.EnableWin(FALSE);
	}
}

void CMainFrame::OnMenuLogout() 
{
	KillTimer(m_iLoginTimer);
	string strRsp;
	m_obNeOper.SendCmd(":logout", strRsp);
	m_obNeOper.Disconnect();
	m_pOutputView->AddTextPrint("Account logout!!\r\n\r\n");	
}

void CMainFrame::OnUpdateMenuLogout(CCmdUI* pCmdUI) 
{
 	if ( m_obNeOper.IsNeConn() && m_obNeOper.IsNeLogin() )
 	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}	
}

void CMainFrame::OnUpdateMenuItemAllCollec(CCmdUI* pCmdUI) 
{
	if ( m_obNeOper.IsNeConn() && m_obNeOper.IsNeLogin() && m_bMenuEnable )
	{
		pCmdUI->Enable( TRUE );
 	}
 	else
 	{
 		pCmdUI->Enable( FALSE );
 	}
}


void CMainFrame::OnNeSubsidiary() 
{
	CAssistantDlg obAssistantDlg;
	if (obAssistantDlg.DoModal() == IDOK)
	{
// 		#if PYTHON_EX
// 			m_obPythonEx.ExePyModule("NeAssistant.wen");
// 		#endif
	}
}

//打开选择文件对话框
string CMainFrame::FileDialog(string& strFlied)
{
	vector<string> vStrAgr;
	Split(strFlied, "\n", vStrAgr);

	if (vStrAgr.size() != 3)
	{
		return "";
	}

	string strSign = vStrAgr[0];
	string strDefName = vStrAgr[1];
	string strFlieds = vStrAgr[2];
	BOOL bSign = TRUE;
	if (strSign == "0")
	{
		bSign = FALSE;
	}

	const char* pDefName = NULL;
	if (strDefName != "")
	{
		pDefName = strDefName.c_str();
	}

	if (strFlieds == "")
	{
		strFlieds= "All Files (*.*)|*.*||";
	}

	CFileDialog fileDlg (bSign, NULL, pDefName,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFlieds.c_str(), this);

	if( fileDlg.DoModal() == IDOK )
	{
		string strFileAllName = "";
		fileDlg.GetPathName( ).LockBuffer();

		POSITION iPos = fileDlg.GetStartPosition();
		while (iPos != NULL)
		{
			fileDlg.GetPathName( ).LockBuffer();
			strFileAllName += fileDlg.GetNextPathName(iPos).LockBuffer();
			strFileAllName += "\n";
		}

		strFileAllName = TrimRight(strFileAllName, "\n");
		return strFileAllName;
	}

	return "";
}


//选择文件夹对话框
string CMainFrame::FolderDialog()
{
	CString str;
	BROWSEINFO bi;
	char sName[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();              //调用该对话框的父窗口句柄
	bi.pszDisplayName = sName;                      //用来保存用户选中的目录字符串的内存地址。该缓冲区的大小缺省是定义的MAX_PATH常量宏。
	bi.lpszTitle = "路径选择：";                   //该浏览文件夹对话框对话框的显示文本，用来提示该浏览文件夹对话框的功能、作用和目的。
	
	//bi.ulFlags = BIF_USENEWUI;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl == NULL)
		return "";
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer(); 
	return str.LockBuffer();
}



CString CMainFrame::GetFileVer()
{
	char cPath[200];
	//	char szVersionBuffer[200];
	DWORD dwHandle,InfoSize;
	CString strVersion;
	::GetModuleFileName(NULL,cPath,sizeof(cPath)); //首先获得版本信息资源的长度
	InfoSize = GetFileVersionInfoSize(cPath,&dwHandle); //将版本信息资源读入缓冲区
	if(InfoSize==0) return _T("None VerSion Supprot");
	char *InfoBuf = new char[InfoSize];
	GetFileVersionInfo(cPath,0,InfoSize,InfoBuf); //获得生成文件使用的代码页及文件版本
	unsigned int  cbTranslate = 0;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
	// Read the file description for each language and code page.
	for( int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
	{
		char  SubBlock[200];
		wsprintf( SubBlock,
            TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
            lpTranslate[i].wLanguage,
            lpTranslate[i].wCodePage);
		void *lpBuffer=NULL;
		unsigned int dwBytes=0;
		VerQueryValue(InfoBuf,
			SubBlock,
			&lpBuffer,
			&dwBytes);
		CString strTemp=(char *)lpBuffer;
		strVersion+=strTemp;
		
	}
	return strVersion;
}



// LRESULT CMainFrame::OnUploadFileMsg(WPARAM wParam, LPARAM lParam)
// {
// 	m_pOutputView->AddTextPrint("上载完成!!!\r\n\r\n");	
// 	return 0;
// }

void CMainFrame::OnMenuItemAllCollec() 
{
	CAllCollecDlg obAllCollec;
	if ( IDOK != obAllCollec.DoModal() )
	{
		return;
	}

	CString strOper = "";
	CString strNeIdList;
	if( 1 == obAllCollec.m_iRadioType)
	{
		CNePointDlg obNePointDlg;
		strOper =  obAllCollec.m_strComboGroup;
		obNePointDlg.GetNeGroup(strOper);
		if ( obAllCollec.m_strComboGroup == "" || IDOK != obNePointDlg.DoModal() )
		{
			return;
		}
		
		strNeIdList = obNePointDlg.m_strNeIdList;
	} 
	else
	{
		strNeIdList = obAllCollec.m_strComboNeId;
	}
	strNeIdList.TrimRight(";");
	
	CString strAreaType;
	strAreaType.Format( "%d", obAllCollec.m_cOperArea );

	CString strContent;
	strContent.Format( "%d", obAllCollec.m_cContent );
	
	//string strArea = "Config.Py_Script.Py_DataCollecte.SubMain";
	string strArea = "Config.Py_Script.LspDataCollect.item2";

	if(strNeIdList != "")
	{
		//命令类型  ； 业务类型   ； 采集内容；   网元ID列表
		CString strArgs = strAreaType + ";" + strOper + ";" + strContent + ";" + strNeIdList;
		//AfxMessageBox(strArgs);
		m_obNeOper.PySetArgs(strArgs.LockBuffer());
		m_obNeOper.PyExeModule(strArea.c_str());
		SetMenuEnable(FALSE);
	}
}

void CMainFrame::SetMenuEnable(BOOL bEnable/* = TRUE*/)
{
	m_bMenuEnable = bEnable;
	m_pEditView->SetEditCmd(bEnable);
}

LRESULT CMainFrame::OnMenuEnable(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0)
	{
		SetMenuEnable(FALSE);
	}
	else
	{
		SetMenuEnable(TRUE);	
	}
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1:
		{
			if (FALSE == m_obNeOper.IsNeConn())
			{
				m_pOutputView->AddTextPrint("No Connect NE!!!\r\n\r\n");
				KillTimer(m_iLoginTimer);
			}
			else if ( FALSE == m_obNeOper.IsNeLogin())
			{
				m_pOutputView->AddTextPrint("Account logout!!!\r\n\r\n");
				UserLogin();
			}	
		}
		break;
	case 2:	//给服务端发送心跳
		{
			if( TRUE == m_obNeOper.IsConnSrv() )
			{
				m_obNeOper.SendHeart();
			}
			else
			{
				KillTimer(m_iHeartTimer);
				MessageBox("AsonSrv服务被强制关闭，系统将退出！", "系统错误", MB_ICONSTOP);
				OnClose();
			}
		}
		break;
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnDestroy() 
{
	KillTimer(m_iLoginTimer);
	KillTimer(m_iHeartTimer);
	m_obNeOper.ExitSrv();
	m_obNeOper.CloseRsp();

	CFrameWnd::OnDestroy();
}


void CMainFrame::OnRun()
{
	
}

void CMainFrame::OnRunStop() 
{
	RunStop();
}

void CMainFrame::RunStop()
{
	StartServer();
	ConnectAndLoginNe();
	SetMenuEnable(TRUE);
}

void CMainFrame::OnUpdateRunStop(CCmdUI* pCmdUI) 
{
	if (  !m_bMenuEnable )//m_obNeOper.IsNeConn() && && m_obNeOper.IsNeLogin()
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
 	}
}

void CMainFrame::OnMenuItemAsonlogAnalyse() 
{
	m_obNeOper.PyExeModule("Config.Py_Script.Py_QuestionAnalyse.Py_LogAnalyse.ParseServer");
	SetMenuEnable(FALSE);
}


void CMainFrame::OnUpdateMenuItemAsonlogAnalyse(CCmdUI* pCmdUI) 
{
	if ( m_bMenuEnable )
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
 	}
}


void CMainFrame::OnMenuAllGwInfo()
{
	CAllGwInfoDlg obAllGwInfoDlg;
	if( IDOK == obAllGwInfoDlg.DoModal() )
	{
		ExeCmd(":logout", FALSE);
		g_obAppCfg.WriteCfgInfo("GATEWAY_INFO", "LocalIp", obAllGwInfoDlg.m_strLocalIp.LockBuffer());
		g_obAppCfg.WriteCfgInfo("GATEWAY_INFO", "Cycle", obAllGwInfoDlg.m_strTimer.LockBuffer());
		g_obAppCfg.WriteCfgInfo("GATEWAY_INFO", "Title", obAllGwInfoDlg.m_strTitle.LockBuffer());
		g_obAppCfg.WriteCfgInfo("GATEWAY_INFO", "Account", obAllGwInfoDlg.m_strUser.LockBuffer());
		g_obAppCfg.WriteCfgInfo("GATEWAY_INFO", "Password", obAllGwInfoDlg.m_strPwd.LockBuffer());
		//格式：用户名\n密码\n本机IP\n周期\n标题
 		CString strInfo;
		strInfo.Format("%s\n%s\n%s\n%s\n%s", obAllGwInfoDlg.m_strUser, obAllGwInfoDlg.m_strPwd, obAllGwInfoDlg.m_strLocalIp, obAllGwInfoDlg.m_strTimer, obAllGwInfoDlg.m_strTitle);
 		m_obNeOper.PySetArgs(strInfo.LockBuffer());
 		m_obNeOper.PyExeModule("Config.Py_Script.Py_Timer.AllGatewayInfo");
		SetMenuEnable(FALSE);
	}
}

void CMainFrame::OnUpdateMenuAllGwInfo(CCmdUI* pCmdUI) 
{
	if ( m_bMenuEnable )
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
 	}
}
