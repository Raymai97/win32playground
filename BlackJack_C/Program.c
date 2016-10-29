// Requires C++ or C99 to compile

#include <stdio.h>
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include "BJack.h"
#include "GenList.h"
#include "MyWin32.h"

#define CLASS_NAME		_T("BlackJack Win32")
#define WND_TITLE		_T("BlackJack!")
#define WND_STYLE		(WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX))

#define CARD_CX		100
#define CARD_CY		120
#define CARD_MARGIN	32
#define GAMEOVER	(BJack_State() != BJS_NOTHING)

COLORREF
	clr_wnd_bg = RGB(0,128,0),
	clr_wnd_text = RGB(255,255,255),
	clr_card_bg = RGB(0,192,0),
	clr_card_text = RGB(0,0,0),
	clr_point_text = RGB(0,224,0),
	clr_frame = RGB(0,0,0);
HFONT hFont;
RECT rc_btn_take_card;
RECT rc_btn_deal;
RECT rc_btn_reset;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnLButtonDown(HWND, BOOL, int, int, UINT);
void OnRButtonUp(HWND, int, int, UINT);
void OnKeyDown(HWND, UINT, BOOL, int, UINT);
void OnPaint(HWND);

// Draw card with text
void DrawCard(HDC, HBRUSH hbrFill, HBRUSH hbrFrame,
	int x, int y, // of the first card
	bool reverse, // to offset opposite direction
	size_t index, // decides the offset of x
	int card); // decides the text (eg. if 13, text will be 'K')
// Redraw window and check BJack state
void CheckBJS(HWND);
// Call this after SetAutoXYFactor()
void UpdateLayout(HWND);

// Helper -----
#define INSIDE_RECT(rc, x, y)	(x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom)

void FillFrameRect(HDC hdc, RECT const *prc, HBRUSH hbrFill, HBRUSH hbrFrame) {
	FillRect(hdc, prc, hbrFill);
	FrameRect(hdc, prc, hbrFrame);
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR lpCmdLine, int nCmdShow)
{
	SetDpiAware();
	SetAutoXYFactor(GetMonitorDpi(NULL) / 96.0);
	// Register class
	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = hInst;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = CLASS_NAME;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wcex)) { return 1; }
	// Create and show window
	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WND_TITLE,
		WND_STYLE,
		CW_USEDEFAULT, 0, 0, 0,
		NULL, NULL,
		hInst, NULL);
	if (!hWnd) { return 2; }
	// Init game
	BJack_Reset();
	UpdateLayout(hWnd);
	ShowWindow(hWnd, nCmdShow);
	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteObject(hFont);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_KEYDOWN: HANDLE_WM_KEYDOWN(hWnd, w, l, OnKeyDown); break;
	case WM_LBUTTONDOWN: HANDLE_WM_LBUTTONDOWN(hWnd, w, l, OnLButtonDown); break;
	case WM_RBUTTONUP: HANDLE_WM_RBUTTONUP(hWnd, w, l, OnRButtonUp); break;
	case WM_PAINT: HANDLE_WM_PAINT(hWnd, w, l, OnPaint); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, msg, w, l);
	}
	return 0;
}

void OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	if (INSIDE_RECT(rc_btn_take_card, x, y)) { BJack_TakeCard(); }
	else if (INSIDE_RECT(rc_btn_deal, x, y)) { BJack_Deal(); }
	else if (INSIDE_RECT(rc_btn_reset, x, y)) { BJack_Reset(); }
	else { return; }
	CheckBJS(hWnd);
}

