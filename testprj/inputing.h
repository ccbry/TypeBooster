#pragma once
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <windows.h>
#include<windowsx.h>
#include<richedit.h>
#include<commdlg.h>
#include<Commctrl.h>
#include<strsafe.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include"globle.h"
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include"history.h"

//������ʾ�ı����Ӵ���
VOID CreateDisplay(HINSTANCE hInstance, HWND hParent, UINT YAxis);
//���������
VOID CreateInput(HINSTANCE hInstance, HWND hParent, UINT YAxis);
//��������
HANDLE CreateDefaultFont();
//�����ı� �ı�����ΪGBK 2312
VOID LoadFile(LPWSTR file);
//�����ı�
DWORD CALLBACK DisplayStreamInCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

VOID CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime);
VOID ShowScore(HWND hwnd);
//��������
VOID InPuting(UINT YAxis);