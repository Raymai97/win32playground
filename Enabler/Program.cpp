#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define CLASS_NAME   _T("MaiSoft.Enabler")
#define WND_TITLE    _T("Enabler")
#define WND_STYLE    (WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX))

#define CX(rc)		(rc.right - rc.left)
#define CY(rc)		(rc.bottom - rc.top)
#define AUTODPI(x)	(MulDiv(x, dpi_y, 96))

int dpi_y;
HFONT hFont;
TCHAR text[200];
volatile bool paused;
volatile HWND hWndTarget;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);
BOOL CALLBACK EnumChildProc(HWND, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, int nCmdShow)
{
	/* Detect System DPI */
	HDC hdc0 = GetDC(NULL);
	dpi_y = GetDeviceCaps(hdc0, LOGPIXELSY);
	ReleaseDC(NULL, hdc0);
	/* ... */
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = CLASS_NAME;
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    if (!RegisterClassEx(&wcex)) { return 1; }
    HWND hWnd = CreateWindowEx(
        WS_EX_TOPMOST,
        CLASS_NAME,
        WND_TITLE,
        WND_STYLE,
        CW_USEDEFAULT, 0,
        AUTODPI(250), AUTODPI(140),
        NULL, NULL,
        NULL, NULL);
    if (!hWnd) { return 2; }
    ShowWindow(hWnd, nCmdShow);
	/* Init font */
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy(lf.lfFaceName, _T("Tahoma"));
	lf.lfHeight = -MulDiv(8, dpi_y, 72);
	hFont = CreateFontIndirect(&lf);
    /* SetTimer and immediately start */
    SetTimer(hWnd, 0, 1000, TimerProc);
	TimerProc(hWnd, 0, 0, 0);
	/* ... */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

VOID CALLBACK TimerProc(HWND hWnd, UINT, UINT, DWORD) {
	if (!paused)
    {
        hWndTarget = GetForegroundWindow();
        EnumChildWindows(hWndTarget, EnumChildProc, 0);
    }
    InvalidateRect(hWnd, NULL, TRUE);
}

BOOL CALLBACK EnumChildProc(HWND hChild, LPARAM lParam)
{
    EnableWindow(hChild, TRUE);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (w == VK_ESCAPE) { DestroyWindow(hWnd); break; }
        if (w != VK_RETURN) { break; }
    case WM_LBUTTONDBLCLK:
        paused = !paused;
		InvalidateRect(hWnd, NULL, TRUE);
        break;
	case WM_RBUTTONUP:
		MessageBox(hWnd, _T("Written by Raymai97."), WND_TITLE,
			MB_OK | MB_ICONINFORMATION);
		break;
    case WM_PAINT:
        {
			RECT rc;
			GetClientRect(hWnd, &rc);
            PAINTSTRUCT ps;
            ZeroMemory(&ps, sizeof(ps));
            HDC hdc = BeginPaint(hWnd, &ps);
			HGDIOBJ old_hfo = SelectObject(hdc, hFont);

            SetBkMode(hdc, TRANSPARENT);
			_stprintf(text,
				_T("Status: %s \n\n")
				_T("Target HWND: 0x%p \n\n")
				_T("Double Click / ENTER to toggle state.\n")
				_T("ESC to quit."),
				paused ? _T("Paused") : _T("Running"),
				hWndTarget);
			RECT rcText = {0};
            DrawText(hdc, text, -1, &rcText, DT_CALCRECT);
			rcText.right += rcText.left = (CX(rc) - CX(rcText)) / 2;
			rcText.bottom += rcText.top = (CY(rc) - CY(rcText)) / 2;				
			DrawText(hdc, text, -1, &rcText, 0);
			
			SelectObject(hdc, old_hfo);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, w, l);
    }
    return 0;
}
