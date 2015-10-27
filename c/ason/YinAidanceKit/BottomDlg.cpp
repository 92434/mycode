// BottomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "BottomDlg.h"
#include "MainFrm.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBottomDlg dialog


CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBottomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBottomDlg)
	DDX_Control(pDX, IDC_BOTTOM_TAB, m_obBottomTab);
	DDX_Control(pDX, IDC_EDIT_DEBUG, m_obEditDebug);
	DDX_Control(pDX, IDC_EDIT_CMD, m_obEditCmd);
	//}}AFX_DATA_MAP


}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialog)
	//{{AFX_MSG_MAP(CBottomDlg)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BOTTOM_TAB, OnSelchangeBottomTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomDlg message handlers

void CBottomDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

	if ( IsWindow( m_obEditDebug.GetSafeHwnd() ) && IsWindow( m_obEditCmd.GetSafeHwnd() ) && 
		IsWindow( m_obBottomTab.GetSafeHwnd() ) && IsWindow( m_obBusinessDlg.GetSafeHwnd() ) )
	{
		int iHeight = cy - 21;
		m_obEditDebug.SetWindowPos(NULL,0,0,cx, iHeight,SWP_SHOWWINDOW);
		m_obEditCmd.SetWindowPos(NULL,0,0,cx, iHeight,SWP_SHOWWINDOW);
		m_obBusinessDlg.SetWindowPos(NULL,0,0,cx, iHeight,SWP_SHOWWINDOW);
		m_obBottomTab.SetWindowPos(NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);
		ShowCtrl();
	}

}

BOOL CBottomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_obBottomTab.InsertItem(0, "Cmd", 0);
	m_obBottomTab.InsertItem(1, "Debug", 0);
//	m_obBottomTab.InsertItem(2, "OCH_LSP", 0);

	m_obEditDebug.SetMyEditFont();
	m_obEditCmd.SetMyEditFont();

	m_obEditDebug.SetMargins(5, 5);	
	m_obEditCmd.SetMargins(5, 5);

	m_obBusinessDlg.Create(IDD_DIALOG_BUSINESS, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBottomDlg::OnSelchangeBottomTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowCtrl();
	*pResult = 0;
}


void CBottomDlg::ShowCtrl()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	COutputView*  pOutputView = (COutputView*)pWnd->m_pOutputView;

	int iIndex = m_obBottomTab.GetCurSel();
	switch (iIndex)
	{
	case 0:
		{
			m_obBusinessDlg.ShowWindow(SW_HIDE);
			m_obEditDebug.ShowWindow(SW_HIDE);
			m_obEditCmd.ShowWindow(SW_SHOW);
			
			if ( pOutputView != NULL && IsWindow(pOutputView->GetSafeHwnd()))
			{
				pOutputView->ShowCtrl(0);
			}
			
		}
		break;
	case 1:
		{
			m_obBusinessDlg.ShowWindow(SW_HIDE);
			m_obEditDebug.ShowWindow(SW_SHOW);
			m_obEditCmd.ShowWindow(SW_HIDE);
			
			if ( pOutputView != NULL && IsWindow(pOutputView->GetSafeHwnd()))
			{
				pOutputView->ShowCtrl(0);
			}
		}
		break;
	case 2:
		{
			m_obBusinessDlg.ShowWindow(SW_SHOW);
			m_obEditDebug.ShowWindow(SW_HIDE);
			m_obEditCmd.ShowWindow(SW_HIDE);
			
			if ( pOutputView != NULL && IsWindow(pOutputView->GetSafeHwnd()))
			{
				pOutputView->ShowCtrl(1);

				string strRelust;
// 				#if PYTHON_EX
// 				if ( pWnd->m_obPythonEx.GetPyRelust("OCH_LSP_INFO", strRelust) == 0 )
// 				{
// 					m_obBusinessDlg.AddItems(strRelust);
// 				}
// 				#endif
				
			}
		}
		break;

	}
}



void CBottomDlg::EnableWin(BOOL bSign)
{
	m_obBottomTab.EnableWindow(bSign);
	EnableWindow(bSign);
}


void CBottomDlg::OnOK()
{

}

void CBottomDlg::OnCancel()
{

}
