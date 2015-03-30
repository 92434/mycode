// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "MainFrm.h"
#include "MyEdit.h"

// #include <string>
// using namespace std;

//extern unsigned int GetGlobalNeId();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit


CMyEdit::CMyEdit()
{
    m_bCtrlKeyDown = FALSE;
//    m_iFindStart   = 0;
//    m_pFindView    = NULL;
//    m_pNewMenu     = NULL;

    m_ColorText    = RGB(0,0,0);    // 默认字体为黑色

    memset(&m_logNewFont, 0, sizeof(LOGFONT));       // zero out structure
    m_logNewFont.lfHeight           = 16;            // request a 16-pixel-height font
    m_logNewFont.lfWeight           = 400;
    m_logNewFont.lfOutPrecision     = OUT_STRING_PRECIS;
    m_logNewFont.lfClipPrecision    = CLIP_STROKE_PRECIS;
    m_logNewFont.lfQuality          = DRAFT_QUALITY;
    m_logNewFont.lfPitchAndFamily   = (FF_MODERN | FIXED_PITCH);

    /*
    int nLangFlag = CInitCfg::Instance()->GetLanguageFlg();
    // 是中文版本
    if (0 == nLangFlag)
    {
        m_logNewFont.lfCharSet      = GB2312_CHARSET;
        strcpy(m_logNewFont.lfFaceName, "Fixedsys");    // request a face name "Fixedsys"
    }
    else
    {
        strcpy(m_logNewFont.lfFaceName, "Courier New"); // request a face name "Courier New"
    }
    */
    FontInit();

    // Added by LiuKai(00119002) for DTS2010082600255, 2010/9/1
    m_hbrEditColor = ::CreateSolidBrush(RGB(255,255,255));
    ASSERT(NULL != m_hbrEditColor);
    // End of addition by LiuKai(00119002), 2010/9/1
}

CMyEdit::~CMyEdit()
{
    // Added by LiuKai(00119002) for DTS2010082600255, 2010/9/1
    BOOL bRet = ::DeleteObject(m_hbrEditColor);
    ASSERT(FALSE != bRet);
    // End of addition by LiuKai(00119002), 2010/9/1
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
    //{{AFX_MSG_MAP(CMyEdit)
    ON_WM_CREATE()
    ON_WM_KEYUP()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_ACTIVATE()
    ON_WM_SETFOCUS()
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_RBUTTONUP()
//     ON_COMMAND(ID_MENUITEM_OUTPUT_COPY, OnMenuitemOutputCopy)
//     ON_COMMAND(ID_MENUITEM_OUTPUT_CLEAR, OnMenuitemOutputClear)
//     ON_COMMAND(ID_MENUITEM_OUTPUT_FIND, OnMenuitemOutputFind)
//   ON_COMMAND(ID_MENUITEM_INPUT_COPY, OnMenuitemInputCopy)
//     ON_COMMAND(ID_MENUITEM_INPUT_CUT, OnMenuitemInputCut)
//     ON_COMMAND(ID_MENUITEM_INPUT_FIND, OnMenuitemInputFind)
//     ON_COMMAND(ID_MENUITEM_INPUT_PASTE, OnMenuitemInputPaste)
//     ON_COMMAND(ID_MENUITEM_INPUT_SELECT_ALL, OnMenuitemInputSelectAll)
//     ON_COMMAND(ID_MENUITEM_INPUT_UNDO, OnMenuitemInputUndo)
//     ON_COMMAND(ID_MENUITEM_FONT, OnMenuitemFont)
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers
BOOL  CMyEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bRes = CEdit::Create(dwStyle, rect,  pParentWnd, nID);
	this->SetMyEditFont();
	return bRes;
}


int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    // TODO: Add your specialized creation code here
    this->SetMyEditFont();

    return 0;
}

void CMyEdit::FontInit()
{
//     int nLangFlag = CInitCfg::Instance()->GetLanguageFlg();
//     // 是中文版本
//     if (0 == nLangFlag)
//     {
//     /* Begin: Modified by zhangwei 119000, 2011-6-27 for UpgradeKit*/
//     // 修改中文字体，和英文保持一致
// //        m_logNewFont.lfCharSet      = GB2312_CHARSET;
// //        strcpy(m_logNewFont.lfFaceName, "Fixedsys");    // request a face name "Fixedsys"
//         strcpy(m_logNewFont.lfFaceName, "Courier New");
//     /* End:    Modified by zhangwei 119000, 2011-6-27*/
//     }
//     else
//     {
        strcpy(m_logNewFont.lfFaceName, "Courier New"); // request a face name "Courier New"
//    }
}

void CMyEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ( VK_CONTROL == nChar )
    {
        m_bCtrlKeyDown = FALSE;
    }

    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ( VK_CONTROL == nChar )
    {
        m_bCtrlKeyDown = TRUE;
    }

	if (m_bCtrlKeyDown == TRUE)
	{
		if ( 'A' == nChar || 'a' == nChar )
        {
            SetSel(0,-1);
			return;
        }
	}

    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

   
void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ( VK_RETURN == nChar )
    {
		DWORD dwStyle = GetStyle();
		if (ES_READONLY & dwStyle) 	return;
	
		int  iIndex = LineFromChar();
		int iLength = LineLength( LineIndex(iIndex) );
		if ( iLength > 0 )
		{
			string strCmd;
			strCmd.resize(iLength, 0x00);
			GetLine(iIndex, (char*)strCmd.c_str(), iLength);

			CMainFrame* pWndFrm = (CMainFrame*)AfxGetMainWnd();
			pWndFrm->ExeCmd(strCmd.c_str());
			return;
		}		 

    }

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CMyEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
    CEdit::OnActivate(nState, pWndOther, bMinimized);
    
    // TODO: Add your message handler code here
}

void CMyEdit::OnSetFocus(CWnd* pOldWnd) 
{  
	int iStyle = GetStyle() & ES_READONLY;
	if(iStyle == 0)
	{
		CEdit::OnSetFocus(pOldWnd);
	}

    // TODO: Add your message handler code here
    CWnd* pMain = AfxGetMainWnd();

//     if ( pMain )
//     {
//         CMenu* pMenu = pMain->GetMenu();
//         if ( pMenu )
//         {
//             CMenu* pEditMenu = pMenu->GetSubMenu(1);
// 
//             if ( pEditMenu )
//             {
//                 UINT uiRet = 0; 
//                 if ( GetStyle() & ES_READONLY )
//                 {
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | MF_ENABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND | MF_ENABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | MF_ENABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_PASTE, MF_BYCOMMAND | MF_ENABLED);
//                 }
//                 else
//                 {
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | MF_DISABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND | MF_DISABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | MF_ENABLED);
//                     uiRet = pMenu->EnableMenuItem(ID_EDIT_PASTE, MF_BYCOMMAND | MF_DISABLED);
//                 }
//             }
//         }
//     }
// 
//     if (NULL != m_pFindView)
//     {
//         if (NULL != g_pDlgFind)
//         {
//             g_pDlgFind->SetFindView(m_pFindView);
//         }
//     }
}


// void CMyEdit::FindStr(const CString &strFind)
// {
    // 不区分大小写的查找
//     CString strRealFind(strFind);
//     strRealFind.MakeLower();
// 
//     CString strEdit;
//     this->GetWindowText(strEdit);
//     strEdit.MakeLower();
// 
//     const int iLen = strRealFind.GetLength();
//     int iFind = strEdit.Find(strRealFind, m_iFindStart);
// 
//     // 没有找到
//     if (-1 == iFind)
//     {
//         if (0 != m_iFindStart)
//         {
//             // 从头开始再查找一次
//             iFind = strEdit.Find(strRealFind, 0);
//             if (-1 != iFind)
//             {
//                 m_iFindStart = iFind + 1;
//                 this->SetFocus();
//                 this->SetSel(iFind, iFind + iLen);
//             }
//             else
//             {
//                 m_iFindStart = 0;
//             }
//         }
//     } 
//     else
//     {
//         m_iFindStart = iFind + 1;
//         this->SetFocus();
//         this->SetSel(iFind, iFind + iLen);
//     }
  //  g_pDlgFind->DoAfterFind();
