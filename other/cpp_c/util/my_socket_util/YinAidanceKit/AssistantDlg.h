#if !defined(AFX_ASSISTANTDLG_H__CBA117AF_37D7_4E23_823A_5CAEBDC7AF74__INCLUDED_)
#define AFX_ASSISTANTDLG_H__CBA117AF_37D7_4E23_823A_5CAEBDC7AF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssistantDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAssistantDlg dialog

class CAssistantDlg : public CDialog
{
// Construction
public:
	CAssistantDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAssistantDlg)
	enum { IDD = IDD_ASSISTANT_DIALOG };
	CTreeCtrl	m_obAssTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssistantDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAssistantDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASSISTANTDLG_H__CBA117AF_37D7_4E23_823A_5CAEBDC7AF74__INCLUDED_)
