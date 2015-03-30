// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__959A505E_F23A_4466_9D2B_17A002F762BD__INCLUDED_)
#define AFX_MAINFRM_H__959A505E_F23A_4466_9D2B_17A002F762BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Network\NeOper.h"

#include <vector>
#include <string>
using namespace std;


class CYinAidanceKitView;
class COutputView;

class CMainFrame : public CFrameWnd
{
public:

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)


// Attributes
public:
	CYinAidanceKitView* m_pEditView;
	COutputView*		m_pOutputView;

	CNeOper				m_obNeOper;
	//CNeOperate			m_obNeOperate;
	//M_ObPythonEx;
// Operations
public:
	string ExeCmd(const char* strCmd, BOOL isPrint = TRUE);
	string LocalCmd(const char* pCmd);
	string FileDialog(string& strFlied);
	string FolderDialog();
	CString GetFileVer();

	void SetMenuEnable(BOOL bEnable = TRUE);

	void OnRun();

	void StartServer();
	void ConnectAndLoginNe();

	void RunStop();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void InitToolBar();
	void UserLogin();

private:
	string		m_strName;
    string		m_strPWD;
	string		m_strNeID;
	string		m_strNeIP;
	int			m_iPort;
	int			m_iLoginTimer;
	int			m_iHeartTimer;

	BOOL		m_bMenuEnable;

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CSplitterWnd	m_obSplitterWnd;


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuTest();
	afx_msg void OnMenuTest1();
	afx_msg void OnMenuLogin();
	afx_msg void OnMenuLogout();
	afx_msg void OnUpdateMenuLogout(CCmdUI* pCmdUI);
	afx_msg void OnNeSubsidiary();
	afx_msg LRESULT OnUploadFileMsg(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnMenuItemAllCollec();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateMenuItemAllCollec(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnRunStop();
	afx_msg void OnUpdateRunStop(CCmdUI* pCmdUI);
	afx_msg LRESULT OnMenuEnable(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnMenuItemAsonlogAnalyse();
	afx_msg void OnUpdateMenuItemAsonlogAnalyse(CCmdUI* pCmdUI);
	afx_msg void OnMenuAllGwInfo();
	afx_msg void OnUpdateMenuAllGwInfo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__959A505E_F23A_4466_9D2B_17A002F762BD__INCLUDED_)
