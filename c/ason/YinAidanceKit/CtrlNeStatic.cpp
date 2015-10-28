// CtrlNeStatic.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "CtrlNeStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlNeStatic

CCtrlNeStatic::CCtrlNeStatic()
{
	m_bStartMove = FALSE;

	m_obNeBmp.LoadBitmap(IDB_NE_BITMAP);
	m_obNeBmp.GetBitmap(&m_obBitmap);

// 	   //加载图片
// 	strcpy(m_bmpPath,bmpPath);
// 	m_hbmp =   (HBITMAP)LoadImage(NULL,"c:/BIN/res/Close.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
//     m_curBmp.Attach(m_hbmp);
//     if (NULL == m_curBmp.m_hObject)

}

CCtrlNeStatic::~CCtrlNeStatic()
{
}


BEGIN_MESSAGE_MAP(CCtrlNeStatic, CStatic)
	//{{AFX_MSG_MAP(CCtrlNeStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlNeStatic message handlers

void CCtrlNeStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bStartMove)
    {       
		CRect rect;     
		ClientToScreen(&point);    
		GetParent()->ScreenToClient(&point);   
		GetClientRect(&rect);      
		point.Offset(-rect.Width()/2, -rect.Height()/2);

		//这里可能需要控制按钮的移动范围，使其不移动到界面外，方法：CWnd::SetWindowPlacement
        ::SetWindowPos(GetSafeHwnd(), HWND_TOP, point.x , point.y , 0, 0, SWP_NOSIZE);//SWP_NOZORDER|   
		Invalidate();       
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

void CCtrlNeStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bStartMove = TRUE;
	SetCapture();
	CStatic::OnLButtonDown(nFlags, point);
}

void CCtrlNeStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bStartMove = FALSE;
	ReleaseCapture();
	
	CStatic::OnLButtonUp(nFlags, point);
}

void CCtrlNeStatic::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, SS_NOTIFY);
	CStatic::PreSubclassWindow();
}



void CCtrlNeStatic::OnPaint() 
{
	CPaintDC dc(this); 
    CRect curRect;
    GetClientRect(curRect);
    CDC memDC;
    memDC.CreateCompatibleDC(NULL);
    memDC.SelectObject(&m_obNeBmp);
	
    dc.BitBlt(0,0, m_obBitmap.bmWidth, m_obBitmap.bmHeight,&memDC,0,0,SRCCOPY);
//	CStatic::OnPaint();
}

 int CCtrlNeStatic::OnCreate( LPCREATESTRUCT dd )
 {
	::SetWindowPos(GetSafeHwnd(), 0, 0 , 0 , m_obBitmap.bmWidth, m_obBitmap.bmHeight, SWP_NOMOVE);
	 return 0;
 }
