#if !defined(AFX_ALLCOLLECDLG_H__430E3884_14A7_471F_8B5E_54EF1FDF69D5__INCLUDED_)
#define AFX_ALLCOLLECDLG_H__430E3884_14A7_471F_8B5E_54EF1FDF69D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllCollecDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllCollecDlg dialog

class CAllCollecDlg : public CDialog
{
// Construction
public:
	CAllCollecDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_strComboGroup;
	CString m_strComboNeId;
	char	m_cOperArea;
	char	m_cContent;
// Dialog Data
	//{{AFX_DATA(CAllCollecDlg)
	enum { IDD = IDD_ALL_COLLEC_DIALOG };
	CComboBox	m_obComboThGroup;
	CComboBox	m_obComboNeid;
	int		m_iRadioType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllCollecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAllCollecDlg)
	virtual void OnOK();
	afx_msg void OnRadioOnlySite();
	afx_msg void OnRadioOperation();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLCOLLECDLG_H__430E3884_14A7_471F_8B5E_54EF1FDF69D5__INCLUDED_)
