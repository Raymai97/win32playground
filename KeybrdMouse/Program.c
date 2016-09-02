#define UNICODE
#define _UNICODE
#define _WIN32_WINNT	0x0400
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>

#define CLASS_NAME	_T("KeybrdMouse Win32")
#define WND_TITLE	_T("KeyboardMouse")
#define WND_STYLE	(WS_OVERLAPPEDWINDOW)
#define NI_MSG		(WM_USER)
#define PRESSING(x)	(GetAsyncKeyState(x) < 0)

HWND hWnd;
NOTIFYICONDATA ni;
UINT taskbar_created_msg;

// momentum
int mmt_x, mmt_y, mmt_max = 10;
// modifier key
BOOL ing_WINLOGO;
// navigate keys
BOOL ing_left, ing_right, ing_up, ing_down;
// keys of MouseDown & MouseUp
// (Left, Middle, Right)
BOOL ing_Q, ing_W, ing_E;
// was MouseDown already?
BOOL was_Q, was_W, was_E;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK HookKeybrdProc(int code, WPARAM w, LPARAM l);
VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);
void OnRButtonUp(HWND, int x, int y, UINT flags);
BOOL OnPressing(DWORD vk);
BOOL OnReleasing(DWORD vk);

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR lpCmdLine, int nCmdShow) {
	HANDLE hMutex;
	HHOOK hhkKeybrd;
	WNDCLASSEX wcex;
	MSG msg;
	// Detect prev instance
	hMutex = CreateMutex(NULL, TRUE, CLASS_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, _T("The program is running already."), WND_TITLE, 
			MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	// RegisterClass & CreateWindow
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.lpszClassName = CLASS_NAME;
	wcex.lpfnWndProc = WndProc;
	if (!RegisterClassEx(&wcex)) { return 1; }
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WND_TITLE,
		WND_STYLE,
		0, 0, 0, 0,
		NULL, NULL,
		NULL, NULL);
	if (!hWnd) { return 2; }
	// Notify Icon
	ni.cbSize = sizeof(ni);
	ni.uCallbackMessage = NI_MSG;
	ni.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	ni.hWnd = hWnd;
	ni.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_tcscpy(ni.szTip, _T("KeyboardMouse"));
	Shell_NotifyIcon(NIM_ADD, &ni);
	taskbar_created_msg = RegisterWindowMessage(_T("TaskbarCreated"));
	// Timer adjust cursor position, simulate MouseDown/MouseUp
	// HookKeybrdProc eat handled keystroke
	SetTimer(hWnd, 0, 0, TimerProc);
	hhkKeybrd = SetWindowsHookEx(WH_KEYBOARD_LL, HookKeybrdProc, hInst, 0);
	// ...
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Shell_NotifyIcon(NIM_DELETE, &ni);
	UnhookWindowsHookEx(hhkKeybrd);
	ReleaseMutex(hMutex);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
	if (msg == NI_MSG) {
		if (l == WM_RBUTTONUP) {
			HANDLE_WM_RBUTTONUP(hWnd, w, l, OnRButtonUp);
		}
	}
	else if (msg == taskbar_created_msg) {
		Shell_NotifyIcon(NIM_ADD, &ni);
	}
	else if (msg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else {
		return DefWindowProc(hWnd, msg, w, l);
	}
	return 0;
}

void OnRButtonUp(HWND hWnd, int x, int y, UINT flags) {
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MFS_DISABLED, 0, WND_TITLE);
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_BYCOMMAND, 1, _T("About..."));
	AppendMenu(hMenu, MF_BYCOMMAND, 2, _T("Exit"));
	{
		int cmd;
		POINT m;
		GetCursorPos(&m);
		SetForegroundWindow(hWnd);
		cmd = TrackPopupMenu(
			hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, m.x, m.y, 0, hWnd, NULL);
		if (cmd == 1) {
			MessageBox(NULL,
				_T("Version 1.0 . by Raymai97 . using ANSI C\n\n")
				_T("While holding WIN key...\n")
				_T("Press Q for left mouse button.\n")
				_T("Press W for middle mouse button.\n")
				_T("Press E for right mouse button.\n")
				_T("Press arrow key for navigation."),
				WND_TITLE,
				MB_OK | MB_ICONINFORMATION);
		}
		else if (cmd == 2) {
			DestroyWindow(hWnd);
		}
	}
	DestroyMenu(hMenu);
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
	POINT m;
	ing_WINLOGO = PRESSING(VK_LWIN) || PRESSING(VK_RWIN);
	// Adjust Momentum
	if (ing_left) {
		if (mmt_x > -mmt_max) { --mmt_x; }
	}
	else if (ing_right) {
		if (mmt_x < mmt_max) { ++mmt_x; }
	}
	else if (mmt_x) {
		mmt_x += (mmt_x < 0 ? 1 : -1);
	}
	if (ing_up) {
		if (mmt_y > -mmt_max) { --mmt_y; }
	}
	else if (ing_down) {
		if (mmt_y < mmt_max) { ++mmt_y; }
	}
	else if (mmt_y) {
		mmt_y += (mmt_y < 0 ? 1 : -1);
	}
	// Apply Momentum on CursorPos
	GetCursorPos(&m);
	m.x += mmt_x;
	m.y += mmt_y;
	SetCursorPos(m.x, m.y);
	// Do MouseDown/MouseUp
	if (!ing_Q && was_Q) {
		mouse_event(MOUSEEVENTF_LEFTUP, m.x, m.y, 0, 0);
		was_Q = FALSE;
	}
	else if (ing_Q && !was_Q) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, m.x, m.y, 0, 0);
		was_Q = TRUE;
	}
	if (!ing_W && was_W) {
		mouse_event(MOUSEEVENTF_MIDDLEUP, m.x, m.y, 0, 0);
		was_W = FALSE;
	}
	else if (ing_W && !was_W) {
		mouse_event(MOUSEEVENTF_MIDDLEDOWN, m.x, m.y, 0, 0);
		was_W = TRUE;
	}
	if (!ing_E && was_E) {
		mouse_event(MOUSEEVENTF_RIGHTUP, m.x, m.y, 0, 0);
		was_E = FALSE;
	}
	else if (ing_E && !was_E) {
		mouse_event(MOUSEEVENTF_RIGHTDOWN, m.x, m.y, 0, 0);
		was_E = TRUE;
	}
}

