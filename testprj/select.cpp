#include"select.h"

BOOL selectedArticle() {
	HWND selectHwnd;
	ZeroMemory(&selectHwnd, sizeof(selectHwnd));
	selectHwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"Selectwindow",
		L"Select",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		// Size and position
		200, 200, 400, 500,
		NULL,
		NULL,
		hinstance,
		NULL
	);
	if (selectHwnd == NULL) {
		ErrorCatching(TEXT("createWindowInstance() returned NULL pointer!"), NULL);
		UnregisterClass(L"aboutwindow", hinstance);
		return FALSE;
	}
	ShowWindow(selectHwnd, 10);
	return TRUE;

}

LRESULT CALLBACK SelectWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam) {
	RECT clientAera;
	GetClientRect(hwnd, &clientAera);
	static HWND articleList = { 0 };
	static UINT rowCount = 0;
	switch (uint) {
	case WM_CREATE:
	{
	PaintManageWindow:
		articleList = CreateWindow(WC_LISTVIEW,
			L"",
			WS_CHILD | WS_BORDER | LVS_SHOWSELALWAYS | LVS_REPORT | WS_VISIBLE | LVS_SINGLESEL,
			clientAera.left, 1, clientAera.right, clientAera.bottom * 0.9,
			hwnd,
			(HMENU)IDC_MANAGELIST,
			NULL, NULL);
		ListView_SetExtendedListViewStyle(articleList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		HWND hSelectButton = CreateWindow(WC_BUTTON, L"Select",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0 + clientAera.right / 3, clientAera.bottom * 0.9, clientAera.right / 3, clientAera.bottom * 0.1,
			hwnd, (HMENU)IDC_SELECTBUTTON, NULL, NULL);

		WCHAR name[10] = TEXT("name");
		WCHAR path[10] = TEXT("path");

		//list head
		LVCOLUMN columOfName = { 0 };
		columOfName.mask |= LVCF_WIDTH | LVCF_TEXT;
		columOfName.cx = 60;
		columOfName.pszText = name;
		SendMessageW(articleList, LVM_INSERTCOLUMN, 0, (LPARAM)&columOfName); //sending message or using LISTVIEW_INSERTCOLUM

		LVCOLUMN columOfPath = { 0 };
		columOfPath.mask |= LVCF_WIDTH | LVCF_TEXT;
		columOfPath.cx = 200;
		columOfPath.pszText = path;
		SendMessageW(articleList, LVM_INSERTCOLUMN, 1, (LPARAM)&columOfPath);

		//read ini and display
		UINT NumOfFileInIni = GetPrivateProfileInt(L"Total_Item", L"count", -1, L"..\\etc\\docList.ini");
		Infile namearray = { 0 };
		readProfile(&namearray);

		for (UINT i = 0; i < NumOfFileInIni; i++) {
			WCHAR pathName[128] = { 0 };
			GetPrivateProfileString(namearray.nameArray[i + 1], L"path", L"error", pathName, 128, L"..\\etc\\docList.ini");
			ShowItem(rowCount, namearray.nameArray[i + 1], pathName, articleList);
			rowCount++;
		}
		break;


	}
	case WM_COMMAND: {
		UINT controlID = LOWORD(wParam);
		UINT controlCode = HIWORD(wParam);
		if (controlCode == BN_CLICKED) {
			switch (controlID)
			{
			case IDC_SELECTBUTTON: {
				UINT selectedRow = 0;
				selectedRow = ListView_GetSelectionMark(articleList);
				ListView_GetItemText(articleList, selectedRow, 1, returnPath, 128);
				if (*returnPath != NULL) {
					ShowWindow(hwnd, SW_HIDE);
				}
				else {
					MessageBox(hwnd, L"Please select a text file.", L"Tip", NULL);
				}

				break;
			}
			}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uint, wParam, lParam);
	}
}

BOOL regSelectWindow() {
	BOOL ret = FALSE;
	WNDCLASSEX SelectClass;
	ZeroMemory(&SelectClass, sizeof(SelectClass));
	HBRUSH brush;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectClass.cbSize = sizeof(SelectClass);
	SelectClass.lpfnWndProc = SelectWindowProc;
	SelectClass.style = CS_HREDRAW | CS_VREDRAW;
	SelectClass.hbrBackground = brush;
	SelectClass.lpszClassName = L"Selectwindow";
	SelectClass.hInstance = hinstance;
	ret = (RegisterClassEx(&SelectClass) != 0);
	if (!ret) {
		ErrorCatching(TEXT("Window class registeration failure"), NULL);
		return FALSE;
	}
	return TRUE;
}
/*
* return a LPWSTR to a random file path
*/
BOOL randomArticle() {
	UINT NumOfFileInIni = GetPrivateProfileInt(L"Total_Item", L"count", -1, L"..\\etc\\docList.ini");
	if (NumOfFileInIni == 0) {
		MessageBox(hwnd, L"No datium in article list!", L"error", NULL);
		return FALSE;
	}

	Infile namearray = { 0 };
	readProfile(&namearray);
	srand((UINT)time(NULL));
	UINT ret = 1 + rand() % NumOfFileInIni;
	GetPrivateProfileString(namearray.nameArray[ret], L"path", L"error", returnPath, 128, L"..\\etc\\docList.ini");
	return TRUE;
}
