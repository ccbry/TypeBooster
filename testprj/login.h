//Ϊ�˱���һЩ����
#define _CRT_SECURE_NO_DEPRECATE
#define MAXLEN 13


#include<Windows.h>
#include<windowsx.h>
#include"resource.h"


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


//��¼����
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

//ע�����
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//�ж��û����������Ƿ���ȷ
bool judge(wchar_t username[], wchar_t password[]);
//�ж��û����Ƿ��Ѿ�����
bool judge_username(wchar_t username[]);

int LogIn(HINSTANCE);