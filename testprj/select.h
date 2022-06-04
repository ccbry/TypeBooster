#pragma once
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include<time.h>
#include"resource.h"
#include"Error.h"
#include"globle.h"
#include"IniIo.h"
#include"manage.h"
#include"inputing.h"
//Below are Function prototypes and constants used in article selection
BOOL randomArticle();
BOOL selectedArticle();
BOOL regSelectWindow();
LRESULT CALLBACK SelectWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);