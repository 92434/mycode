#if !defined(AFX_MYEDIT_H__4498FB97_8F94_4C01_95EE_180F75325146__INCLUDED_)
#define AFX_MYEDIT_H__4498FB97_8F94_4C01_95EE_180F75325146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();
//    void FindStr(const CString &strFind);
//    void SetNewMenu(CMenu *pMenu)      {m_pNewMenu = pMenu;}
    void FontInit();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
		virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();
	    void SetMyEditFont(void);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
// 	afx_msg void OnMenuitemOutputCopy();
// 	afx_msg void OnMenuitemOutputClear();
// 	afx_msg void OnMenuitemOutputFind();
// 	afx_msg void OnMenuitemInputCopy();
// 	afx_msg void OnMenuitemInputCut();
// 	afx_msg void OnMenuitemInputFind();
// 	afx_msg void OnMenuitemInputPaste();
// 	afx_msg void OnMenuitemInputSelectAll();
// 	afx_msg void OnMenuitemInputUndo();
// 	afx_msg void OnMenuitemFont();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

//	afx_msg LRESULT OnRegisteredMouseWheel( WPARAM wParam, LPARAM lParam );

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	BOOL	m_bCtrlKeyDown;
//    int     m_iFindStart;
//   CMenu   *m_pNewMenu;

    CFont	    m_objNewFont;
    LOGFONT     m_logNewFont;
    COLORREF    m_ColorText;

    // Added by LiuKai(00119002) for DTS2010082600255, 2010/9/1
    HBRUSH      m_hbrEditColor;
    // End of addition by LiuKai(00119002), 2010/9/1


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__4498FB97_8F94_4C01_95EE_180F75325146__INCLUDED_)
