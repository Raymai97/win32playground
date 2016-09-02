#include <stdio.h>
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <time.h>
#include "GenList.h"
#include "DpiAware.h"

typedef GENLIST         CardList;

#define CLASS_NAME		_T("BlackJack Win32")
#define WND_TITLE		_T("BlackJack!")
#define WND_STYLE		(WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX))

HWND hWnd;
HBRUSH hbr_wnd_bg;
HBRUSH hbr_card_bg;
HBRUSH hbr_black;
HFONT hFont;

RECT rc_btn_take_card;
RECT rc_btn_deal;
RECT rc_btn_reset;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnLButtonDown(HWND, BOOL, int, int, UINT);
void OnKeyDown(HWND, UINT, BOOL, int, UINT);
void OnPaint(HWND);
void DrawCardText(HDC, RECT, int card);

/* Helper ------------------- */

#define INSIDE_RECT(rc, x, y)	(x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom)

int rnd(int min, int max) {
	return rand() % (max - min + 1) + min;
}

int LogicalFromPoint(int point) {
	int dpi_y = DPIAware_y(96);
	return -MulDiv(point, dpi_y, 72);
}

void MySetRect(LPRECT rc, int x, int y, int w, int h) {
    rc->left = DPIAware_x(x);
    rc->top = DPIAware_y(y);
    rc->right = rc->left + DPIAware_x(w);
    rc->bottom = rc->top + DPIAware_y(h);
}

/* BlackJack ---------------- */

#define BJ_CARD_MIN		1  // "A"
#define BJ_CARD_MAX		13 // "K"

typedef enum BJ_MSG_TYPE {
	BJ_MSG_WIN,
	BJ_MSG_LOSE,
	BJ_MSG_OTHER
} BJ_MSG_TYPE;

void Msg(TCHAR*, BJ_MSG_TYPE);
void ResetGame();
void Deal();
void TakeCard();
void ComTurn();
bool GameWinOrLose();
int  GetRandomCard();
int  PointOf(CardList const);

/* BlackJack global var ----- */

CardList unused_cards, player_cards, com_cards;
bool show_msgbox, game_ended;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE z, LPTSTR zz, int zzz)
{
	srand(time(NULL));
	InitDPIAware();
	hbr_card_bg = CreateSolidBrush(0x0000C000);
	hbr_black = (HBRUSH)GetStockObject(BLACK_BRUSH);
	MySetRect(&rc_btn_take_card, 120, 266, 100, 40);
	MySetRect(&rc_btn_deal,		 240, 266, 100, 40);
	MySetRect(&rc_btn_reset,	 360, 266, 100, 40);

	GenList_Create(&unused_cards, sizeof(int), true);
	GenList_Create(&player_cards, sizeof(int), true);
	GenList_Create(&com_cards, sizeof(int), true);

	// Register class
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.hbrBackground = hbr_wnd_bg = CreateSolidBrush(0x008000);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = CLASS_NAME;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wcex)) { return 1; }
	// Create and show window
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WND_TITLE,
		WND_STYLE,
		CW_USEDEFAULT, 0,
		DPIAware_x(600), DPIAware_y(400),
		NULL, NULL,
		NULL, NULL);
	if (!hWnd) { return 2; }
	ShowWindow(hWnd, SW_SHOW);
	// Create font
	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	_tcscpy(logFont.lfFaceName, _T("Tahoma"));
	logFont.lfHeight = LogicalFromPoint(14);
	logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	hFont = CreateFontIndirect(&logFont);
	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		HANDLE_WM_KEYDOWN(hWnd, w, l, OnKeyDown);
		break;
	case WM_LBUTTONDOWN:
		HANDLE_WM_LBUTTONDOWN(hWnd, w, l, OnLButtonDown);
		break;
	case WM_PAINT:
		HANDLE_WM_PAINT(hWnd, w, l, OnPaint);
		break;
	case WM_SHOWWINDOW: // frmMain_Load
		ResetGame();
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, w, l);
	}
	return 0;
}

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	if (INSIDE_RECT(rc_btn_take_card, x, y)) { TakeCard(); }
	if (INSIDE_RECT(rc_btn_deal, x, y)) { Deal(); }
	if (INSIDE_RECT(rc_btn_reset, x, y)) { ResetGame(); }
}

