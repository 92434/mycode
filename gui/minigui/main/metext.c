
/*MINIGUi   sample mitwindows*/
/*gcc -o metext metext.c   -lminigui -lpthread -lpng -ljpeg   -lmgext*/
#include <stdio.h>
#include <string.h>
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#define IDC_BUTTON1                102   //按钮
#define IDC_BUTTON2                103   //按钮
#define IDC_BUTTON3                104   //按钮
#define IDC_BUTTON4                105   //按钮
#define IDC_FANHUI                 106   //按钮
#define IDC_MAINMENU               107   //菜单

static char *hello_str = "Hello MiniGUI!";

static HWND hMainWnd; //窗口句柄

static DLGTEMPLATE Dlgweihu= //DLGTEMPLATE 对话框模板
{
	WS_BORDER | WS_CAPTION,
	WS_EX_NONE,
	0, 0, 320, 240,
	"control",
	0, 0,
	2, NULL,
	0
};static CTRLDATA Ctrlweihu[] = //对话框中的组件数组
{
	{
		"button",
		WS_VISIBLE | WS_TABSTOP | WS_GROUP, 
		200, 150, 50, 30,
		IDC_FANHUI,
		"exit",
		0
	},   {
		"button",
		WS_VISIBLE | WS_TABSTOP | WS_GROUP, 
		20, 150, 50, 30,
		IDC_MAINMENU,
		"mainmenu",
		0
	},

};//=====================================================回调函数==DialogBoxProc3

static int DialogBoxProc3 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case MSG_INITDIALOG:
			return 1;
		case MSG_COMMAND:
			{
				int id = LOWORD (wParam);
				int code = HIWORD (wParam);
				switch (wParam)
				{
					case IDC_MAINMENU:
						// if (id==IDC_MAINMENU && code==BN_CLICKED)
						if (id == IDC_MAINMENU && code == BN_DBLCLK)
							// if (id==IDC_MAINMENU && code==BN_PUSHED)
						{
							SetWindowText(hDlg,"Hello world!");
						}
						break;
					case IDC_FANHUI:
					case IDOK:
					case IDCANCEL:
						EndDialog (hDlg, wParam);//结束对话框
						break;
				}

				break;
			}
	}  
	return DefaultDialogProc (hDlg, message, wParam, lParam);
}
//=====================================================================

static void testDialogBox3 (HWND hWnd)
{
	Dlgweihu.controls = Ctrlweihu; /*dialog   controls */

	DialogBoxIndirectParam (&Dlgweihu, hWnd, DialogBoxProc3, 0L);
}
//下边是第二级窗口的代码******************************************************************

static HWND hMainWnd1 = HWND_INVALID;

static int InitOrderProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)   //第二及处理消息
{

	switch (message) {
		case MSG_CREATE:                 
			CreateWindow (CTRL_BUTTON,
					"return to top window",
					WS_CHILD | BS_PUSHBUTTON | BS_CHECKED | WS_VISIBLE,
					IDC_BUTTON3, /*button 3*/
					10, 70, 80, 20, hWnd, 0);             

			CreateWindow (CTRL_BUTTON,
					"to 3th window",
					WS_CHILD | BS_PUSHBUTTON | BS_CHECKED | WS_VISIBLE,
					IDC_BUTTON4, /*Button 4 */
					100, 70, 80, 20, hWnd, 0);  
			break;  

		case MSG_COMMAND:
			{//-----------------Event
				int id    = LOWORD(wParam);
				int code = HIWORD(wParam);
				char buffer [256];
				sprintf (buffer, "ID: %d, Code: %x", id, code);
				switch (id)
				{
					case (IDC_BUTTON3):             
						printf ("hWnd:%x, id:%d, code:%x\n", hWnd, id, code);
						//SendMessage (hWnd, MSG_CLOSE, 0, 0);  
						break;              
					case (IDC_BUTTON4):
						if(code == BN_CLICKED)

					{ 
						printf ("hWnd:%x, id:%d, code:%x\n", hWnd, id, code);
						testDialogBox3 (hWnd);               
					}
						break;    
					default:
						break;
				}                      
			}
			break;

		case MSG_DESTROY:
			DestroyAllControls (hWnd);
			hMainWnd1 = HWND_INVALID;
			return 0;  

		case MSG_CLOSE:
			DestroyMainWindow (hWnd);
			MainWindowCleanup (hWnd);
			return 0;
	}   
	return DefaultMainWinProc (hWnd, message, wParam, lParam);
}
//=============================================================================

