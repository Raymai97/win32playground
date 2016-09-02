#include "DpiAware.h"

typedef BOOL (WINAPI *SetDpiAware)(void);
typedef HRESULT (WINAPI *SetDpiAwareness)(int /* PROCESS_DPI_AWARENESS enum */);

int dpi_x = 96, dpi_y = 96;

void InitDPIAware() {
    HDC screen_hdc = GetDC(NULL);
    // Try set DPI aware ...
    SetDpiAwareness SetProcessDpiAwareness = (SetDpiAwareness)GetProcAddress(
        LoadLibraryA("shcore.dll"), "SetProcessDpiAwareness"
    );
    if (SetProcessDpiAwareness) {
        SetProcessDpiAwareness(1); // PROCESS_SYSTEM_DPI_AWARE
    } else {
        SetDpiAware SetProcessDpiAware = (SetDpiAware)GetProcAddress(
            LoadLibraryA("user32.dll"), "SetProcessDPIAware"
        );
        if (SetProcessDpiAware) { SetProcessDpiAware(); }
    }
    // GetDeviceCaps always work, even in Win95
    dpi_x = GetDeviceCaps(screen_hdc, LOGPIXELSX);
    dpi_y = GetDeviceCaps(screen_hdc, LOGPIXELSY);
    ReleaseDC(NULL, screen_hdc);
}

int DPIAware_x(int x) {
    return x * dpi_x / 96;
}

int DPIAware_y(int y) {
    return y * dpi_y / 96;
}
