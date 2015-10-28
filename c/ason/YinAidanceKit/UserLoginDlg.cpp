// UserLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "UserLoginDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog


CUserLoginDlg::CUserLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLoginDlg)
	//}}AFX_DATA_INIT
}


void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLoginDlg)
	DDX_Control(pDX, IDC_EDIT_PORT, m_obPortEdit);
	DDX_Control(pDX, IDC_USER_NAME_EDIT, m_obNameEdit);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_obPwdEdit);
	DDX_Control(pDX, IDC_NE_ID_EDIT, m_obEditNeID);
	DDX_Control(pDX, IDC_GATEWAY_IP_COMBO, m_obComboNeIp);
	DDX_Control(pDX, IDC_HOST_IP_COMBO, m_obComboHostIp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CUserLoginDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_HOST_IP_COMBO, OnSelchangeHostIpCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg message handlers

BOOL CUserLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMainFrame* pFrmWnd = (CMainFrame*)AfxGetMainWnd();

	//1.本机IP Combo控件初始化
	vector<string> vStrHostIP;
	pFrmWnd->m_obNeOper.GetHostIP(vStrHostIP);
	for (int i=0; i<vStrHostIP.size(); i++)
	{
		m_obComboHostIp.InsertString(i, vStrHostIP[i].c_str());
	}

	m_obComboHostIp.SetCurSel(0);
	m_obComboHostIp.SelectString(0, g_obAppCfg.GetLocalIp().c_str());
	OnSelchangeHostIpCombo();


	m_obEditNeID.SetWindowText("0");
	m_obPortEdit.SetWindowText("1400");


	m_obNameEdit.SetWindowText(g_obAppCfg.GetUserName().c_str());
	m_obPwdEdit.SetWindowText(g_obAppCfg.GetPwd().c_str());////nesoft//@ywh12345

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLoginDlg::AddNeIP(const char* strIP)
{	
	m_obComboNeIp.AddString(strIP);
	m_obComboNeIp.SetCurSel(0);
	m_obComboNeIp.SelectString(0, g_obAppCfg.GetNeIp().c_str());
}

void CUserLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CUserLoginDlg::OnOK() 
{	
	m_obComboNeIp.GetWindowText(m_strNeIP);
	m_obComboHostIp.GetWindowText(m_strHostIP);

	m_obEditNeID.GetWindowText(m_strNeID);
	m_obNameEdit.GetWindowText(m_strName);
	m_obPwdEdit.GetWindowText(m_strPWD);
	CString strPort;
	m_obPortEdit.GetWindowText(strPort);
	m_iPort = atoi(strPort.LockBuffer());

	g_obAppCfg.WriteLocalIp(m_strHostIP.LockBuffer());
	g_obAppCfg.WriteNeIp(m_strNeIP.LockBuffer());
	g_obAppCfg.WriteUserName(m_strName.LockBuffer());
	g_obAppCfg.WritePwd(m_strPWD.LockBuffer());

	CDialog::OnOK();
}

void CUserLoginDlg::OnSelchangeHostIpCombo() 
{
	m_obComboHostIp.GetWindowText(m_strHostIP);
	m_obComboNeIp.ResetContent();

	if (m_strHostIP == "")
	{
		return;
	}
	
// 	//1.查找网元IP
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	vector<string> vStrGwAllIp;
	pWnd->m_obNeOper.SendAllGip(m_strHostIP.LockBuffer(), vStrGwAllIp);

	for (int i=0; i<vStrGwAllIp.size(); i++)
	{
		m_obComboNeIp.AddString(vStrGwAllIp[i].c_str());
	}

	m_obComboNeIp.SetCurSel(0);
	m_obComboNeIp.SelectString(0, g_obAppCfg.GetNeIp().c_str());
}