void OnRButtonUp(HWND hWnd, int x, int y, UINT flags) {
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MFS_DISABLED, 0, _T("BlackJack by Raymai97"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MFS_DISABLED, 0, _T("--- Scale UI ---"));
	AppendMenu(hMenu, MF_BYCOMMAND, 1, _T("100%"));
	AppendMenu(hMenu, MF_BYCOMMAND, 2, _T("125%"));
	AppendMenu(hMenu, MF_BYCOMMAND, 3, _T("150%"));
	AppendMenu(hMenu, MF_BYCOMMAND, 4, _T("200%"));
	POINT pt;
	GetCursorPos(&pt);
	int cmd = TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, NULL);
	if (cmd == 1) { SetAutoXYFactor(1.0); }
	else if (cmd == 2) { SetAutoXYFactor(1.25); }
	else if (cmd == 3) { SetAutoXYFactor(1.5); }
	else if (cmd == 4) { SetAutoXYFactor(2.0); }
	else { return; }
	UpdateLayout(hWnd);
}

void OnKeyDown(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
	if (vk == 'C') { BJack_TakeCard(); }
	else if (vk == 'D') { BJack_Deal(); }
	else if (vk == 'R') { BJack_Reset(); }
	else {
		if (vk == VK_APPS) { OnRButtonUp(hWnd, 0, 0, 0); }
		return;
	}
	CheckBJS(hWnd);
}

void OnPaint(HWND hWnd) {
	HBRUSH hbrWndBg = CreateSolidBrush(clr_wnd_bg);
	HBRUSH hbrCardBg = CreateSolidBrush(clr_card_bg);
	HBRUSH hbrFrame = CreateSolidBrush(clr_frame);

	RECT rc;
	GetClientRect(hWnd, &rc);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HGDIOBJ old_hfo = SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	FillRect(hdc, &rc, hbrWndBg);

	int i, count, cards[5];
	count = BJack_GetPlayerCards(cards);
	for (i = 0; i < count; ++i) {
		DrawCard(hdc, hbrCardBg, hbrFrame, 100, 70, false, i, cards[i]);
	}
	if (GAMEOVER) {
		count = BJack_GetComCards(cards);
		for (i = 0; i < count; ++i) {
			DrawCard(hdc, hbrCardBg, hbrFrame, 380, 100, true, i, cards[i]);
		}
	}

	UINT fmt = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	SetTextColor(hdc, clr_card_text);
	FillFrameRect(hdc, &rc_btn_take_card, hbrCardBg, hbrFrame);
	DrawText(hdc, _T("Take &Card"), -1, &rc_btn_take_card, fmt);
	FillFrameRect(hdc, &rc_btn_deal, hbrCardBg, hbrFrame);
	DrawText(hdc, _T("&Deal!"), -1, &rc_btn_deal, fmt);
	FillFrameRect(hdc, &rc_btn_reset, hbrCardBg, hbrFrame);
	DrawText(hdc, _T("&Reset"), -1, &rc_btn_reset, fmt);

	TCHAR str[200];
	_stprintf(str, _T("Your point = %i"), BJack_GetPlayerPoint());
	if (GAMEOVER) {
        _stprintf(str, _T("%s\nCOM point = %i"), str, BJack_GetComPoint());
	}
	RECT rcText = rc;
	rcText.top += 3; rcText.right -= 6;
	SetTextColor(hdc, clr_point_text);
	DrawText(hdc, str, -1, &rcText, DT_RIGHT);

	if (GAMEOVER) {
		SetTextColor(hdc, clr_wnd_text);
		SetRectAuto(&rc, 56, 74, 50, 20);
		DrawText(hdc, _T("You"), -1, &rc, 0);
		SetRectAuto(&rc, 494, 194, 50, 20);
		DrawText(hdc, _T("COM"), -1, &rc, 0);
	}

	SelectObject(hdc, old_hfo);
	// don't delete hFont here
	DeleteObject(hbrWndBg);
	DeleteObject(hbrCardBg);
	DeleteObject(hbrFrame);
	EndPaint(hWnd, &ps);
}

