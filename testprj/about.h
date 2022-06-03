#pragma once
#include<Windows.h>
#include"globle.h"
#include"Error.h"
//Below are function prototypes and constants used in About
BOOL ShowAbout();
LRESULT CALLBACK aboutWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL regAboutWindow();