static void InitCreateInfoTWO (PMAINWINCREATE pCreateInfo)
{
	pCreateInfo->dwStyle = WS_CHILD | WS_BORDER | WS_VISIBLE|WS_CAPTION;
	pCreateInfo->dwExStyle = WS_EX_NONE;
	pCreateInfo->spCaption = "to 2nd window";
	pCreateInfo->hMenu = 0;
	pCreateInfo->hCursor = GetSystemCursor(1);
	pCreateInfo->hIcon = 0;
	pCreateInfo->MainWindowProc = InitOrderProc; //窗体回调函数
	pCreateInfo->lx = 0;
	pCreateInfo->ty = 0;
	pCreateInfo->rx = 320;
	pCreateInfo->by = 240;
	pCreateInfo->iBkColor = COLOR_lightwhite;
	pCreateInfo->dwAddData = 0;
	pCreateInfo->hHosting = hMainWnd;
}
//=============================================================================
void orderdesk (HWND hwnd)
{
	MAINWINCREATE CreateInfo2; //新建一个窗口

	InitCreateInfoTWO (&CreateInfo2);
	hMainWnd1 = CreateMainWindow (&CreateInfo2);//建立窗口  

	if (hMainWnd1 != HWND_INVALID) 
	{
		ShowWindow (hMainWnd1, SW_SHOWNORMAL); //显示窗口
		return;
	}
} 
//=====================================================================================
//************************************************************************主窗口回调函数
static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rc;
	switch (message) {      
		case MSG_CREATE:                                   
			CreateWindow (CTRL_BUTTON,//============CTRL_BUTTON 按钮
					"2nd window",
					WS_CHILD | BS_PUSHBUTTON |BS_CHECKED | WS_VISIBLE,
					IDC_BUTTON1,//=========================按钮ID=IDC_BUTTON1
					10, 70, 80, 20, hWnd, 0);           

			CreateWindow (CTRL_BUTTON,//============CTRL_BUTTON 按钮
					"close",
					WS_CHILD | BS_PUSHBUTTON |BS_CHECKED | WS_VISIBLE,
					IDC_BUTTON2,//=========================按钮ID=IDC_BUTTON2
					100, 70, 80, 20, hWnd, 0);
			break;  

		case MSG_COMMAND:
			{
				int id    = LOWORD(wParam);//低
				int code = HIWORD(wParam);//高
				//printf ("Notification from buttons: ID: %d, Code: %x\n", id, code);          
				switch (id)
				{

					case IDC_BUTTON1:
						printf ("hWnd:%x, id:%d, code:%x\n", hWnd, id, code);
						orderdesk(hWnd);   //======================================函数调用
						break;

					case IDC_BUTTON2:
						printf ("hWnd:%x, id:%d, code:%x\n", hWnd, id, code);
						SendMessage (hWnd, MSG_CLOSE, 0, 0L);//发送消息
						break;              
					default:
						break;
				}
				break;
			}      
			break;        



		case MSG_PAINT://绘
			hdc = BeginPaint (hWnd);
			GetClientRect (hWnd, &rc);
			DrawText (hdc, hello_str, -1, &rc, DT_LEFT);
			EndPaint (hWnd, hdc);

			break;    

		case MSG_CLOSE:
			DestroyMainWindow (hWnd);
			PostQuitMessage (hWnd);
			break;
	}    


	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}
//===========================================================================主窗口


static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{

	memset(pCreateInfo, 0, sizeof(*pCreateInfo));
	pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU;
	pCreateInfo->dwExStyle = WS_EX_NONE;
	pCreateInfo->spCaption = "1st window";
	//pCreateInfo->hMenu = 0;
	pCreateInfo->hCursor = GetSystemCursor (0);
	//pCreateInfo->hIcon = 0;
	pCreateInfo->MainWindowProc = HelloWinProc;//窗口回调函数
	pCreateInfo->lx = 0; 
	pCreateInfo->ty = 0;
	pCreateInfo->rx = 320;
	pCreateInfo->by = 240;
	//pCreateInfo->iBkColor = COLOR_lightwhite; 
	//pCreateInfo->dwAddData = 0;
	//pCreateInfo->hHosting = HWND_DESKTOP;//======1   
}//========================================================================

int MiniGUIMain (int argc, const char* argv[])
{
	MSG Msg;
	MAINWINCREATE CreateInfo;

#ifdef _LITE_VERSION
	SetDesktopRect(0, 0, 1024, 768);
#endif


	InitCreateInfo (&CreateInfo);  

	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
		return -1;

	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	printf ("The main window showed.\n");
	//==================================================================
	while (GetMessage(&Msg, hMainWnd))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}    

	MainWindowThreadCleanup (hMainWnd);    


	return 0;
}
#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

