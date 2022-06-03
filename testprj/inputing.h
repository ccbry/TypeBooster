#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include<windowsx.h>
#include<richedit.h>
#include<commdlg.h>
#include<Commctrl.h>
// C 运行时头文件
#include <stdlib.h>
#include"globle.h"
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include<strsafe.h>
//创建显示文本的子窗口
VOID CreateDisplay(HINSTANCE hInstance, HWND hParent, UINT YAxis);
//创建输入框
VOID CreateInput(HINSTANCE hInstance, HWND hParent, UINT YAxis);
//创建字体
HANDLE CreateDefaultFont();
//导入文本 文本编码为GBK 2312
VOID LoadFile(LPWSTR file);
//导入文本
DWORD CALLBACK DisplayStreamInCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

//处理输入
VOID InPuting(UINT YAxis);