void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
	if (vk == 'C') { TakeCard(); }
	else if (vk == 'D') { Deal(); }
	else if (vk == 'R') { ResetGame(); }
}

void OnPaint(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);

	size_t player_cards_count, com_cards_count;
	GenList_GetInfo(player_cards, &player_cards_count, NULL, NULL);
	GenList_GetInfo(com_cards, &com_cards_count, NULL, NULL);

	int card;
	RECT rc;
	for (size_t i = 0; i < player_cards_count; ++i) {
		MySetRect(&rc, 100 + (32*i), 70, 100, 120);
		FillRect(hdc, &rc, hbr_card_bg);
		FrameRect(hdc, &rc, hbr_black);
		GenList_GetAt(player_cards, i, (void**)&card);
		DrawCardText(hdc, rc, card);
	}
	if (game_ended) {
		for (size_t i = 0; i < com_cards_count; ++i) {
			MySetRect(&rc, 380 - (32*i), 100, 100, 120);
			FillRect(hdc, &rc, hbr_card_bg);
			FrameRect(hdc, &rc, hbr_black);
			GenList_GetAt(com_cards, i, (void**)&card);
			DrawCardText(hdc, rc, card);
		}
	}

	UINT fmt = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	FillRect(hdc, &rc_btn_take_card, hbr_card_bg);
	FrameRect(hdc, &rc_btn_take_card, hbr_black);
	DrawText(hdc, _T("Take &Card"), -1, &rc_btn_take_card, fmt);

	FillRect(hdc, &rc_btn_deal, hbr_card_bg);
	FrameRect(hdc, &rc_btn_deal, hbr_black);
	DrawText(hdc, _T("&Deal"), -1, &rc_btn_deal, fmt);

	FillRect(hdc, &rc_btn_reset, hbr_card_bg);
	FrameRect(hdc, &rc_btn_reset, hbr_black);
	DrawText(hdc, _T("&Reset"), -1, &rc_btn_reset, fmt);

	TCHAR str[200] = {0};
	_stprintf(str, _T("Your point = %i"), PointOf(player_cards));
	if (game_ended) {
        _stprintf(str, _T("%s\nCOM point = %i"), str, PointOf(com_cards));
	}
	GetClientRect(hWnd, &rc);
	rc.top += 3; rc.right -= 6;
	SetTextColor(hdc, 0x00E000);
	DrawText(hdc, str, -1, &rc, DT_RIGHT);

	if (game_ended) {
		SetTextColor(hdc, 0xFFFFFF);
		MySetRect(&rc, 56, 74, 50, 20);
		DrawText(hdc, _T("You"), -1, &rc, 0);
		MySetRect(&rc, 494, 190, 50, 20);
		DrawText(hdc, _T("COM"), -1, &rc, 0);
	}

	EndPaint(hWnd, &ps);
}

void DrawCardText(HDC hdc, RECT rc, int card) {
	TCHAR str[10] = {0};
	if (card == 1) { _tcscpy(str, _T("A")); }
	else if (card == 11) { _tcscpy(str, _T("J")); }
	else if (card == 12) { _tcscpy(str, _T("Q")); }
	else if (card == 13) { _tcscpy(str, _T("K")); }
	else { _stprintf(str, _T("%i"), card); }
	rc.top += 3; rc.bottom -= 3;
	rc.left += 6; rc.right -= 6;
	DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
	DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
}

/* BlackJack definition ----- */

void Msg(TCHAR *msg, BJ_MSG_TYPE type) {
	if (!show_msgbox) { return; }

	game_ended = true;
	InvalidateRect(hWnd, NULL, TRUE);

	TCHAR buf[200];
	ZeroMemory(&buf, sizeof(buf));
	_tcscpy(buf, msg);
	if (type == BJ_MSG_WIN)	{
		_tcscat(buf, _T("\nCongratulations!"));
	} else if (type == BJ_MSG_LOSE) {
		_tcscat(buf, _T("\nGame Over!"));
	}
	MessageBox(hWnd, buf,
		type == BJ_MSG_WIN ? _T("You win!") :
		type == BJ_MSG_LOSE ? _T("You lose!") :
		_T("BlackJack"),
		type == BJ_MSG_LOSE ? MB_ICONWARNING :
		MB_ICONINFORMATION);
	game_ended = false;
}