//}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    // TODO: Change any attributes of the DC here
    if ((CTLCOLOR_EDIT == nCtlColor)
        || (CTLCOLOR_STATIC == nCtlColor))
    {
        pDC->SetTextColor(m_ColorText);
        pDC->SetBkColor(RGB(255,255,255));
        // Modified by LiuKai(00119002) for DTS2010082600255, 2010/9/1
        // HBRUSH b=CreateSolidBrush(RGB(255,255,255));
        // return b;
        return m_hbrEditColor;
        // End of Modify by LiuKai(00119002), 2010/9/1
    }
    
    // TODO: Return a non-NULL brush if the parent's handler should not be called
    return NULL;
}

void CMyEdit::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnRButtonUp(nFlags, point);

//     if (NULL == m_pNewMenu)
//     {
//         CEdit::OnRButtonUp(nFlags, point);
//     }
//     else
//     {
//         this->ClientToScreen(&point);
//         CMenu *pSubMenu = m_pNewMenu->GetSubMenu(0);
//         if (NULL != pSubMenu)
//         {
//             pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
//                                      point.x, point.y, this, NULL);
// 
//         }
//     }
}

// void CMyEdit::OnMenuitemOutputCopy() 
// {
//     this->Copy();
// }
// 
// void CMyEdit::OnMenuitemOutputClear() 
// {
//     m_pFindView->Clear();
// }
// 
// void CMyEdit::OnMenuitemOutputFind() 
// {
//     g_pDlgFind->ShowWindow(SW_SHOW);
//     g_pDlgFind->SetFocus();
// }
// 
// void CMyEdit::OnMenuitemInputCopy() 
// {
//     this->Copy();
// }
// 
// void CMyEdit::OnMenuitemInputCut() 
// {
//     this->Cut();
// }
// 
// void CMyEdit::OnMenuitemInputFind() 
// {
//     g_pDlgFind->ShowWindow(SW_SHOW);
//     g_pDlgFind->SetFocus();
// }
// 
// void CMyEdit::OnMenuitemInputPaste() 
// {
//     this->Paste();
// }
// 
// void CMyEdit::OnMenuitemInputSelectAll() 
// {
//     this->SetSel(0,-1);
// }
// 
// void CMyEdit::OnMenuitemInputUndo() 
// {
//     this->Undo();
// }
// 
// void CMyEdit::OnMenuitemFont() 
// {
//     CFontDialog dlgFont(&m_logNewFont);
//     dlgFont.m_cf.rgbColors = m_ColorText;
//     
//     if (IDOK == dlgFont.DoModal())
//     {
//         m_ColorText = dlgFont.GetColor();
//         dlgFont.GetCurrentFont(&m_logNewFont);
//         m_logNewFont.lfHeight = ::abs(m_logNewFont.lfHeight);
//         this->SetMyEditFont();
//     }
// }

void CMyEdit::SetMyEditFont()
{
    m_objNewFont.Detach();
    
    BOOL bRet = m_objNewFont.CreateFontIndirect(&m_logNewFont);
    ASSERT(bRet);
    
    if (bRet)
    {
        this->SetFont(&m_objNewFont);
    }
}

//#include <windef.h>

//extern void VGlobalOutput(const char *pFormat, ...);
BOOL CMyEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    // VGlobalOutput("CMyEdit::OnMouseWheel nFlags=%d, zDelta=%d\r\n", nFlags, zDelta);
    if (MK_CONTROL == nFlags)
    {
        const LONG lfMinHeight = 8;
        const LONG lfMaxHeight = 68;
        LONG lfHeight = m_logNewFont.lfHeight;
        if (zDelta > 0)
        {
            ++lfHeight;
        } 
        else
        {
            --lfHeight;
        }

		//modify yinwenhui 2011-8-17
		//         lfHeight = max(lfHeight, lfMinHeight);
		//         lfHeight = min(lfHeight, lfMaxHeight);
		lfHeight = lfHeight > lfMinHeight ? lfHeight : lfMinHeight;
        lfHeight = lfHeight < lfMaxHeight ? lfHeight : lfMaxHeight;
		//end yinwenhui
        m_logNewFont.lfHeight = lfHeight;
        // VGlobalOutput("lfHeight=%d\r\n", m_logNewFont.lfHeight);
        this->SetMyEditFont();
    }
    return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

// LRESULT CMyEdit::OnRegisteredMouseWheel( WPARAM wParam, LPARAM lParam )
// {
// 	return 0;
// }



void CMyEdit::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//ModifyStyle(0, SS_NOTIFY, TRUE);
	CEdit::PreSubclassWindow();
}