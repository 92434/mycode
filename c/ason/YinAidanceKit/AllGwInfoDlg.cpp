// AllGwInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yinaidancekit.h"
#include "AllGwInfoDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllGwInfoDlg dialog


CAllGwInfoDlg::CAllGwInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllGwInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllGwInfoDlg)
	m_strPwd = _T("");
	m_strUser = _T("");
	m_strTimer = _T("");
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
}


void CAllGwInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllGwInfoDlg)
	DDX_Control(pDX, IDC_LOCAL_IP_COMBO, m_obComLocalIp);
	DDX_Text(pDX, IDC_PWD_EDIT, m_strPwd);
	DDX_Text(pDX, IDC_USER_EDIT, m_strUser);
	DDX_Text(pDX, IDC_TIMER_EDIT, m_strTimer);
	DDV_MaxChars(pDX, m_strTimer, 12);
	DDX_Text(pDX, IDC_TITLE_EDIT, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllGwInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAllGwInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllGwInfoDlg message handlers

BOOL CAllGwInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMainFrame* pFrmWnd = (CMainFrame*)AfxGetMainWnd();

	//1.本机IP Combo控件初始化
	vector<string> vStrHostIP;
	pFrmWnd->m_obNeOper.GetHostIP(vStrHostIP);
	for (int i=0; i<vStrHostIP.size(); i++)
	{
		m_obComLocalIp.InsertString(i, vStrHostIP[i].c_str());
	}
	m_obComLocalIp.SetCurSel(0);
	m_obComLocalIp.SelectString(0, g_obAppCfg.GetCfgInfo("GATEWAY_INFO", "LocalIp").c_str());
	
	m_strPwd = g_obAppCfg.GetCfgInfo("GATEWAY_INFO", "Password").c_str();
	m_strUser = g_obAppCfg.GetCfgInfo("GATEWAY_INFO", "Account").c_str();
	m_strTimer = g_obAppCfg.GetCfgInfo("GATEWAY_INFO", "Cycle").c_str();
	m_strTitle = g_obAppCfg.GetCfgInfo("GATEWAY_INFO", "Title").c_str();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAllGwInfoDlg::OnOK() 
{
	UpdateData(TRUE);
	m_obComLocalIp.GetWindowText(m_strLocalIp);
	if( m_strLocalIp == "" || m_strUser == "" || m_strPwd == "" || m_strTimer == "" || m_strTimer == "0")
	{	
		AfxMessageBox( "输入数据不能为空" );
		return;
	}
	
	CDialog::OnOK();
}
