
/*MINIGUi sample mitwindows*/
/*gcc -o metext metext.c -lminigui -lpthread -lpng -ljpeg -lmgext*/
#include <stdio.h>
#include <string.h>
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "main_common.h"

#define mydebug(format, ...) printf("[%s:%s:%d]:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)
typedef int (*my_window_proc_t)(HWND, int, WPARAM, LPARAM);

static HWND hwnd_main = 0;

//control

#define IDC_BUTTON1 (100)
#define IDC_BUTTON2 (IDC_BUTTON1 + 1)
#define IDC_BUTTON3 (IDC_BUTTON2 + 1)
#define IDC_BUTTON4 (IDC_BUTTON3 + 1)
//#define IDC_CHARS (IDC_BUTTON2 + 1)

//#define MAIN_WIN_STYLE_MAIN (WS_CAPTION | WS_BORDER | WS_SYSMENU)
#define MAIN_WIN_STYLE_MAIN (WS_NONE)
static void init_main_window_create_info (PMAINWINCREATE pcreate_info, char *caption, my_window_proc_t my_window_proc,int lx, int ty, int rx, int by, HWND  hHosting) {
	memset(pcreate_info, 0, sizeof(*pcreate_info));

	pcreate_info->dwStyle = MAIN_WIN_STYLE_MAIN;
	pcreate_info->dwExStyle = WS_EX_NONE;
	pcreate_info->spCaption = caption;
	pcreate_info->hMenu = 0;
	pcreate_info->hCursor = GetSystemCursor(0);
	pcreate_info->hIcon = 0;
	pcreate_info->MainWindowProc = my_window_proc;
	pcreate_info->lx = lx;
	pcreate_info->ty = ty;
	pcreate_info->rx = rx;
	pcreate_info->by = by;
	pcreate_info->iBkColor = COLOR_lightwhite;
	pcreate_info->dwAddData = 0;
	pcreate_info->hHosting = hHosting;
}

#define WIN_STYLE_BUTTON_CHECKED (WS_CHILD | BS_PUSHBUTTON | BS_CHECKED | WS_VISIBLE)
static void my_create(HWND hWnd, int message, WPARAM wParam, LPARAM lParam) {
	char *msg_code = trans_msg_code(message);
	int button1_id = 0;
	int button2_id = 0;

	mydebug("hWnd:%x, message:%s(%x), wParam:%x, lParam:%lx\n", hWnd, msg_code, message, wParam, lParam);

	if(0 == hwnd_main) {
		CreateWindow(CTRL_BUTTON, "button1", WIN_STYLE_BUTTON_CHECKED, IDC_BUTTON1, 160, 160, 80, 20, hWnd, 0);
		CreateWindow(CTRL_BUTTON, "button2", WIN_STYLE_BUTTON_CHECKED, IDC_BUTTON2, 400, 160, 80, 20, hWnd, 0);
	} else {
		CreateWindow(CTRL_BUTTON, "button3", WIN_STYLE_BUTTON_CHECKED, IDC_BUTTON3, 160, 160, 80, 20, hWnd, 0);
		CreateWindow(CTRL_BUTTON, "button4", WIN_STYLE_BUTTON_CHECKED, IDC_BUTTON4, 400, 160, 80, 20, hWnd, 0);
	}

}

static create_main_window_child(HWND hWnd, my_window_proc_t my_window_proc) {
	MAINWINCREATE create_info;
	static HWND hwnd_child = NULL;

	init_main_window_create_info(&create_info, "child", my_window_proc, 0, 240, 640, 480, hWnd);

	hwnd_child = CreateMainWindow(&create_info);
	if (hwnd_child == HWND_INVALID) {
		return -1;
	}

	mydebug("hwnd_child:%x\n", hwnd_child);

	ShowWindow(hwnd_child, SW_SHOWNORMAL);

	mydebug("The child window showed.\n");
}

static int my_command(HWND hWnd, WPARAM wParam, LPARAM lParam, my_window_proc_t my_window_proc) {
	int id = LOWORD(wParam);
	int nc = HIWORD(wParam);

	mydebug("id:%x, nc:%s(%x)\n", id, trans_button_msg_code(nc), nc);

	switch (id) {
		case IDC_BUTTON1:
			if(nc == BN_CLICKED) {
				create_main_window_child(hWnd, my_window_proc);
			}
			break;
		case IDC_BUTTON2:
			if(nc == BN_CLICKED) {
				SendMessage(hWnd, MSG_CLOSE, 0, 0L);
			}
			break;
		case IDC_BUTTON3:
			if(nc == BN_CLICKED) {
				create_main_window_child(hWnd, my_window_proc);
			}
			break;
		case IDC_BUTTON4:
			if(nc == BN_CLICKED) {
				SendMessage(hWnd, MSG_CLOSE, 0, 0L);
			}
			break;
		default:
			break;
	}

	return 0;
}

static int my_paint(HWND hWnd) {
	HDC hdc;
	RECT rect;
	char *hello_str = "Hello MiniGUI!";

	hdc = BeginPaint(hWnd);
	GetClientRect(hWnd, &rect);
	DrawText(hdc, hello_str, -1, &rect, DT_LEFT);
	EndPaint(hWnd, hdc);
	mydebug("l:%d, t:%d, r:%d, b:%d\n", rect.left, rect.top, rect.right, rect.bottom);
}

static int my_window_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam) {
	static int debug = 0;
	static char key = 0;
	char *msg_code = trans_msg_code(message);

	if(debug != 0) {
		if(msg_code) {
			mydebug("hWnd:%x, message:%s(%x), wParam:%x, lParam:%lx\n", hWnd, msg_code, message, wParam, lParam);
		} else {
			mydebug("hWnd:%x, message:%s(%x), wParam:%x, lParam:%lx\n", hWnd, msg_code, message, wParam, lParam);
		}
	}

	switch (message) {
		case MSG_CREATE:
			my_create(hWnd, message, wParam, lParam);
			break;
		case MSG_COMMAND:
			my_command(hWnd, wParam, lParam, my_window_proc);
			break;
		case MSG_PAINT:
			my_paint(hWnd);
			break;
		case MSG_CLOSE:
			DestroyMainWindow(hWnd);
			if(hWnd == hwnd_main) {
				PostQuitMessage(hWnd);
			}
			break;
		case MSG_CHAR:
			key = (char)wParam;
			if(key == 'd') {
				debug = ~debug;
			}
			break;
		default:
			break;
	}

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

//modal
int MiniGUIMain (int argc, const char* argv[]) {
	MSG msg;
	MAINWINCREATE create_info;

#ifdef _LITE_VERSION
	SetDesktopRect(0, 0, 1024, 768);
#endif

	init_main_window_create_info(&create_info, "main", my_window_proc, 0, 0, 640, 240, HWND_DESKTOP);

	hwnd_main = CreateMainWindow(&create_info);
	if (hwnd_main == HWND_INVALID) {
		return -1;
	}

	mydebug("hwnd_main:%x\n", hwnd_main);

	ShowWindow(hwnd_main, SW_SHOWNORMAL);

	mydebug("The main window showed.\n");

	while (GetMessage(&msg, hwnd_main))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	MainWindowThreadCleanup (hwnd_main);


	return 0;
}
#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

