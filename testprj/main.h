#pragma once
#include<Windows.h>
#include<windowsx.h>

#include<CommCtrl.h>
#include<wchar.h>
#include<stdlib.h>



#include"resource.h"
#include"IniIo.h"
#include"select.h"
#include"Error.h"
#include"login.h"
#include"music.h"
#include"about.h"
//Below are function prototypes and constants used in main window 
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL RegWindowClass(HINSTANCE hInstacne);
BOOL createWindowInstance(HINSTANCE hinstance, int cmdShow);
int mainWindowGo(HINSTANCE hInstance, int nShowCmd);

LPCWSTR WINDOW_CLASS_NAME = L"MainWindowForTyper";