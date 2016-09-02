#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

#define MY_CLASS_NAME   _T("Enabler by MaiSoft")
#define MY_WND_TITLE    _T("Enabler")
#define MY_WND_STYLE    (WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX))

volatile bool paused, exiting;
volatile HANDLE hEvent;
volatile HWND hWndTarget;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ThreadProc(LPVOID);
BOOL CALLBACK EnumChildProc(HWND, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR lpCmdLine, int nCmdShow)
{
    paused = false;
    exiting = false;

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = MY_CLASS_NAME;
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    if (!RegisterClassEx(&wcex)) { return 1; }

    HWND hWnd = CreateWindowEx(
        WS_EX_TOPMOST,
        MY_CLASS_NAME,
        MY_WND_TITLE,
        MY_WND_STYLE,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        NULL, NULL,
        NULL, NULL);
    if (!hWnd) { return 2; }

    /* Define the Client Size */
    RECT rc;
    SetRect(&rc, 0, 0, 300, 150);
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOMOVE);
    ShowWindow(hWnd, SW_SHOW);

    /* Create Thread and Event */
    DWORD tid;
    CreateThread(NULL, 0, ThreadProc, hWnd, 0, &tid);
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    while (!exiting)
    {
        if (!paused)
        {
            hWndTarget = GetForegroundWindow();
            EnumChildWindows(hWndTarget, EnumChildProc, 0);
        }
        RedrawWindow((HWND)lpParam, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
        WaitForSingleObject(hEvent, 1000);
    }
    return 0;
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
        SetEvent(hEvent);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ZeroMemory(&ps, sizeof(ps));

            HDC hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            HFONT hFont, hFontOld;
            hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
            hFontOld = (HFONT)SelectObject(hdc, hFont);

            RECT rc;
            SetRect(&rc, 30, 20, 999, 999);
            LPTSTR text = malloc(255 * sizeof(TCHAR));
            _stprintf(text,
                _T("Status: %s \n\n")
                _T("Target HWND: 0x%p \n\n")
                _T("Double Click / ENTER to toggle state.\n")
                _T("ESC to quit."),
                paused ? _T("Paused") : _T("Running"),
                hWndTarget);
            DrawText(hdc, text, _tcslen(text), &rc, 0);
            free(text);

            SelectObject(hdc, hFontOld);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        exiting = true;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, w, l);
    }
    return 0;
}