void ResetGame() {
	// Prepare unused cards
	GenList_Clear(unused_cards);
	for (int i = BJ_CARD_MIN; i <= BJ_CARD_MAX; ++i) {
		for (int j = 0; j < 4; ++j) {
			GenList_Add(unused_cards, &i);
		}
	}
	// Player and COM, both take 2 cards
	show_msgbox = false;
	do {
        int card;
        GenList_Clear(player_cards);
        card = GetRandomCard();
        GenList_Add(player_cards, &card);
        card = GetRandomCard();
	    GenList_Add(player_cards, &card);

        GenList_Clear(com_cards);
        card = GetRandomCard();
        GenList_Add(com_cards, &card);
        card = GetRandomCard();
	    GenList_Add(com_cards, &card);
	} /*loop*/ while ( GameWinOrLose() );
	show_msgbox = true;
	InvalidateRect(hWnd, NULL, TRUE);
}

void TakeCard() {
    int card = GetRandomCard();
	GenList_Add(player_cards, &card);
	ComTurn();
	InvalidateRect(hWnd, NULL, TRUE);
	if (GameWinOrLose()) { ResetGame(); }
}

void Deal() {
	ComTurn();
	int player_point = PointOf(player_cards),
		com_point = PointOf(com_cards);

	if (GameWinOrLose()) {

	} else if (player_point > com_point) {
		Msg(_T("Your point is higher than COM!"), BJ_MSG_WIN);

	} else if (com_point > player_point) {
		Msg(_T("COM's point is higher than yours!"), BJ_MSG_LOSE);

	} else {
		Msg(_T("Both players have same point! Draw game."), BJ_MSG_OTHER);
	}
	ResetGame();
}

void ComTurn() {
	double rate = (20 - PointOf(com_cards)) / 20.0;
    if ( rate + (rnd(1,5) * 0.1) > 0.5) {
    	int card = GetRandomCard();
    	GenList_Add(com_cards, &card);
    }
}

bool GameWinOrLose() {
	int player_point = PointOf(player_cards),
		com_point = PointOf(com_cards);

    size_t player_cards_count, com_cards_count;
    GenList_GetInfo(player_cards, &player_cards_count, NULL, NULL);
    GenList_GetInfo(com_cards, &com_cards_count, NULL, NULL);

    if (player_point > 21 && com_point > 21) {
		Msg(_T("What a rare situation! Both players are busted! Draw game."), BJ_MSG_OTHER);

	}else if (player_point > 21) {
		Msg(_T("Your point is over 21!"), BJ_MSG_LOSE);

	} else if (player_point == 21) {
		Msg(_T("Your point is exactly 21!"), BJ_MSG_WIN);

	} else if (player_cards_count >= 5) {
		Msg(_T("You've taken 5 cards, yet not over 21 yet."), BJ_MSG_WIN);

	} else if (com_point > 21) {
		Msg(_T("COM's point is over 21!"), BJ_MSG_WIN);

	} else if (com_point == 21) {
		Msg(_T("COM's point is exactly 21!"), BJ_MSG_LOSE);

	} else if (com_cards_count >= 5) {
		Msg(_T("COM has taken 5 cards, yet not over 21 yet."), BJ_MSG_LOSE);

	} else { return false; }
	return true;
}

int GetRandomCard() {
    int card;
    size_t unused_cards_count;
    GenList_GetInfo(unused_cards, &unused_cards_count, NULL, NULL);
	size_t i = rnd(0, unused_cards_count - 1);
	GenList_GetAt(unused_cards, i, (void**)&card);
	GenList_RemoveAt(unused_cards, i);
	return card;
}

int PointOf(CardList const cards) {
    int point = 0;
    size_t card_count, count_of_A = 0;
    GenList_GetInfo(cards, &card_count, NULL, NULL);
	for (size_t i = 0; i < card_count; ++i) {
        int card;
        GenList_GetAt(cards, i, (void**)&card);
		if (card == 1) {
			++count_of_A;
			point += 11;
		} else if (card > 10) {
			point += 10;
		} else {
			point += card;
		}
	}
	while (point > 21 && count_of_A > 0) {
		--count_of_A;
		point -= 10;
	}
	return point;
}
