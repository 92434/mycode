// YinAidanceKitView.cpp : implementation of the CYinAidanceKitView class
//

#include "stdafx.h"
#include "YinAidanceKit.h"

#include "YinAidanceKitDoc.h"
#include "YinAidanceKitView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitView

IMPLEMENT_DYNCREATE(CYinAidanceKitView, CView)

BEGIN_MESSAGE_MAP(CYinAidanceKitView, CView)
	//{{AFX_MSG_MAP(CYinAidanceKitView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_STR_DEBUG, OnAddTextDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitView construction/destruction

CYinAidanceKitView::CYinAidanceKitView()
{
	// TODO: add construction code here
}

CYinAidanceKitView::~CYinAidanceKitView()
{
}

BOOL CYinAidanceKitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitView drawing

void CYinAidanceKitView::OnDraw(CDC* pDC)
{
	CYinAidanceKitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitView diagnostics

#ifdef _DEBUG
void CYinAidanceKitView::AssertValid() const
{
	CView::AssertValid();
}

void CYinAidanceKitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYinAidanceKitDoc* CYinAidanceKitView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYinAidanceKitDoc)));
	return (CYinAidanceKitDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitView message handlers

void CYinAidanceKitView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	

//	m_obTab.InsertItem(0, "Cmd", 0);
//	m_obTab.InsertItem(2, "Debug", 0);
	
// 	m_obEditCmd.SetMargins(5, 5);
}

int CYinAidanceKitView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
        | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_NOHIDESEL | ES_WANTRETURN;
	
  //  m_obEditCmd.Create( dwStyle, CRect(0,0,1,1), this, 0);//ES_READONLY
//	m_obEditDebug.Create( dwStyle | ES_READONLY, CRect(0,0,1,1), this, 0);
	
//	m_obTab.Create(WS_CHILD | WS_VISIBLE  | TCS_FIXEDWIDTH | TCS_HOTTRACK  | TCS_BOTTOM | TCS_RIGHT, CRect(0,0,1,1), this, 0 );// this//Tab Control

	m_obBottomDlg.Create(IDD_BOTTOM_DIALOG, this);


	return 0;
}

void CYinAidanceKitView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	m_obBottomDlg.SetWindowPos(NULL,0,0,cx, cy,SWP_SHOWWINDOW);
//	int iHeight = cy - 25;
//	m_obEditCmd.SetWindowPos(NULL,0,0,cx, iHeight,SWP_SHOWWINDOW);
//	m_obTab.SetWindowPos(NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);
}

// void CYinAidanceKitView::OnEditPaste() 
// {
// 	m_obBottomDlg.m_obEditCmd.Paste();	
// }
// 
// void CYinAidanceKitView::OnEditCopy() 
// {
// 	m_obBottomDlg.m_obEditCmd.Copy();
// }
// 
// void CYinAidanceKitView::OnEditCut() 
// {
// 	m_obBottomDlg.m_obEditCmd.Cut();
// }
// 
// void CYinAidanceKitView::OnEditUndo() 
// {
// 	m_obBottomDlg.m_obEditCmd.Undo();
// }


// void CYinAidanceKitView::AddTextCmd(char* pText)
// {
// 	CString strText;
// 	//	m_strText += pText;
// 	
// 	m_obBottomDlg.m_obEditCmd.GetWindowText(strText);
// 	strText += pText;
// 	m_obBottomDlg.m_obEditCmd.SetWindowText(strText);
// 	
// 	m_obBottomDlg.m_obEditCmd.LineScroll(100000);
// }

void CYinAidanceKitView::AddTextDebug(char* pText)
{
	static UINT iStrSize = 0;
	if (m_qStrDebug.empty() == true)
	{
		iStrSize = 0;
	}
	iStrSize += strlen(pText);
	while (iStrSize > 99999)
	{
		string strBack = m_qStrDebug.front();
		iStrSize = iStrSize - strBack.length();
		m_qStrDebug.pop_front();
	}
	
	m_qStrDebug.push_back(pText);

	PostMessage(MSG_STR_DEBUG, 0, 0);
}


LRESULT CYinAidanceKitView::OnAddTextDebug(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	list<string>::iterator it = m_qStrDebug.begin();
	for (;it != m_qStrDebug.end(); it++)
	{
		strText += (*it).c_str();
	}

	m_obBottomDlg.m_obEditDebug.SetWindowText(strText);
	m_obBottomDlg.m_obEditDebug.LineScroll(100000);
	if (lParam == 0)
	{
		SetCulFocus(1);	

		//1.脚本出错后重新开启服务，避免假死现象
		CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
		pWnd->RunStop();
	}
	return 0;
}


void CYinAidanceKitView::ClearTextDebug()
{
	m_qStrDebug.clear();
	PostMessage(MSG_STR_DEBUG, 0, 1);
	//m_obBottomDlg.m_obEditDebug.SetWindowText("");
}


void CYinAidanceKitView::SetCulFocus(int iFace)
{
	m_obBottomDlg.m_obBottomTab.SetCurFocus(iFace);
}

void CYinAidanceKitView::SetEditCmd(BOOL bEnable)
{
	m_obBottomDlg.m_obEditCmd.EnableWindow(bEnable);
}




