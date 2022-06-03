#include"manage.h"

/*
* register the class of about window
* return true if success
*/
BOOL regManageWindow() {
	BOOL ret = FALSE;
	WNDCLASSEX ManageClass;
	ZeroMemory(&ManageClass, sizeof(ManageClass));
	HBRUSH brush;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	ManageClass.cbSize = sizeof(ManageClass);
	ManageClass.lpfnWndProc = ManageWindowProc;
	ManageClass.style = CS_HREDRAW | CS_VREDRAW;
	ManageClass.hbrBackground = brush;
	ManageClass.lpszClassName = L"Managewindow";
	ManageClass.hInstance = hinstance;
	ret = (RegisterClassEx(&ManageClass) != 0);
	if (!ret) {
		ErrorCatching(TEXT("Window class registeration failure"), NULL);
		return FALSE;
	}
	return TRUE;
}

/*
* open a new file chooser dialog
* return a LPWSTR array :first carring path and second carrying name
*/
LPWSTR* SelectFileToAdd() {
	IFileOpenDialog* pFileOpen;
	LPWSTR fileInfo[2] = { NULL, NULL };

	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &fileInfo[0]);
				hr = pItem->GetDisplayName(SIGDN_PARENTRELATIVEEDITING, &fileInfo[1]);
				return fileInfo;
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	return 0;
}

/*
* Handle messages from manage window
*/
LRESULT CALLBACK ManageWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam) {
	RECT clientAera;
	GetClientRect(hwnd, &clientAera);
	static HWND articleList = { 0 };
	static UINT rowCount = 0;
	int SelectedRow = 0;
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
		HWND hAddButton = CreateWindow(WC_BUTTON, L"Add",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			clientAera.left, clientAera.bottom * 0.9, clientAera.right / 3, clientAera.bottom * 0.1,
			hwnd, (HMENU)IDC_ADDBUTTON, NULL, NULL);
		HWND hDeleteButton = CreateWindow(WC_BUTTON, L"Delete",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0 + clientAera.right / 3, clientAera.bottom * 0.9, clientAera.right / 3, clientAera.bottom * 0.1,
			hwnd, (HMENU)IDC_DELETEBUTTON, NULL, NULL);
		HWND hSaveButton = CreateWindow(WC_BUTTON, L"Save",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0 + clientAera.right * 2 / 3, clientAera.bottom * 0.9, clientAera.right / 3, clientAera.bottom * 0.1,
			hwnd, (HMENU)IDC_SAVEBUTTON, NULL, NULL);

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
			case IDC_ADDBUTTON: {
				LPWSTR* info = { 0 };
				info = SelectFileToAdd();
				if (info == 0) {
					goto jump;
				}
				WCHAR* path = info[0];
				WCHAR* name = info[1];
				ShowItem(rowCount, name, path, articleList);
				rowCount++;
				break;
			}
			case IDC_DELETEBUTTON: {
				SelectedRow = ListView_GetSelectionMark(articleList);
				ListView_DeleteItem(articleList, SelectedRow);
				break;
			}
			case IDC_SAVEBUTTON: {
				WCHAR count[20] = { 0 };
				wsprintf(count, L"%d", ListView_GetItemCount(articleList));
				WriteProfile(L"Total_Item", L"count", count, L"..\\etc\\docList.ini");
				WCHAR inList[64][128] = { 0 };
				for (UINT i = 0; i < rowCount; i++) {

					ListView_GetItemText(articleList, i, 0, inList[i], 128);
				}
				Infile namearray = { 0 };
				readProfile(&namearray);
				//compare two arrays of string to find out which one is differ.
				for (UINT i = 1; i < 64; i++) {
					BOOL have = FALSE;
					for (UINT j = 0; j < 64; j++) {
						if (wcscmp(namearray.nameArray[i], inList[j]) == 0) {
							have = TRUE;
						}
					}
					if (!have) {
						WriteProfile(namearray.nameArray[i], NULL, NULL, L"..\\etc\\docList.ini");
					}

				}
				for (UINT i = 0; i < 64; i++) {
					BOOL have = FALSE;
					for (UINT j = 0; j < 64; j++) {

						if (wcscmp(inList[i], namearray.nameArray[j]) == 0) {
							have = TRUE;
						}

					}
					if (!have) {
						WCHAR recieveName[128] = { 0 };
						WCHAR recievePath[128] = { 0 };
						ListView_GetItemText(articleList, i, 0, recieveName, 128);
						ListView_GetItemText(articleList, i, 1, recievePath, 128);
						if (recieveName != L"") {
							WriteProfile(recieveName, L"path", recievePath, L"..\\etc\\docList.ini");
						}

					}
				}
			}
			}
		}
		break;
	}
			   jump:
	default:
		return DefWindowProc(hwnd, uint, wParam, lParam);
	}

}

/*
* Call the article manager
* return true if success.
*/
BOOL articleManager() {
	HWND manageHwnd;
	ZeroMemory(&manageHwnd, sizeof(manageHwnd));
	manageHwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"managewindow",
		L"Manager",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		// Size and position
		200, 200, 400, 500,
		NULL,
		NULL,
		hinstance,
		NULL
	);
	if (manageHwnd == NULL) {
		ErrorCatching(TEXT("createWindowInstance() returned NULL pointer!"), NULL);
		UnregisterClass(L"aboutwindow", hinstance);
		return FALSE;
	}
	ShowWindow(manageHwnd, 10);
	return TRUE;

}

/*
* recieve the item info and display it on the article list
*/
VOID ShowItem(UINT row, WCHAR* Name, WCHAR* Path, HWND list) {
	WCHAR exname[256] = { 0 };
	wcscpy_s(exname, 128, Name);
	WCHAR expath[256] = { 0 };
	wcscpy_s(expath, 128, Path);
	if (exname == expath) {
		return;
	}

	LV_ITEM item1 = { 0 };
	item1.mask |= LVIF_TEXT;
	item1.pszText = exname;
	ListView_InsertItem(list, &item1);
	ListView_SetItemText(list, 0, 1, expath);
}