#if !defined(AFX_CTRLNESTATIC_H__13594378_7ED4_4417_B7FF_47DB5210771B__INCLUDED_)
#define AFX_CTRLNESTATIC_H__13594378_7ED4_4417_B7FF_47DB5210771B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtrlNeStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCtrlNeStatic window

class CCtrlNeStatic : public CStatic
{
// Construction
public:
	CCtrlNeStatic();

// Attributes
public:
	//HBITMAP m_hbmp;   
 	CBitmap m_obNeBmp;
	BITMAP m_obBitmap;
// Operations
public:
	BOOL m_bStartMove;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlNeStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCtrlNeStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCtrlNeStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate( LPCREATESTRUCT dd);
	void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRLNESTATIC_H__13594378_7ED4_4417_B7FF_47DB5210771B__INCLUDED_)
