#include"about.h"

/*
* message processer for about window
*/
LRESULT CALLBACK aboutWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam) {
	LPCWSTR about = L"Created By:Öì½­ÔÆ Åíè×Ô¦  @2022.5";
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (uint) {
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &paintStruct);
		HFONT font = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, FIXED_PITCH | FF_MODERN, L"Î¢ÈíÑÅºÚ");
		SelectObject(hdc, font);
		TextOutW(hdc, 5, 5, about, wcslen(about));
		TextOutW(hdc, 5, 40, L"For more infomation:", wcslen(L"For more infomation:"));
		TextOutW(hdc, 5, 75, L"GitHub Repository or README", wcslen(L"GitHub Repository or README"));
		TextOutW(hdc, 5, 120, L"Special thanks to:", wcslen(L"Special thanks to:"));
		TextOutW(hdc, 5, 155, L"Windows, for cleaning up the mess", wcslen(L"Windows, for cleaning up the mess"));
		TextOutW(hdc, 5, 190, L"                 we left in memery", wcslen(L"                 we left in memory"));

		DeleteObject(font);

		EndPaint(hwnd, &paintStruct);
		break;
	}

	case WM_DESTROY:
		UnregisterClass(L"aboutwindow", hinstance);
	default:
		return DefWindowProc(hwnd, uint, wParam, lParam);
	}

}

/*
* start a about window
*/
BOOL ShowAbout() {
	HWND aboutHwnd;
	ZeroMemory(&aboutHwnd, sizeof(aboutHwnd));
	aboutHwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"aboutwindow",
		L"About",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,  //WS_OVERLAPPEDWINDOW&        
		// Size and position
		200, 200, 400, 300,
		NULL,
		NULL,
		hinstance,
		NULL
	);
	if (aboutHwnd == NULL) {
		ErrorCatching(TEXT("createWindowInstance() returned NULL pointer!"), NULL);
		UnregisterClass(L"aboutwindow", hinstance);
		return FALSE;
	}
	ShowWindow(aboutHwnd, 10);
	return TRUE;

}

/*
* register the class of about window
* return true if success
*/
BOOL regAboutWindow() {
	BOOL ret = FALSE;
	WNDCLASSEX aboutClass;
	ZeroMemory(&aboutClass, sizeof(aboutClass));
	HBRUSH brush;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	aboutClass.cbSize = sizeof(aboutClass);
	aboutClass.lpfnWndProc = aboutWindowProc;
	aboutClass.style = CS_HREDRAW | CS_VREDRAW;
	aboutClass.hbrBackground = brush;
	aboutClass.lpszClassName = L"aboutwindow";
	aboutClass.hInstance = hinstance;
	ret = (RegisterClassEx(&aboutClass) != 0);
	if (!ret) {
		ErrorCatching(TEXT("Window class registeration failure"), NULL);
		return FALSE;
	}
	return TRUE;
}


