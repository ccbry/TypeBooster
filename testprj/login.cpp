#include"login.h"

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
// 程序入口点
int LogIn(HINSTANCE hInstance)
{
	//当第一次运行程序的时候，判断配置文件是否存在
	WIN32_FIND_DATA wfd;
	find_file = FindFirstFileW(config_file_name, &wfd);
	if (find_file == INVALID_HANDLE_VALUE)
		WritePrivateProfileStringW(L"Count", L"count", L"0", config_file_name);
	//读取最大的ID
	GetPrivateProfileStringW(L"Count", L"count", L"-1", MAXIDW, 10, config_file_name);
	MAXID = _wtoi(MAXIDW);
	FindClose(find_file);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SIGNIN), NULL, DialogProcSignIn);
	return current_user;
}

INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(hdlg, L"确定退出程序？", L"提示", MB_OKCANCEL) == IDOK)
			PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDSIGNIN:
			GetDlgItemTextW(hdlg, IDC_SIGNINNAME, username, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_SIGNINPWD, password, MAXLEN);
			//判断输入信息与配置文件中是否一致

			if (judge(username, password))
				EndDialog(hdlg, TRUE);
			else
			{
				MessageBox(hdlg, L"用户名或密码错误", L"提示", MB_OK);
				SetWindowTextW(GetDlgItem(hdlg, IDC_SIGNINNAME), NULL);
				SetWindowTextW(GetDlgItem(hdlg, IDC_SIGNINPWD), NULL);
			}
			break;

		case IDSIGNUP:
			EndDialog(hdlg, IDSIGNUP);
			DialogBox(hinst, MAKEINTRESOURCE(IDD_SIGNUP), NULL, DialogProcSignUp);
			break;
		}
	}
	}
	return (INT_PTR)false;
}
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(hdlg, L"确定退出注册？", L"提示", MB_OKCANCEL) == IDOK)
		{
			EndDialog(hdlg, IDOK);
			DialogBox(hinst, MAKEINTRESOURCE(IDD_SIGNIN), NULL, DialogProcSignIn);
		}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDOK:
			//获取用户输入
			GetDlgItemTextW(hdlg, 1002, username, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_EDITPWD, password, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_EDITPWD2, password2, MAXLEN);
			//判断用户输入是否符合要求
			//限制输入长度？
			if (wcscmp(password, password2))
				MessageBox(hdlg, L"密码不一致", L"提示", MB_OK);
			else if (!(*password || *password2 || *username))
				MessageBox(hdlg, L"用户名和密码不能为空", L"提示", MB_OK);
			else if (judge_username(username))
				MessageBox(hdlg, L"用户名重复", L"提示", MB_OK);
			else
			{
				//TODO：防止用户名重复

				//将用户输入写入配置文件，更新count
				MAXID++;
				_itow(MAXID, MAXIDW, 10);
				WritePrivateProfileStringW(MAXIDW, L"UserName", username, config_file_name);
				WritePrivateProfileStringW(MAXIDW, L"PassWord", password, config_file_name);
				WritePrivateProfileStringW(L"Count", L"count", MAXIDW, config_file_name);

				MessageBox(hdlg, L"注册成功", L"提示", MB_OK);
				EndDialog(hdlg, IDOK);
				DialogBox(hinst, MAKEINTRESOURCE(IDD_SIGNIN), NULL, DialogProcSignIn);
			}
			break;
		case IDOK2:
			EndDialog(hdlg, IDOK2);
			DialogBox(hinst, MAKEINTRESOURCE(IDD_SIGNIN), NULL, DialogProcSignIn);
			break;
		}
	}
	}
	return (INT_PTR)false;
}

bool judge(wchar_t username[], wchar_t password[])
{
	wchar_t ID_Str[10];
	wchar_t temp_username[MAXLEN];
	wchar_t temp_password[MAXLEN];
	for (int i = 1; i <= MAXID; i++)
	{
		_itow(i, ID_Str, 10);
		GetPrivateProfileStringW(ID_Str, L"UserName", L"用户名读取失败", temp_username, MAXLEN, config_file_name);
		GetPrivateProfileStringW(ID_Str, L"PassWord", L"密码读取失败", temp_password, MAXLEN, config_file_name);
		if (!(lstrcmpW(username, temp_username)) && !(lstrcmpW(password, temp_password)))
		{
			current_user = i;
			return true;
		}
	}
	return false;
}

bool judge_username(wchar_t username[])
{
	wchar_t ID_Str[10];
	wchar_t temp_username[MAXLEN];

	for (int i = 1; i <= MAXID; i++)
	{
		_itow(i, ID_Str, 10);
		GetPrivateProfileStringW(ID_Str, L"UserName", L"用户名读取失败", temp_username, MAXLEN, config_file_name);

		if (!lstrcmpW(username, temp_username))
			return true;
	}
	return false;
}