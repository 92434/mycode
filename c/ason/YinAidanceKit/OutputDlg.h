#if !defined(AFX_OUTPUTDLG_H__568BC93D_2900_4183_BCD0_767195D1A89B__INCLUDED_)
#define AFX_OUTPUTDLG_H__568BC93D_2900_4183_BCD0_767195D1A89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputDlg.h : header file
//
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog

class COutputDlg : public CDialog
{
// Construction
public:
	COutputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COutputDlg)
	enum { IDD = IDD_OUTPUT_DIALOG };
	CMyEdit	m_obOutputEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){};
	virtual void OnCancel(){};
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTDLG_H__568BC93D_2900_4183_BCD0_767195D1A89B__INCLUDED_)
