// NePointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "yinaidancekit.h"
#include "NePointDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNePointDlg dialog


CNePointDlg::CNePointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNePointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNePointDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNePointDlg)
	DDX_Control(pDX, IDC_NEID_LIST_TREE, m_obTreeNeId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNePointDlg, CDialog)
	//{{AFX_MSG_MAP(CNePointDlg)
	ON_NOTIFY(NM_CLICK, IDC_NEID_LIST_TREE, OnClickNeidListTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNePointDlg message handlers

BOOL CNePointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HTREEITEM hTreeitem = m_obTreeNeId.InsertItem("NE ID List", NULL, TVI_ROOT);
	
	
	int iSize = m_vstrNeID.size();
	for (int i=0; i<iSize; i++)
	{
		m_obTreeNeId.InsertItem(m_vstrNeID[i].c_str(), hTreeitem);
	}
	m_obTreeNeId.Expand(hTreeitem,TVE_EXPAND);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CNePointDlg::GetNeGroup(LPCTSTR lpStrGroup)
{
	 string strNeIdList = "";	 

	 CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	 pWnd->m_obNeOper.PySetArgs(lpStrGroup);
	 pWnd->m_obNeOper.PyGetResult("Config.Py_Script.Py_DataCollecte.GetArNode", strNeIdList);

	 
	 Split(strNeIdList, ";", m_vstrNeID);
}

void CNePointDlg::GetNeIdList(CString& strNeIdList)
{
	HTREEITEM hTreeItem = m_obTreeNeId.GetRootItem();
	
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_obTreeNeId.GetChildItem(hTreeItem);
	
	while (hChildItem != NULL)
	{		
		BOOL bIsCheck = m_obTreeNeId.GetCheck(hChildItem);
		if ( TRUE == bIsCheck )
		{
			strNeIdList += m_obTreeNeId.GetItemText(hChildItem);
			strNeIdList += ";";
		}
		hNextItem = m_obTreeNeId.GetNextItem(hChildItem, TVGN_NEXT);
		
		hChildItem = hNextItem;
	}	
}


void CNePointDlg::OnOK() 
{
	m_strNeIdList = "";
	GetNeIdList(m_strNeIdList);

	CDialog::OnOK();
}


void CNePointDlg::OnClickNeidListTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hRootItem = m_obTreeNeId.GetRootItem();
	CPoint   pt; 
	UINT   flag; 
	HTREEITEM   hCurrentItem; 
	
	GetCursorPos(&pt); 
	ScreenToClient(&pt); 
	hCurrentItem   =   m_obTreeNeId.HitTest(pt, &flag);   
	if   (flag & TVHT_ONITEMSTATEICON)//ÊÇµã»÷CHECK¿ò 
	{ 
		static 	BOOL bRootItem = TRUE;

		BOOL bIsCheck = m_obTreeNeId.GetCheck(hRootItem);
		if ( (TRUE == bIsCheck && FALSE == bRootItem) || (FALSE == bIsCheck && TRUE == bRootItem) )
		{
			HTREEITEM hNextItem;
			HTREEITEM hChildItem = m_obTreeNeId.GetChildItem(hRootItem);
			
			while (hChildItem != NULL)
			{		
				m_obTreeNeId.SetCheck(hChildItem, bRootItem);
				hNextItem = m_obTreeNeId.GetNextItem(hChildItem, TVGN_NEXT);
				
				hChildItem = hNextItem;
			}
			bRootItem = bIsCheck;
		}
	}
	m_obTreeNeId.Expand(hRootItem,TVE_EXPAND);
	*pResult = 0;
	
}
