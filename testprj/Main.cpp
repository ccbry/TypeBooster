#include"main.h"

#pragma comment(lib,"WINMM.LIB")

//Below are function prototypes and constants designed to globally functioning
VOID ErrorCatching(LPCWSTR errorMessage, HWND hParent);

//Below are function prototypes and constants used in main window 
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL RegWindowClass(HINSTANCE hInstacne);
BOOL createWindowInstance(HINSTANCE hinstance, int cmdShow);
int mainWindowGo(HINSTANCE hInstance, int nShowCmd);

LPCWSTR WINDOW_CLASS_NAME = L"MainWindowForTyper";

//Below are function prototypes and constants used in multimedia
VOID musicLoad();
VOID musicPlay();
VOID musicPause();
VOID musicResume();
VOID musicErrorCatching(MCIERROR err, HWND hwnd);

//Below are global variables
HWND hwnd;
HINSTANCE hinstance;
WCHAR returnPath[128] = { 0 };
//Below are Function prototypes and constants used in article selection
LPWSTR randomArticle();
BOOL selectedArticle();
BOOL regSelectWindow();
LRESULT CALLBACK SelectWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);

//Below are function prototypes and constants used in About
BOOL ShowAbout();
LRESULT CALLBACK aboutWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL regAboutWindow();

// Below are function prototypes and constants used in aritcle management
BOOL regManageWindow();
LRESULT CALLBACK ManageWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL articleManager();
LPWSTR* LoadFile();
VOID ShowItem(UINT row, WCHAR * Name, WCHAR * Path, HWND list);

// Below are function prototypes and constants used in "ini" file IO;
VOID WriteProfile(LPCWSTR section, LPCWSTR item, LPCWSTR info, LPCWSTR relectivePath);

