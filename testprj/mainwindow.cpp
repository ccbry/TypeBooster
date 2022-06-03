#include"mainwindow.h"
LPCWSTR WINDOW_CLASS_NAME = L"MainWindowForTyper";
HANDLE CreateFont()
{
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));

	// 设置字体为Courier New
	lf.lfHeight = 20;
	lf.lfWidth = 8;
	lf.lfWeight = 500;
	lf.lfOutPrecision = 3;
	lf.lfClipPrecision = 2;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 1;
	StringCchCopyW((STRSAFE_LPWSTR)&lf.lfFaceName, 32, L"Courier New");

	return CreateFontIndirectW(&lf);
}
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
	static HWND hwnd2;
	static WCHAR history[300];
	static RECT clientAera;
	GetClientRect(hwnd, &clientAera);
	switch (uint) {
	case WM_CREATE: {
		CreateDisplay(hinstance, hwnd, 80000 / clientAera.right > 100 ? 200 : 100);
		CreateInput(hinstance, hwnd, 80000 / clientAera.right > 100 ? 200 : 100);
		score his[5] = { 0 };
		readHistory(his, ID);
		hwnd2 = CreateWindow(
			L"STATIC", L"abc",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			0, 0, clientAera.right, 80000 / clientAera.right > 100 ? 200 : 100,
			hwnd,
			(HMENU)114514, hinstance, NULL);
		StringCchPrintf(history, 300,
			L"历史1：字数：%d，错误数：%d，准确度：%f%，按键次数：%d，退格次数：%d，速度：%d，时间：%d\n"
			L"历史2：字数：%d，错误数：%d，准确度：%f%，按键次数：%d，退格次数：%d，速度：%d，时间：%d\n"
			L"历史3：字数：%d，错误数：%d，准确度：%f%，按键次数：%d，退格次数：%d，速度：%d，时间：%d\n"
			L"历史4：字数：%d，错误数：%d，准确度：%f%，按键次数：%d，退格次数：%d，速度：%d，时间：%d\n"
			L"历史5：字数：%d，错误数：%d，准确度：%f%，按键次数：%d，退格次数：%d，速度：%d，时间：%d\n",
			his[0].word_count, his[0].error_count, his[0].accuracy, his[0].key_count, his[0].back, his[0].speed, his[0].time,
			his[1].word_count, his[1].error_count, his[1].accuracy, his[1].key_count, his[1].back, his[1].speed, his[1].time,
			his[2].word_count, his[2].error_count, his[2].accuracy, his[2].key_count, his[2].back, his[2].speed, his[2].time,
			his[3].word_count, his[3].error_count, his[3].accuracy, his[3].key_count, his[3].back, his[3].speed, his[3].time,
			his[4].word_count, his[4].error_count, his[4].accuracy, his[4].key_count, his[4].back, his[4].speed, his[4].time);
		SetWindowText(hwnd2, history);
		SetFocus(hInPut);
	}break;

	case WM_SIZE: {
		DestroyWindow(hwnd2);
		hwnd2 = CreateWindow(
			L"STATIC", history,
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			0, 0, clientAera.right, 80000 / clientAera.right > 100 ? 200 : 100,
			hwnd,
			(HMENU)114514, hinstance, NULL);
		SendMessageW(hwnd2, WM_SETFONT, (WPARAM)CreateFont(), TRUE);
	}break;
	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;
		switch (nmhdr->code)
		{
		case EN_MSGFILTER:
			MSGFILTER* msgfilter = (MSGFILTER*)lParam;
			switch (msgfilter->msg)
			{
			case WM_KEYDOWN:
				DisplayScore.key_count++;
			case WM_CHAR:
				switch (msgfilter->wParam)
				{
				case VK_BACK:
					DisplayScore.back++;
				default:
					break;
				}
			}
		}
		return FALSE;
	}
	case WM_DESTROY:
		return HANDLE_WM_DESTROY(hwnd, wParam, lParam, MainWindow_Cls_OnDestroy);
	case WM_COMMAND: {
		RECT clientAera;
		GetClientRect(hwnd, &clientAera);
		if (HIWORD(wParam) == EN_CHANGE)
			InPuting(80000 / clientAera.right > 100 ? 200 : 100);
		return HANDLE_WM_COMMAND(hwnd, wParam, lParam, MainWindow_Cls_OnCommand);
	}
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
