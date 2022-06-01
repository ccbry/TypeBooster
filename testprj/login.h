//为了避免一些报错
#define _CRT_SECURE_NO_DEPRECATE
#define MAXLEN 13


#include<Windows.h>
#include<windowsx.h>
#include"resource.h"


wchar_t username[MAXLEN] = { 0 };
wchar_t password[MAXLEN] = { 0 };
wchar_t password2[MAXLEN] = { 0 };

//wchar_t s1[] = L"朱江云";
//wchar_t s2[] = L"1234";

//标记配置文件中用户个数
wchar_t MAXIDW[10] = { '\0' };
int MAXID = 0;
int current_user;
//用户配置文件目录
wchar_t config_file_name[] = L".\\config.ini";
//创建一个handle变量用于标识配置文件是否已经存在
HANDLE find_file;

//创建一个全局变量用于存放句柄
HINSTANCE hinst;


//登录界面
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

//注册界面
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//判断用户名和密码是否正确
bool judge(wchar_t username[], wchar_t password[]);
//判断用户名是否已经存在
bool judge_username(wchar_t username[]);

int LogIn(HINSTANCE);