/*
* entry function
*/
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdline,
	_In_ int nShowCmd
) {
	if (LogIn(hInstance) == 0) {
		ErrorCatching(L"Login Exception", hwnd);
		PostQuitMessage(0);
	}
	regAboutWindow();
	regManageWindow();
	regSelectWindow();
	mainWindowGo(hInstance, nShowCmd);

	return 0;
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
		//TODO
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
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,            // Window style

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
/*
* load music and ready to play 
*/
VOID musicLoad() {
	MCIERROR err;
	err = mciSendString(L"open ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}
/*
* start music
*/
VOID musicPlay() {
	MCIERROR err;
	err = mciSendString(L"play ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* pause music
*/
VOID musicPause() {
	MCIERROR err;
	err = mciSendString(L"pause ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* continue music
*/
VOID musicResume() {
	MCIERROR err;
	err = mciSendString(L"resume ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* catch MCI error and print.
*/
VOID musicErrorCatching(MCIERROR err, HWND hwnd) {
	LPWSTR errorText = { 0 };
	mciGetErrorString(err, errorText, 100);
	MessageBox(hwnd, errorText, L"MCI ERROR", MB_OK);
}


/*
* message processer for about window
*/
LRESULT CALLBACK aboutWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam) {
	LPCWSTR about =  L"Created By:朱江云 彭枳驭  @2022.5" ;
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (uint) {
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &paintStruct);
		HFONT font = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, FIXED_PITCH | FF_MODERN, L"微软雅黑");
		SelectObject(hdc, font);
		TextOutW(hdc, 5, 5, about, wcslen(about));
		TextOutW(hdc, 5, 40, L"For more infomation:", wcslen(L"For more infomation:"));
		TextOutW(hdc, 5, 75, L"GitHub Repository or README", wcslen(L"GitHub Repository or README"));
		TextOutW(hdc, 5, 120, L"Special thanks to:", wcslen(L"Special thanks to:"));
		TextOutW(hdc, 5, 155, L"Windows, for cleaning up the mess", wcslen(L"Windows, for cleaning up the mess"));
		TextOutW(hdc, 5, 190, L"                 we left in memery", wcslen(L"                 we left in memery"));
		
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
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,          
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
LPWSTR* LoadFile() {
	IFileOpenDialog* pFileOpen;
	LPWSTR fileInfo[2] = { NULL, NULL};

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
			0 + clientAera.right* 2 / 3, clientAera.bottom * 0.9, clientAera.right / 3, clientAera.bottom * 0.1,
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
		WCHAR inFileBuffer[64 * 128] = { 0 };
		GetPrivateProfileSectionNames(inFileBuffer, 64 * 128, L"..\\etc\\docList.ini");
		WCHAR inFile[64][128] = { 0 };
		UINT infileCount = 0;
		UINT charNum = 0;
		for (UINT i = 0; i < 64 * 128; ) {
			inFile[infileCount][charNum] = inFileBuffer[i];
			if (inFileBuffer[i] == '\0' && inFileBuffer[i + 1] == '\0' && inFileBuffer[i + 2] == '\0') {
				goto out;
			}
			if (inFileBuffer[i] == '\0') {
				infileCount++;
				charNum = 0;
				i++;
				continue;
			}
			charNum++;
			i++;
		}
	out:
		;

		for (UINT i = 0; i < NumOfFileInIni; i++) {
			WCHAR pathName[128] = { 0 };
			GetPrivateProfileString(inFile[i + 1], L"path", L"error", pathName, 128, L"..\\etc\\docList.ini");
			ShowItem(rowCount, inFile[i + 1], pathName, articleList);
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
				info = LoadFile();
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
				WCHAR inFileBuffer[64 * 128] = { 0 };
				GetPrivateProfileSectionNames(inFileBuffer, 64 * 128, L"..\\etc\\docList.ini");
				WCHAR inFile[64][128] = { 0 };
				UINT infileCount = 0;
				UINT charNum = 0;
				for (UINT i = 0; i < 64 * 128; ) {
					inFile[infileCount][charNum] = inFileBuffer[i];
					if (inFileBuffer[i] == '\0' && inFileBuffer[i + 1] == '\0' && inFileBuffer[i + 2] == '\0') {
						goto out1;
					}
					if (inFileBuffer[i] == '\0') {
						infileCount++;
						charNum = 0;
						i++;
						continue;
					}
					charNum++;
					i++;
				}
				out1:
				;
				//compare two arrays of string to find out which one is differ.
				for (UINT i = 1; i < 64; i++) {
					BOOL have = FALSE;
					for (UINT j = 0; j < 64; j++) {
						if (wcscmp(inFile[i], inList[j]) == 0) {
							have = TRUE;
						}
					}
					if (!have) {
						WriteProfile(inFile[i], NULL, NULL, L"..\\etc\\docList.ini");
					}
					
				}
				for (UINT i = 0; i < 64; i++) {
					BOOL have = FALSE;
					for (UINT j = 0; j < 64; j++) {
						
						if (wcscmp(inList[i], inFile[j]) == 0) {
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
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,          
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
	WCHAR exname[256] = {0};
	wcscpy_s(exname, 128, Name);
	WCHAR expath[256] = {0};
	wcscpy_s(expath, 128, Path);
	if (exname == expath) {
		return;
	}
	
	LV_ITEM item1 = { 0 };
	item1.mask |= LVIF_TEXT;
	item1.pszText = exname;
	ListView_InsertItem(list, &item1);
	ListView_SetItemText(list,0, 1, expath);
}

/*
* write ini file
*/
VOID WriteProfile(LPCWSTR section, LPCWSTR item, LPCWSTR info, LPCWSTR relectivePath) {
	WritePrivateProfileStringW(section, item, info, relectivePath);
}

/*
* return a LPWSTR to a random file path
*/
LPWSTR randomArticle() {
	UINT NumOfFileInIni = GetPrivateProfileInt(L"Total_Item", L"count", -1, L"..\\etc\\docList.ini");
	WCHAR inFileBuffer[64 * 128] = { 0 };
	GetPrivateProfileSectionNames(inFileBuffer, 64 * 128, L"..\\etc\\docList.ini");
	WCHAR inFile[64][128] = { 0 };
	UINT infileCount = 0;
	UINT charNum = 0;
	for (UINT i = 0; i < 64 * 128; ) {
		inFile[infileCount][charNum] = inFileBuffer[i];
		if (inFileBuffer[i] == '\0' && inFileBuffer[i + 1] == '\0' && inFileBuffer[i + 2] == '\0') {
			goto out;
		}
		if (inFileBuffer[i] == '\0') {
			infileCount++;
			charNum = 0;
			i++;
			continue;
		}
		charNum++;
		i++;
	}
out:
	;
	srand((UINT)time(NULL));
	UINT ret = 1 + rand() % NumOfFileInIni;
	return inFile[ret];
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

		HWND hDeleteButton = CreateWindow(WC_BUTTON, L"Select",
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
		WCHAR inFileBuffer[64 * 128] = { 0 };
		GetPrivateProfileSectionNames(inFileBuffer, 64 * 128, L"..\\etc\\docList.ini");
		WCHAR inFile[64][128] = { 0 };
		UINT infileCount = 0;
		UINT charNum = 0;
		for (UINT i = 0; i < 64 * 128; ) {
			inFile[infileCount][charNum] = inFileBuffer[i];
			if (inFileBuffer[i] == '\0' && inFileBuffer[i + 1] == '\0' && inFileBuffer[i + 2] == '\0') {
				goto out;
			}
			if (inFileBuffer[i] == '\0') {
				infileCount++;
				charNum = 0;
				i++;
				continue;
			}
			charNum++;
			i++;
		}
	out:
		;

		for (UINT i = 0; i < NumOfFileInIni; i++) {
			WCHAR pathName[128] = { 0 };
			GetPrivateProfileString(inFile[i + 1], L"path", L"error", pathName, 128, L"..\\etc\\docList.ini");
			ShowItem(rowCount, inFile[i + 1], pathName, articleList);
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
BOOL selectedArticle() {
	HWND selectHwnd;
	ZeroMemory(&selectHwnd, sizeof(selectHwnd));
	selectHwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"Selectwindow",
		L"Select",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
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

//TODO: Implement the menu bar
//TODO: test music cycle
//TODO: Learn com
//TODO: music path
//TODO: about https
//TODO: high dpi