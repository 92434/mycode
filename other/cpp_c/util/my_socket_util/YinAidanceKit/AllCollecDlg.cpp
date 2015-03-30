// AllCollecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "AllCollecDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllCollecDlg dialog


CAllCollecDlg::CAllCollecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllCollecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllCollecDlg)
	m_iRadioType = 0;
	//}}AFX_DATA_INIT
}


void CAllCollecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllCollecDlg)
	DDX_Control(pDX, IDC_COMBO_THREE_GROUP, m_obComboThGroup);
	DDX_Control(pDX, IDC_COMBO_NEID, m_obComboNeid);
	DDX_Radio(pDX, IDC_RADIO_ONLY_SITE, m_iRadioType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllCollecDlg, CDialog)
	//{{AFX_MSG_MAP(CAllCollecDlg)
	ON_BN_CLICKED(IDC_RADIO_ONLY_SITE, OnRadioOnlySite)
	ON_BN_CLICKED(IDC_RADIO_OPERATION, OnRadioOperation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllCollecDlg message handlers


BOOL CAllCollecDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//CButton* pCheckButtonWdm = (CButton*)GetDlgItem(IDC_CHECK_WDM);
	//pCheckButtonWdm->SetCheck(1);

	CButton* pCheckCfgInfo = (CButton*)GetDlgItem(IDC_CHECK_CFG_INFO);
	pCheckCfgInfo->SetCheck(1);
	CButton* pCheckUpload = (CButton*)GetDlgItem(IDC_CHECK_UPLOAD);
	pCheckUpload->SetCheck(1);

	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	string strResult = pWnd->ExeCmd(":cfg-get-itgattrib", FALSE);
	if (string::npos == strResult.find("enable") )
	{
		CWnd* pRadioWnd = GetDlgItem(IDC_RADIO_OPERATION);
		pRadioWnd->EnableWindow(FALSE);
		m_obComboThGroup.EnableWindow(FALSE);
	}
	
	OnRadioOnlySite();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAllCollecDlg::OnOK() 
{
	m_obComboThGroup.GetWindowText(m_strComboGroup);
	m_obComboNeid.GetWindowText(m_strComboNeId);
	UpdateData(TRUE);

	CButton* pCheckButtonWdm = (CButton*)GetDlgItem(IDC_CHECK_WDM);
	CButton* pCheckButtonSdh = (CButton*)GetDlgItem(IDC_CHECK_SDH);
	CButton* pCheckButtonOtn = (CButton*)GetDlgItem(IDC_CHECK_OTN);

	m_cOperArea = 0;
	if ( pCheckButtonWdm->GetCheck() != 0 )
	{
		m_cOperArea = m_cOperArea | 4;
	}
	if ( pCheckButtonSdh->GetCheck() != 0)
	{
		m_cOperArea = m_cOperArea | 2;
	}
	if ( pCheckButtonOtn->GetCheck() != 0 )
	{
		m_cOperArea = m_cOperArea | 1;
	}

	if (m_cOperArea == 0)
	{
		MessageBox("请选择采集领域！", "提示", MB_ICONEXCLAMATION);
		//MessageBox("如果只上载日志文件，请选择确认；\r\n如需采集配置信息，请选择取消，返回上一级界面并勾选采集领域。", "提示", MB_ICONEXCLAMATION);
		return;
	}

	if( 1 == m_iRadioType)
	{
		if (m_strComboGroup == "")
		{	
			MessageBox("没有业务可采集！", "提示", MB_ICONEXCLAMATION);
			return;
		}
	}
	else
	{
		if (m_strComboNeId == "")
		{	
			MessageBox("没有网元可采集！", "提示", MB_ICONEXCLAMATION);
			return;
		}
	}

	CButton* pCheckCfgInfo = (CButton*)GetDlgItem(IDC_CHECK_CFG_INFO);
	CButton* pCheckUpload = (CButton*)GetDlgItem(IDC_CHECK_UPLOAD);

	m_cContent = 0;
	if ( pCheckCfgInfo->GetCheck() != 0 )
	{
		m_cContent = m_cContent | 1;
	}
	if ( pCheckUpload->GetCheck() != 0 )
	{
		m_cContent = m_cContent | 2;
	}

	if (m_cContent == 0)
	{
		MessageBox("请选择采集内容！", "提示", MB_ICONEXCLAMATION);
		return;
	}


	CDialog::OnOK();
}

void CAllCollecDlg::OnRadioOnlySite() 
{
	//m_obComboNeid.EnableWindow(TRUE);
	m_obComboNeid.ResetContent();

	string strResult;

	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->m_obNeOper.PyGetResult("Config.Py_Script.Py_DataCollecte.GetAllNeID", strResult);


	vector<string> vstrNeID;
	Split(strResult, ",", vstrNeID);
	int iSize = vstrNeID.size();
	for (int i=0; i<iSize; i++)
	{
		m_obComboNeid.AddString(vstrNeID[i].c_str());
	}

	m_obComboNeid.SetCurSel(m_obComboNeid.GetTopIndex());
}

void CAllCollecDlg::OnRadioOperation()
{
	//m_obComboNeid.EnableWindow(FALSE);
	//m_obComboThGroup.EnableWindow(TRUE);
	m_obComboThGroup.ResetContent();

	string strResult;

 	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
 	pWnd->m_obNeOper.PyGetResult("Config.Py_Script.Py_DataCollecte.GetAllLsp", strResult);

	
	vector<string> vstrNeID;
	Split(strResult, ";", vstrNeID);
	int iSize = vstrNeID.size();
	for (int i=0; i<iSize; i++)
	{
		m_obComboThGroup.AddString(vstrNeID[i].c_str());
	}

	m_obComboThGroup.SetCurSel(m_obComboThGroup.GetTopIndex());
}