void DrawCard(HDC hdc, HBRUSH hbrFill, HBRUSH hbrFrame,
			  int first_x, int y, bool reverse, size_t i, int card)
{
	int x = (CARD_MARGIN*i);
	if (reverse) { x *= -1; }
	x += first_x;
	RECT rc;
	SetRectAuto(&rc, x, y, CARD_CX, CARD_CY);
	FillFrameRect(hdc, &rc, hbrFill, hbrFrame);
	// Figure out card text
	char str[3];
	if (card > 1 && card < 11) {
		sprintf(str, "%i", card);
	}
	else {
		strcpy(str,
			card == 1  ? "A" :
			card == 11 ? "J" :
			card == 12 ? "Q" :
			card == 13 ? "K" : "?");
	}
	// Draw card text
	rc.top += 3; rc.bottom -= 3;
	rc.left += 6; rc.right -= 6;
	SetTextColor(hdc, clr_card_text);
	DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
	DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
}

void CheckBJS(HWND hWnd) {
	InvalidateRect(hWnd, NULL, TRUE);
	if (GAMEOVER) {
		BJS bjs = BJack_State();
		LPCTSTR szMsg =
			bjs == BJS_YOU_BUSTED ?
			_T("You're busted!") :
			bjs == BJS_COM_BUSTED ?
			_T("COM is busted!") :
			bjs == BJS_BOTH_BUSTED ?
			_T("You all are busted!") :
			bjs == BJS_YOU_EXACT21 ?
			_T("You've scored exactly 21!") :
			bjs == BJS_COM_EXACT21 ?
			_T("COM has scored exactly 21!") :
			bjs == BJS_BOTH_EXACT21 ?
			_T("You all have scored exactly 21!") :
			bjs == BJS_YOU_5CARD ?
			_T("You've taken 5 cards, yet not busted.") :
			bjs == BJS_COM_5CARD ?
			_T("COM has taken 5 cards, yet not busted.") :
			bjs == BJS_BOTH_5CARD ?
			_T("You all have taken 5 cards, yet not busted.") :
			bjs == BJS_YOU_SCORE_BETTER ?
			_T("Your point is higher!") :
			bjs == BJS_COM_SCORE_BETTER ?
			_T("COM's point is higher!") :
			bjs == BJS_SAME_SCORE ?
			_T("Same point!") : _T("What happened?");
		LPCTSTR szTitle = (
			bjs == BJS_YOU_BUSTED ||
			bjs == BJS_COM_EXACT21 ||
			bjs == BJS_COM_5CARD ||
			bjs == BJS_COM_SCORE_BETTER ) ?
			_T("You lose!") : (
			bjs == BJS_COM_BUSTED ||
			bjs == BJS_YOU_EXACT21 ||
			bjs == BJS_YOU_5CARD ||
			bjs == BJS_YOU_SCORE_BETTER ) ?
			_T("You win!") :
			_T("Draw game!");
		UINT uType = (
			bjs == BJS_YOU_BUSTED ||
			bjs == BJS_COM_EXACT21 ||
			bjs == BJS_COM_5CARD ||
			bjs == BJS_COM_SCORE_BETTER ) ?
			MB_ICONWARNING : MB_ICONINFORMATION;
		MessageBox(hWnd, szMsg, szTitle, uType);
		BJack_Reset();
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

void UpdateLayout(HWND hWnd) {
	SetWindowClientSize(hWnd, AutoX(580), AutoY(370));
	CenterWindow(hWnd);
	SetRectAuto(&rc_btn_take_card,	120, 266, 100, 40);
	SetRectAuto(&rc_btn_deal,		240, 266, 100, 40);
	SetRectAuto(&rc_btn_reset,		360, 266, 100, 40);
	if (hFont) { DeleteObject(hFont); }
	LOGFONT logFont = {0};
	_tcscpy(logFont.lfFaceName, _T("Tahoma"));
	logFont.lfHeight = AutoY(LOGICAL(14));
	logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	hFont = CreateFontIndirect(&logFont);
	InvalidateRect(hWnd, NULL, TRUE);
}
