// BusinessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "BusinessDlg.h"
#include "MainFrm.h"
#include "OutputView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBusinessDlg dialog


CBusinessDlg::CBusinessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBusinessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBusinessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBusinessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBusinessDlg)
	DDX_Control(pDX, IDC_LIST_BUSINESS, m_obList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBusinessDlg, CDialog)
	//{{AFX_MSG_MAP(CBusinessDlg)
	ON_WM_SIZE()

	ON_NOTIFY(NM_CLICK, IDC_LIST_BUSINESS, OnClickListBusiness)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBusinessDlg message handlers

void CBusinessDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if ( IsWindow( m_obList.GetSafeHwnd() ) )
	{
		//int iHeight = cy - 21;
		//		//m_obList.SetWindowPos(NULL,0,0,cx, iHeight,SWP_SHOWWINDOW);
		m_obList.SetWindowPos(NULL,0,0,cx, cy,SWP_SHOWWINDOW);
	}
}

BOOL CBusinessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_obList.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	m_obList.SetExtendedStyle(dwStyle); //设置扩展风格
	
	m_obList.InsertColumn(0,"INGRESS",LVCFMT_LEFT,150); //插入列
	m_obList.InsertColumn(1,"EGRESS",LVCFMT_LEFT,150);
	m_obList.InsertColumn(2,"INDEX",LVCFMT_LEFT,100);
	m_obList.InsertColumn(3,"INST",LVCFMT_LEFT,100);


	CBitmap bmp; 
	bmp.LoadBitmap(IDR_MAINFRAME); 
	m_ImgList.Create(1, 16, ILC_COLOR8|ILC_MASK, 2, 0);
	m_ImgList.Add(&bmp,   RGB(0,   128,   128)); 
	m_obList.SetImageList(&m_ImgList,LVSIL_STATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// #include <vector>
// #include <string>
// using namespace std;



void CBusinessDlg::AddItems(string& strInfo)
{
	vector<string> vStrItems;
	m_obList.DeleteAllItems();
	Split(strInfo, "\n", vStrItems);

	for (int i=0; i < vStrItems.size(); i++)
	{
		int nRow = m_obList.InsertItem(i, "");
		vector<string> vItem;
		Split(vStrItems[i], " ", vItem);
		for (int j=0; j<vItem.size(); j++)
		{	
			m_obList.SetItemText(nRow, j, vItem[j].c_str());
		}
	}

}


void CBusinessDlg::OnClickListBusiness(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem!=-1)
	{
		CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
		COutputView* pOutputView = (COutputView*)pWnd->m_pOutputView;
		
		//pNMListView->iSubItem); 列
		int iRow = pNMListView->iItem;
		CString strINGRESS = m_obList.GetItemText(iRow, 0);
		CString strEGRESS = m_obList.GetItemText(iRow, 1);
		CString strINDEX = m_obList.GetItemText(iRow, 2);

		CString strArgs = strINGRESS + "," + strEGRESS + "," + strINDEX;

// 		#if PYTHON_EX
// 		pWnd->m_obPythonEx.SetArgs(strArgs.LockBuffer());
// 		string strRelust;
// 		if ( pWnd->m_obPythonEx.GetPyRelust("OCH_ARNODE", strRelust) == 0 )
// 		{
// 			pOutputView->m_obNeLinkDlg.ClearNeStatic();
// 			vector<string> vStrNeIP;
// 			Split(strRelust, ",", vStrNeIP);
// 			
// 			for (int i=0; i<vStrNeIP.size(); i++)
// 			{
// 				string strNeIP = vStrNeIP[i];
// 				pOutputView->m_obNeLinkDlg.CreateNeImg(strNeIP);
// 
// 			}
// 		}
// 		#endif
	}
	

	*pResult = 0;
}
