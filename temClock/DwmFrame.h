#pragma once
#include <Windows.h>
#include "MyWinAPI/Dwm.h"

class DwmFrame {
	HWND m_hwnd;
	bool m_hopeExtend, m_isExtended;

public:
	// Note: Pass HWND that hasn't extend frame yet
	DwmFrame();
	~DwmFrame();

	void SetHwnd(HWND);

	bool IsExtended() const;
	HRESULT DoExtend(bool hope);

	void OnDwmCompositionChanged();
};
