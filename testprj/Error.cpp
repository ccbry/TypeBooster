#include"Error.h"

/*
* print error info
*/
VOID ErrorCatching(LPCWSTR errorMessage, HWND hParent) {
	DWORD errCode = GetLastError();
	MessageBox(
		hParent,
		errorMessage,
		TEXT("ERROR"),
		MB_OK
	);
}