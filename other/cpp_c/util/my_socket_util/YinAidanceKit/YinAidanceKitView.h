// YinAidanceKitView.h : interface of the CYinAidanceKitView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINAIDANCEKITVIEW_H__677FE758_D980_4783_B237_7A59877C64E5__INCLUDED_)
#define AFX_YINAIDANCEKITVIEW_H__677FE758_D980_4783_B237_7A59877C64E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BottomDlg.h"

#define  MSG_STR_DEBUG      (WM_USER + 301)

class CYinAidanceKitDoc;

class CYinAidanceKitView : public CView//CEditView
{
protected: // create from serialization only
	CYinAidanceKitView();
	DECLARE_DYNCREATE(CYinAidanceKitView)

// Attributes
public:
	CYinAidanceKitDoc* GetDocument();
	void AddTextDebug(char* pText);
	void ClearTextDebug();
	void SetCulFocus(int iFace);
	void SetEditCmd(BOOL bEnable);
	CBottomDlg m_obBottomDlg;
private:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYinAidanceKitView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CYinAidanceKitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	list<string> m_qStrDebug;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CYinAidanceKitView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnAddTextDebug(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in YinAidanceKitView.cpp
inline CYinAidanceKitDoc* CYinAidanceKitView::GetDocument()
   { return (CYinAidanceKitDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YINAIDANCEKITVIEW_H__677FE758_D980_4783_B237_7A59877C64E5__INCLUDED_)
