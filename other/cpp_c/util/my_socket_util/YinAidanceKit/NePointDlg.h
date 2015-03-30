#if !defined(AFX_NEPOINTDLG_H__7246BFCD_D934_47E6_959F_AA4EBBCFDF1B__INCLUDED_)
#define AFX_NEPOINTDLG_H__7246BFCD_D934_47E6_959F_AA4EBBCFDF1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NePointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNePointDlg dialog

class CNePointDlg : public CDialog
{
// Construction
public:
	CNePointDlg(CWnd* pParent = NULL);   // standard constructor

	void GetNeGroup(LPCTSTR lpStrGroup);
	void GetNeIdList(CString& strNeIdList);
	CString m_strNeIdList;
// Dialog Data
	//{{AFX_DATA(CNePointDlg)
	enum { IDD = IDD_SELNE_DIALOG };
	CTreeCtrl	m_obTreeNeId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNePointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNePointDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickNeidListTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	vector<string> m_vstrNeID;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEPOINTDLG_H__7246BFCD_D934_47E6_959F_AA4EBBCFDF1B__INCLUDED_)
