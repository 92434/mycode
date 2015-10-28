// UIThread.cpp : implementation file
//

#include "stdafx.h"
#include "..\yinaidancekit.h"
#include "..\MainFrm.h"
#include "UIThread.h"
#include "PythonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	m_pPythonEx = &(pWnd->m_obPythonEx);
}

CUIThread::~CUIThread()
{
}

BOOL CUIThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	
	return TRUE;
}

int CUIThread::ExitInstance()
{
	
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
	//{{AFX_MSG_MAP(CUIThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_MESSAGE(WM_MSG_EXE_PY_MOD,OnExePyModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIThread message handlers
LRESULT CUIThread::OnExePyModule(WPARAM wParam, LPARAM lParam)
{
	m_pPythonEx->CreatePython();
	int a = 0;
	m_pPythonEx->ExePyModule("yintest");
	m_pPythonEx->ReleasePython();
	return 0;
}