#if !defined(AFX_NELINKIMGDLG_H__D9D7613E_E5E8_4EB3_A272_6C0ACC75F472__INCLUDED_)
#define AFX_NELINKIMGDLG_H__D9D7613E_E5E8_4EB3_A272_6C0ACC75F472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NeLinkImgDlg.h : header file
//
#include "CtrlNeStatic.h"
#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CNeLinkImgDlg dialog

class CNeLinkImgDlg : public CDialog
{
// Construction
public:
	CNeLinkImgDlg(CWnd* pParent = NULL);   // standard constructor
	void CreateNeImg(string& strIP);
	void ClearNeStatic();

private:
	vector< CCtrlNeStatic* > m_vpNeCtrl;
	vector< CPoint > m_vPoint;
	CPoint m_obPoint;
public:
// Dialog Data
	//{{AFX_DATA(CNeLinkImgDlg)
	enum { IDD = IDD_DIALOG_NE_IMG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeLinkImgDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNeLinkImgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NELINKIMGDLG_H__D9D7613E_E5E8_4EB3_A272_6C0ACC75F472__INCLUDED_)
