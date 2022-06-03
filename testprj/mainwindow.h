#pragma once
#include<Windows.h>
#include<richedit.h>
#include"resource.h"
#include"music.h"
#include"IniIo.h"
#include"select.h"
#include"Error.h"
#include"about.h"
#include"history.h"
#include"globle.h"
#include"inputing.h"

void MainWindow_Cls_OnDestroy(HWND hWnd);
void MainWindow_Cls_OnCommand(HWND hWnd, UINT id, HWND hwndctrl, UINT code);
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uint, WPARAM wParam, LPARAM lParam);
BOOL RegWindowClass(HINSTANCE hInstacne);
BOOL createWindowInstance(HINSTANCE hInstance, int cmdShow);
int mainWindowGo(HINSTANCE hInstance, int nShowCmd);