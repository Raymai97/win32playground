#include "DwmFrame.h"

DwmFrame::DwmFrame() :
	m_hwnd(NULL),
	m_hopeExtend(false),
	m_isExtended(false) {
}

DwmFrame::~DwmFrame() {
	this->DoExtend(false);
}

void DwmFrame::SetHwnd(HWND hwnd) {
	m_hwnd = hwnd;
}

HRESULT DwmFrame::DoExtend(bool hope) {
	HRESULT hr = DwmExtendFrame(m_hwnd, hope ? -1 : 0, 0, 0, 0);
	m_hopeExtend = hope;
	if (SUCCEEDED(hr)) {
		m_isExtended = hope;
	}
	return hr;
}

bool DwmFrame::IsExtended() const {
	return m_isExtended;
}

void DwmFrame::OnDwmCompositionChanged() {
	if (DwmIsEnabled()) {
		this->DoExtend(m_hopeExtend);
	}
	else {
		m_isExtended = false;
	}
}

