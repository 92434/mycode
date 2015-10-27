// NeLinkImgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "NeLinkImgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNeLinkImgDlg dialog


CNeLinkImgDlg::CNeLinkImgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNeLinkImgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNeLinkImgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNeLinkImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNeLinkImgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNeLinkImgDlg, CDialog)
	//{{AFX_MSG_MAP(CNeLinkImgDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeLinkImgDlg message handlers

BOOL CNeLinkImgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_obPoint = CPoint(4, 10);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNeLinkImgDlg::DestroyWindow() 
{
	ClearNeStatic();
	return CDialog::DestroyWindow();
}

void CNeLinkImgDlg::CreateNeImg(string& strIP)
{
	CCtrlNeStatic* pNeCtrl = new CCtrlNeStatic();
	pNeCtrl->Create( strIP.c_str(), WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(m_obPoint, m_obPoint + CPoint(100,20) ), this); //SS_BITMAP
	m_vpNeCtrl.push_back(pNeCtrl);

	m_vPoint.push_back(m_obPoint);

	static int iCol = 0;
	static int iRow = 170;
	m_obPoint += CPoint(iRow, iCol);
	if ( m_obPoint.x < 10 )
	{
		m_obPoint.x = 4;
		iRow = iRow * -1;
		iCol += 90;
	}
	else if(m_obPoint.x > 680)
	{
		m_obPoint.x += 170;
		iRow = iRow * -1;
		iCol += 90;
	
	}

	this->RedrawWindow();
}


void CNeLinkImgDlg::ClearNeStatic()
{
	for (int i=0; i<m_vpNeCtrl.size(); i++)
	{
		m_vpNeCtrl[i]->DestroyWindow();
		delete m_vpNeCtrl[i];
	}
	m_vpNeCtrl.clear();
	m_vPoint.clear();
	m_obPoint = CPoint(4, 10);

}

void CNeLinkImgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int iSize = m_vPoint.size();
	if ( iSize > 1 )
	{
		for (int i=1; i<iSize; i++)
		{
			CPoint obPoint1 = m_vPoint[i-1];
			CPoint obPoint2 = m_vPoint[i];
			dc.MoveTo(obPoint1 + CPoint(38, 20));
			dc.LineTo(obPoint2 + CPoint(0, 20));
		}
	}


	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
