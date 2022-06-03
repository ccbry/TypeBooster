//为了避免一些报错
#define _CRT_SECURE_NO_DEPRECATE
#define MAXLEN 13

#include<Windows.h>
#include<windowsx.h>
#include"resource.h"



//登录界面
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

//注册界面
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//判断用户名和密码是否正确
bool judge(wchar_t username[], wchar_t password[]);
//判断用户名是否已经存在
bool judge_username(wchar_t username[]);

int LogIn(HINSTANCE);