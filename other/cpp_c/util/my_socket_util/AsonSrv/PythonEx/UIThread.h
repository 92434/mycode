#if !defined(AFX_UITHREAD_H__D5E8E6CA_EE0C_45AC_8496_2BA25ED6407A__INCLUDED_)
#define AFX_UITHREAD_H__D5E8E6CA_EE0C_45AC_8496_2BA25ED6407A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIThread.h : header file
//


#define WM_MSG_EXE_PY_MOD    (WM_USER+1234)

class CPythonEx;
/////////////////////////////////////////////////////////////////////////////
// CUIThread thread

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

	           // protected constructor used by dynamic creation
private:
	CPythonEx* m_pPythonEx;
// Attributes
public:
	CUIThread();
	virtual ~CUIThread();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUIThread)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg LRESULT OnExePyModule(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UITHREAD_H__D5E8E6CA_EE0C_45AC_8496_2BA25ED6407A__INCLUDED_)
