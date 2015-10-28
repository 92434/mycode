// OutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "OutputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog


COutputDlg::COutputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputDlg)
	DDX_Control(pDX, IDC_OUTPUT_EDIT, m_obOutputEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputDlg, CDialog)
	//{{AFX_MSG_MAP(COutputDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputDlg message handlers

BOOL COutputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_obOutputEdit.SetMargins(5, 5);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutputDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( IsWindow(m_obOutputEdit.GetSafeHwnd()) )
	{
		m_obOutputEdit.SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);
	}	
}




