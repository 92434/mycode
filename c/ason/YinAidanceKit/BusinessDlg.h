#if !defined(AFX_BUSINESSDLG_H__E502D7A6_8B29_47A7_B77A_EE1ECCDC6D65__INCLUDED_)
#define AFX_BUSINESSDLG_H__E502D7A6_8B29_47A7_B77A_EE1ECCDC6D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BusinessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBusinessDlg dialog

class CBusinessDlg : public CDialog
{
// Construction
public:
	CBusinessDlg(CWnd* pParent = NULL);   // standard constructor
	void AddItems(string& strInfo);

	CImageList m_ImgList;
// Dialog Data
	//{{AFX_DATA(CBusinessDlg)
	enum { IDD = IDD_DIALOG_BUSINESS };
	CListCtrl	m_obList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBusinessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){};
	virtual void OnCancel(){};
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBusinessDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListBusiness(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUSINESSDLG_H__E502D7A6_8B29_47A7_B77A_EE1ECCDC6D65__INCLUDED_)
