#include"main.h"

/*
* entry function
*/
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdline,
	_In_ int nShowCmd
) {
	ID = LogIn(hInstance);
	if (ID == 0) {
		ErrorCatching(L"Login Exception", hwnd);
		PostQuitMessage(0);
	}
	if (!regAboutWindow()) {
		ErrorCatching(L"Can not register about window", hwnd);
	}
	if (!regManageWindow()) {
		ErrorCatching(L"Can not register manage window", hwnd);
	}
	if(!regSelectWindow() ){
		ErrorCatching(L"Can not register select window", hwnd);
	}
	mainWindowGo(hInstance, nShowCmd);

	return 0;
}

