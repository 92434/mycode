#if !defined(AFX_ALLGWINFODLG_H__0A7D3467_0051_4491_9995_3554E9737FDA__INCLUDED_)
#define AFX_ALLGWINFODLG_H__0A7D3467_0051_4491_9995_3554E9737FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllGwInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllGwInfoDlg dialog

class CAllGwInfoDlg : public CDialog
{
// Construction
public:
	CAllGwInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAllGwInfoDlg)
	enum { IDD = IDD_ALL_GW_INFO_DIALOG };
	CComboBox	m_obComLocalIp;
	CString	m_strPwd;
	CString	m_strUser;
	CString	m_strTimer;
	CString	m_strTitle;
	//}}AFX_DATA

	CString m_strLocalIp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllGwInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAllGwInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLGWINFODLG_H__0A7D3467_0051_4491_9995_3554E9737FDA__INCLUDED_)
