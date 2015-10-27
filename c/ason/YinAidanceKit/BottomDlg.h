#if !defined(AFX_BOTTOMDLG_H__43A8C1D5_5335_46F1_A8EB_150E1768C930__INCLUDED_)
#define AFX_BOTTOMDLG_H__43A8C1D5_5335_46F1_A8EB_150E1768C930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottomDlg.h : header file
//
// 	CTabCtrl m_obTab;
// 	CMyEdit m_obEditCmd;
// 	CMyEdit m_obEditDebug;
#include "MyEdit.h"
#include "BusinessDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CBottomDlg dialog

class CBottomDlg : public CDialog
{
// Construction
public:
	CBottomDlg(CWnd* pParent = NULL);   // standard constructor

	void EnableWin(BOOL bSign);

	CBusinessDlg m_obBusinessDlg;
// Dialog Data
	//{{AFX_DATA(CBottomDlg)
	enum { IDD = IDD_BOTTOM_DIALOG };
	CTabCtrl	m_obBottomTab;
	CMyEdit	m_obEditDebug;
	CMyEdit	m_obEditCmd;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
private:
	void ShowCtrl();

protected:

	// Generated message map functions
	//{{AFX_MSG(CBottomDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBottomTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOMDLG_H__43A8C1D5_5335_46F1_A8EB_150E1768C930__INCLUDED_)
