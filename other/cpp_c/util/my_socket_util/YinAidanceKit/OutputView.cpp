// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"

#include "YinAidanceKitDoc.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CEditView)

COutputView::COutputView()
{
}

COutputView::~COutputView()
{
}


BEGIN_MESSAGE_MAP(COutputView, CView)
	//{{AFX_MSG_MAP(COutputView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_STR_PRINT, OnAddTextPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

void COutputView::OnInitialUpdate() 
{
 	CView::OnInitialUpdate();
	//CYinAidanceKitDoc* pDoc = (CYinAidanceKitDoc*)m_pDocument;
}

void COutputView::ClearTextPrint()
{
	m_qStrPrint.clear();
	PostMessage(MSG_STR_PRINT, 0, 0);
	//m_obOutputDlg.m_obOutputEdit.SetWindowText("");
}



// void COutputView::AddText(char* pText)
// {
// 	CString strText;
// 
// 	m_obOutputDlg.m_obOutputEdit.GetWindowText(strText);
// 	strText += pText;
// 
// 	m_obOutputDlg.m_obOutputEdit.SetWindowText(strText);
// 	m_obOutputDlg.m_obOutputEdit.LineScroll(100000);
// }


void COutputView::AddTextPrint(char* pText)
{
	static UINT iStrSize = 0;
// 	if (m_qStrPrint.empty() == true)
// 	{
// 		iStrSize = 0;
// 	}

	UINT iLen = strlen(pText);

	while(iLen + iStrSize > 99999)
	{
		if(m_qStrPrint.empty() == true)
		{
			break;
		}

		string strBack = m_qStrPrint.front();
		iStrSize = iStrSize - strBack.length();
	 	m_qStrPrint.pop_front();
	}
	
	m_qStrPrint.push_back(pText);
	iStrSize += iLen;
	
	PostMessage(MSG_STR_PRINT, 0, 0);
}


LRESULT COutputView::OnAddTextPrint(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	list<string>::iterator it = m_qStrPrint.begin();
	for (;it != m_qStrPrint.end(); it++)
	{
		strText += (*it).c_str();
	}
	
	m_obOutputDlg.m_obOutputEdit.SetWindowText(strText);
	m_obOutputDlg.m_obOutputEdit.LineScroll(100000);
	return 0;
}


int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = WS_CHILD | WS_VISIBLE  | ES_WANTRETURN | WS_HSCROLL | WS_VSCROLL  
        | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY ;//WM_MOUSEWHEEL

//    m_obEdit.Create( dwStyle, CRect(0,0,1,1), this, 0);
	//m_obEdit.CreateEx(dwStyle, "CMyEdit", "MyEdit", dwStyle, CRect(0,0,1,1), this, 0 );
//	m_obEdit.ModifyStyleEx(0,   ,   SWP_DRAWFRAME);// EN_HSCROLL | EN_VSCROLL
	m_obNeLinkDlg.Create(IDD_DIALOG_NE_IMG, this);

	m_obOutputDlg.Create(IDD_OUTPUT_DIALOG, this);//IDD_OUTPUT_DIALOG

	return 0;
}

void COutputView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	m_obOutputDlg.SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);
	m_obNeLinkDlg.SetWindowPos(NULL,0,0,cx,cy,SWP_SHOWWINDOW);
}

void COutputView::OnEditCopy() 
{
//	m_obEdit.Copy();
}

void COutputView::ShowCtrl(int CtrlID)
{
	if(  IsWindow(m_obNeLinkDlg.GetSafeHwnd()))
	{
		switch (CtrlID)
		{
		case 0:
			{
				m_obOutputDlg.ShowWindow(SW_SHOW);
			//	m_obEdit.ShowWindow(SW_HIDE);//SW_SHOW
				m_obNeLinkDlg.ShowWindow(SW_HIDE);
			}
			break;
		case 1:
			{
			//	m_obEdit.ShowWindow(SW_HIDE);
				m_obNeLinkDlg.ShowWindow(SW_SHOW);
			}
			break;
		}
	}
}

void COutputView::OnDestroy() 
{
	m_obNeLinkDlg.DestroyWindow();
	CView::OnDestroy();
}

BOOL COutputView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(   pMsg-> message   ==   WM_MOUSEWHEEL   ) 
	{ 
		int   k   =   0; 
	} 

	return CView::PreTranslateMessage(pMsg);
}
