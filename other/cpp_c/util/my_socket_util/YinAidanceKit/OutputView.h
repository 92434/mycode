#if !defined(AFX_OUTPUTVIEW_H__A88F1595_2318_46C1_B1DD_45CCBE407BE7__INCLUDED_)
#define AFX_OUTPUTVIEW_H__A88F1595_2318_46C1_B1DD_45CCBE407BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputView.h : header file
//#include "YinAidanceKitDoc.h"
//
#include "OutputDlg.h"
#include "MyEdit.h"
#include "NeLinkImgDlg.h"

#define  MSG_STR_PRINT     (WM_USER + 302)
/////////////////////////////////////////////////////////////////////////////
// COutputView view

class COutputView : public CView//CEditView
{
protected:
	COutputView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COutputView)

// Attributes
public:
	void AddTextPrint(char* pText);
	void ShowCtrl(int CtrlID);
	void ClearTextPrint();
// Operations
	COutputDlg	m_obOutputDlg;
	CNeLinkImgDlg m_obNeLinkDlg;
	
private:
	list<string> m_qStrPrint;
	
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COutputView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnAddTextPrint(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVIEW_H__A88F1595_2318_46C1_B1DD_45CCBE407BE7__INCLUDED_)