LRESULT CALLBACK HookKeybrdProc(int code, WPARAM w, LPARAM l) {
	BOOL fEatKey = FALSE;
	DWORD vk = ((PKBDLLHOOKSTRUCT)l)->vkCode;
	if (code == HC_ACTION) {
		switch (w) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				fEatKey = OnPressing(vk);
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				fEatKey = OnReleasing(vk);
				break;
		}
	}
	return (fEatKey ? TRUE : CallNextHookEx(NULL, code, w, l));
}

BOOL OnPressing(DWORD vk) {
	if (ing_WINLOGO) {
		if (vk == VK_LEFT) { ing_left = TRUE; }
		else if (vk == VK_RIGHT) { ing_right = TRUE; }
		if (vk == VK_UP) { ing_up = TRUE; }
		else if (vk == VK_DOWN) { ing_down = TRUE; }
		if (vk == 'Q') { ing_Q = TRUE; }
		else if (vk == 'W') { ing_W = TRUE; }
		else if (vk == 'E') { ing_E = TRUE; }
		return (
			ing_left || ing_right ||
			ing_up || ing_down ||
			ing_Q || ing_W || ing_E );
	}
	return FALSE;
}

BOOL OnReleasing(DWORD vk) {
	if (vk == VK_LEFT) { ing_left = FALSE; }
	if (vk == VK_RIGHT) { ing_right = FALSE; }
	if (vk == VK_UP) { ing_up = FALSE; }
	if (vk == VK_DOWN) { ing_down = FALSE; }
	if (vk == 'Q') { ing_Q = FALSE; }
	if (vk == 'W') { ing_W = FALSE; }
	if (vk == 'E') { ing_E = FALSE; }
	return FALSE;
}
