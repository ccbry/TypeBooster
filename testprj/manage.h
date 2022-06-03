#pragma once
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include<ShlObj.h>
#include"globle.h"
#include"Error.h"
#include"resource.h"
#include"IniIo.h"


// Below are function prototypes and constants used in aritcle management
BOOL regManageWindow();
LRESULT CALLBACK ManageWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL articleManager();
LPWSTR* LoadFile();
VOID ShowItem(UINT row, WCHAR* Name, WCHAR* Path, HWND list);
