#pragma once
#include<Windows.h>
#include<windowsx.h>

#include<CommCtrl.h>
#include<wchar.h>
#include<stdlib.h>

#include"resource.h"
#include"login.h"
#include"globle.h"
#include"Error.h"
#include"about.h"
#include"manage.h"
#include"select.h"


//Below are function prototypes and constants used in main window 
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL RegWindowClass(HINSTANCE hInstacne);
BOOL createWindowInstance(HINSTANCE hinstance, int cmdShow);
int mainWindowGo(HINSTANCE hInstance, int nShowCmd);

