#include"login.h"

wchar_t username[MAXLEN] = { 0 };
wchar_t password[MAXLEN] = { 0 };
wchar_t password2[MAXLEN] = { 0 };

//wchar_t s1[] = L"�콭��";
//wchar_t s2[] = L"1234";

//��������ļ����û�����
wchar_t MAXIDW[10] = { '\0' };
int MAXID = 0;
int current_user;
//�û������ļ�Ŀ¼
wchar_t config_file_name[] = L".\\config.ini";
//����һ��handle�������ڱ�ʶ�����ļ��Ƿ��Ѿ�����
HANDLE find_file;

//����һ��ȫ�ֱ������ڴ�ž��
HINSTANCE hinst;
// ������ڵ�
int LogIn(HINSTANCE hInstance)
{
	//����һ�����г����ʱ���ж������ļ��Ƿ����
	WIN32_FIND_DATA wfd;
	find_file = FindFirstFileW(config_file_name, &wfd);
	if (find_file == INVALID_HANDLE_VALUE)
		WritePrivateProfileStringW(L"Count", L"count", L"0", config_file_name);
	//��ȡ����ID
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
		if (MessageBox(hdlg, L"ȷ���˳�����", L"��ʾ", MB_OKCANCEL) == IDOK)
			PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDSIGNIN:
			GetDlgItemTextW(hdlg, IDC_SIGNINNAME, username, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_SIGNINPWD, password, MAXLEN);
			//�ж�������Ϣ�������ļ����Ƿ�һ��

			if (judge(username, password))
				EndDialog(hdlg, TRUE);
			else
			{
				MessageBox(hdlg, L"�û������������", L"��ʾ", MB_OK);
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
		if (MessageBox(hdlg, L"ȷ���˳�ע�᣿", L"��ʾ", MB_OKCANCEL) == IDOK)
		{
			EndDialog(hdlg, IDOK);
			DialogBox(hinst, MAKEINTRESOURCE(IDD_SIGNIN), NULL, DialogProcSignIn);
		}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDOK:
			//��ȡ�û�����
			GetDlgItemTextW(hdlg, 1002, username, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_EDITPWD, password, MAXLEN);
			GetDlgItemTextW(hdlg, IDC_EDITPWD2, password2, MAXLEN);
			//�ж��û������Ƿ����Ҫ��
			//�������볤�ȣ�
			if (wcscmp(password, password2))
				MessageBox(hdlg, L"���벻һ��", L"��ʾ", MB_OK);
			else if (!(*password || *password2 || *username))
				MessageBox(hdlg, L"�û��������벻��Ϊ��", L"��ʾ", MB_OK);
			else if (judge_username(username))
				MessageBox(hdlg, L"�û����ظ�", L"��ʾ", MB_OK);
			else
			{
				//TODO����ֹ�û����ظ�

				//���û�����д�������ļ�������count
				MAXID++;
				_itow(MAXID, MAXIDW, 10);
				WritePrivateProfileStringW(MAXIDW, L"UserName", username, config_file_name);
				WritePrivateProfileStringW(MAXIDW, L"PassWord", password, config_file_name);
				WritePrivateProfileStringW(L"Count", L"count", MAXIDW, config_file_name);

				MessageBox(hdlg, L"ע��ɹ�", L"��ʾ", MB_OK);
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
		GetPrivateProfileStringW(ID_Str, L"UserName", L"�û�����ȡʧ��", temp_username, MAXLEN, config_file_name);
		GetPrivateProfileStringW(ID_Str, L"PassWord", L"�����ȡʧ��", temp_password, MAXLEN, config_file_name);
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
		GetPrivateProfileStringW(ID_Str, L"UserName", L"�û�����ȡʧ��", temp_username, MAXLEN, config_file_name);

		if (!lstrcmpW(username, temp_username))
			return true;
	}
	return false;
}