//Ϊ�˱���һЩ����
#define _CRT_SECURE_NO_DEPRECATE
#define MAXLEN 13

#include<Windows.h>
#include<windowsx.h>
#include"resource.h"



//��¼����
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

//ע�����
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//�ж��û����������Ƿ���ȷ
bool judge(wchar_t username[], wchar_t password[]);
//�ж��û����Ƿ��Ѿ�����
bool judge_username(wchar_t username[]);

int LogIn(HINSTANCE);