// AssistantDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "AssistantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssistantDlg dialog


CAssistantDlg::CAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssistantDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssistantDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssistantDlg)
	DDX_Control(pDX, IDC_ASSISTANT_TREE, m_obAssTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssistantDlg, CDialog)
	//{{AFX_MSG_MAP(CAssistantDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssistantDlg message handlers

BOOL CAssistantDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_obAssTree.
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


