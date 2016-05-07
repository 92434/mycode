#include <stdio.h>   
  
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
  
#define _FLAT_WINDOW_STYLE   
  
  
/***********************************************************************  
*** 函数原 型：static int ColorEditConProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)  
              static BOOL RegisterColorEditControl (void)  
              static void UnregisterColorEditControl (void)  
*** 参数说明：  
*** 返回值  ：  
*** 创建人  ：王敏敏  
*** 最后修改：  
*** 描述    ："边框颜色可变编辑框"控件，这里利用Minigui中的自定 义控件实现。包括：  
              其回调函数(ColorEditConProc);  
              注册这个控件 (RegisterColorEditControl);  
              注销这个控件 (UnregisterColorEditControl)  
************************************************************************/   
static   int  ColorEditConProc ( HWND  hwnd,  int  message,  WPARAM  wParam,  LPARAM  lParam)  
{     
         HDC  hdc;      
        RECT rc;                  // rc 为文本矩形   
      
    GetClientRect(hwnd, &rc);  // 取 得控件的大小       
      
     switch (message)  
    {  
         case  MSG_CREATE:  
            SetWindowBkColor(hwnd, COLOR_black);  
         break ;  
          
         case  MSG_PAINT:  
            hdc = BeginPaint (hwnd);      
             //SetBkMode(hdc, BM_TRANSPARENT);    // 让 文本框背景透明                    
              
             // 设置边框颜色-浅蓝色，并绘制边框     
            SetPenColor(hdc, RGB2Pixel (hdc, 11, 244 , 244));                          
            Rectangle (hdc, rc.left, rc.top, rc.right-1, rc.bottom-1);   
             // 设置文本颜色——浅蓝色，文本背景色 ——黑色   
            SetTextColor(hdc, RGB2Pixel (hdc, 11, 244 , 244));            
            SetBkColor(hdc, COLOR_black);                                 
            rc.right = rc.right-2;   // 文字到右边框留的距离为3   
             // 输出文本内容，右对齐、上下居中显示   
           DrawText(hdc, GetWindowCaption (hwnd), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);  
            EndPaint (hwnd, hdc);  
             break ;                    
         
                    
         case  MSG_SETTEXT:   // 此消息是为了使得SetDlgItemText()函数(此函数会发出MSG_SETTEXT消息)可用   
               // 将设置的文本保存到caption 中   
              SetWindowCaption (hwnd, ( char *)lParam);   
                
            hdc = GetClientDC (hwnd);  
             // 设置边框颜色-绿色，并绘制边 框                            
            SetPenColor(hdc, RGB2Pixel (hdc, 9, 225 , 24));                 
            Rectangle (hdc, rc.left, rc.top, rc.right, rc.bottom-1);                 
             // 设置文本颜色——浅蓝色，文本背景色 ——黑色                      
            SetTextColor(hdc, RGB2Pixel (hdc, 11, 244 , 244));            
            SetBkColor(hdc, COLOR_black);                                 
            rc.right = rc.right-3;   // 文字到右边框留的距离为3                        
             // 用黑色刷新文字区域   
            SetBrushColor(hdc, 0);              
            FillBox (hdc, rc.left+1, rc.top+1, rc.right-rc.left-1, rc.bottom-rc.top-2);  // 加1减1目的是为了防止把边框刷掉   
             // 输出文本内容，右对齐、上下居中显示   
            DrawText(hdc, ( char *)lParam, -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE);  
            ReleaseDC (hdc);        
             break ;        
    }  
      
     // DefaultControlProc()调用 DefaultMainWinProc(),其内包含对MSG_GETTEXT消息的处理   
     return  DefaultControlProc (hwnd, message, wParam, lParam);    
}  
  
static   BOOL  RegisterColorEditControl ( void )  
{  
    WNDCLASS MyClass;  
  
    MyClass.spClassName =  "coloredit" ;  
    MyClass.dwStyle     = WS_NONE;  
    MyClass.dwExStyle   = WS_EX_NONE;  
    MyClass.hCursor     = GetSystemCursor (IDC_ARROW);  
    MyClass.iBkColor    = COLOR_black;  
    MyClass.WinProc     = ColorEditConProc;  
  
     return  RegisterWindowClass (&MyClass);  
}  
  
static   void  UnregisterColorEditControl ( void )  
{  
    UnregisterWindowClass ( "coloredit" );  
}  
  
  
  
/***********************************************************************  
*** 函数原 型：static int MenuWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)  
*** 参数说明：  
*** 返回值  ：  
*** 创建人  ：王敏敏  
*** 最后修改：  
*** 描述    ：主窗口回调函数  
************************************************************************/   
static   int  MenuWinProc( HWND  hWnd,  int  message,  WPARAM  wParam,  LPARAM  lParam)  
{    
         HWND  hw;  
     switch  (message) {  
         case  MSG_CREATE:  
            CreateWindow ( "coloredit" ,  "brath rate" ,   
                    WS_VISIBLE, 101,   
                    80, 100, 62, 20,   
                    hWnd, 0);   
            hw=CreateWindow (CTRL_TRACKBAR, "" ,  
                        WS_VISIBLE | TBS_NOTIFY | TBS_NOTICK,  
                        102,  
                        10,10,280,18,hWnd,0);  
            SendMessage(hw,TBM_SETRANGE,1,100);  
             break ;  
              
         case  MSG_CLOSE:  
            DestroyMainWindow (hWnd);  
             // 注销自定义控件   
            UnregisterColorEditControl ();  
            PostQuitMessage (hWnd);  
             break ;  
    }  
  
     return  DefaultMainWinProc(hWnd, message, wParam, lParam);  
}  
  
  
/***********************************************************************  
*** 函数原 型：int MiniGUIMain (int argc, const char* argv[])  
*** 参数说明：  
*** 返回值  ：  
*** 创建人  ：王敏敏  
*** 最后修改：  
*** 描述    ：主程序入口函数  
************************************************************************/   
int  MiniGUIMain ( int  argc,  const   char * argv[])  
{  
    MSG Msg;  
     HWND  hMainWnd;  
    MAINWINCREATE CreateInfo;  
  
#ifdef _LITE_VERSION   
    SetDesktopRect(0, 0, 578, 555);  
#endif   
      
     // 注册自定义控件   
    RegisterColorEditControl();  
  
    CreateInfo.dwStyle = WS_VISIBLE ;  
    CreateInfo.dwExStyle = WS_EX_NONE;  
    CreateInfo.spCaption =  "" ;  
    CreateInfo.hMenu = 0;  
    CreateInfo.hCursor = GetSystemCursor(0);  
    CreateInfo.hIcon = 0;  
    CreateInfo.MainWindowProc = MenuWinProc;  
    CreateInfo.lx = 0;  
    CreateInfo.ty = 0;  
    CreateInfo.rx = 578;  
    CreateInfo.by = 555;  
    CreateInfo.iBkColor = COLOR_black;  
    CreateInfo.dwAddData = 0;  
    CreateInfo.hHosting = HWND_DESKTOP;  
      
    hMainWnd = CreateMainWindow (&CreateInfo);  
      
     if  (hMainWnd == HWND_INVALID)  
         return  -1;  
  
    ShowWindow(hMainWnd, SW_SHOWNORMAL);  
  
     while  (GetMessage(&Msg, hMainWnd)) {  
        TranslateMessage(&Msg);  
        DispatchMessage(&Msg);  
    }  
  
    MainWindowThreadCleanup (hMainWnd);  
  
     return  0;  
}  
  
#ifndef _LITE_VERSION   
#include <minigui/dti.c>   
#endif   
