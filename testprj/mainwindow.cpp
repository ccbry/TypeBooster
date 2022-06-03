#include"mainwindow.h"
LPCWSTR WINDOW_CLASS_NAME = L"MainWindowForTyper";
void MainWindow_Cls_OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
}

void MainWindow_Cls_OnCommand(HWND hWnd, UINT id, HWND hwndctrl, UINT code) {

	switch (id)
	{
	case ID_FILE_EXIT:
		MainWindow_Cls_OnDestroy(hWnd);
		break;
	case ID_MUSIC_START:
		musicPlay();
		break;
	case ID_MUSIC_PAUSE:
		musicPause();
		break;
	case ID_MUSIC_RESUME:
		musicResume();
		break;
	case ID_HELP_ABOUT:
		ShowAbout();
		break;
	case ID_FILE_MANAGEARTICLES:
		articleManager();
		break;
	case ID_PRACTICE_RANDOMARTICLE:
		randomArticle();
		break;
	case ID_PRACTICE_SELECTED:
		selectedArticle();
		break;

	}
}
/*
* function: window message processing function
* paras: message sent from window
*/
LRESULT CALLBACK mainWindowProc(
	HWND hwnd,
	UINT uint,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uint) {
	case WM_DESTROY:
		return HANDLE_WM_DESTROY(hwnd, wParam, lParam, MainWindow_Cls_OnDestroy);
	case WM_COMMAND:
		return HANDLE_WM_COMMAND(hwnd, wParam, lParam, MainWindow_Cls_OnCommand);
	default: return DefWindowProc(hwnd, uint, wParam, lParam);
	}
}

/*
* function: regester a windows class
*/
BOOL RegWindowClass(HINSTANCE hInstacne) {

	BOOL ret = FALSE;
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	HBRUSH brush;
	brush = CreateSolidBrush(RGB(255, 255, 255));

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = mainWindowProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = brush;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	wndClass.hInstance = hInstacne;
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
	ret = (RegisterClassEx(&wndClass) != 0);
	if (!ret) {
		ErrorCatching(TEXT("Window class registeration failure"), NULL);
	}
	return ret;
}

/*
* create a window instancd
* return true if success
*/
BOOL createWindowInstance(HINSTANCE hInstance, int cmdShow) {
	//HWND hwnd;
	ZeroMemory(&hwnd, sizeof(hwnd));
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,                // Optional window styles.
		WINDOW_CLASS_NAME,              // Window class
		L"TypeBooster",                 // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);
	if (hwnd == NULL) {
		ErrorCatching(TEXT("createWindowInstance() returned NULL pointer!"), NULL);
		return FALSE;
	}
	ShowWindow(hwnd, cmdShow);
	return TRUE;
}

/*
* start the main window
*/
int mainWindowGo(HINSTANCE hInstance, int nShowCmd) {
	MSG msg;
	BOOL fGotMessage = FALSE;

	if (!RegWindowClass(hInstance) || !createWindowInstance(hInstance, nShowCmd)) {
		return FALSE;
	}
	musicLoad();
	//message cycle
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
