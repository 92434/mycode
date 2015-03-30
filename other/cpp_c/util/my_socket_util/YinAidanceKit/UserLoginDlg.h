#if !defined(AFX_USERLOGINDLG_H__EF0230C1_5828_4402_ACDE_FAA63C9FDFB7__INCLUDED_)
#define AFX_USERLOGINDLG_H__EF0230C1_5828_4402_ACDE_FAA63C9FDFB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog

class CUserLoginDlg : public CDialog
{
// Construction
public:
	CUserLoginDlg(CWnd* pParent = NULL);   // standard constructor
	
	void AddNeIP(const char* strIP);
public:
	CString m_strNeIP;
	CString m_strHostIP;
	UINT m_iPort;
	CString m_strNeID;
	CString m_strName;
	CString m_strPWD;

// Dialog Data
	//{{AFX_DATA(CUserLoginDlg)
	enum { IDD = IDD_lOGIN_DIALOG };
	CEdit	m_obPortEdit;
	CEdit	m_obNameEdit;
	CEdit	m_obPwdEdit;
	CEdit	m_obEditNeID;
	CComboBox	m_obComboNeIp;
	CComboBox	m_obComboHostIp;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserLoginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeHostIpCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLOGINDLG_H__EF0230C1_5828_4402_ACDE_FAA63C9FDFB7__INCLUDED_)
