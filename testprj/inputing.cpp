#include"inputing.h"
#include"globle.h"

UINT PreLen = 0;
//创建展示文本框
VOID CreateDisplay(HINSTANCE hInstance, HWND hParent, UINT Yaxis)
{
    RECT rect;


    GetClientRect(hParent, &rect);
    LoadLibrary(TEXT("Msftedit.dll"));

    hDisplay = CreateWindowEx(0, L"RICHEDIT50W", TEXT(""),
        ES_AUTOVSCROLL | ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_READONLY,
        50, Yaxis, rect.right - 100, rect.bottom / 3 - 23,
        hParent, NULL, hinstance, NULL);
    LoadFile(returnPath);
    //SendMessageW(hDisplay, EM_SETEVENTMASK, TRUE, (LPARAM)EN_PROTECTED);

    SendMessageW(hDisplay, WM_SETFONT, (WPARAM)CreateDefaultFont(), TRUE);
}

VOID CreateInput(HINSTANCE hInstance, HWND hParent, UINT YAxis)
{
    RECT rect;

    GetClientRect(hParent, &rect);
    LoadLibrary(TEXT("Msftedit.dll"));
    hInPut = CreateWindow(L"RICHEDIT50W", TEXT(""),
        ES_AUTOVSCROLL | ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
        50, (rect.bottom - YAxis) / 2, rect.right - 100, rect.bottom - 400,
        hParent, NULL, hInstance, NULL);

    SendMessageW(hInPut, EM_SETEVENTMASK, 0, ENM_CHANGE | ENM_KEYEVENTS);
    SendMessageW(hInPut, WM_SETFONT, (WPARAM)CreateDefaultFont(), TRUE);
}

HANDLE CreateDefaultFont()
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));

    // 设置字体为Courier New
    lf.lfHeight = 40;
    lf.lfWidth = 18;
    lf.lfWeight = 400;
    lf.lfOutPrecision = 3;
    lf.lfClipPrecision = 2;
    lf.lfQuality = PROOF_QUALITY;
    lf.lfPitchAndFamily = 1;
    StringCchCopyW((STRSAFE_LPWSTR)&lf.lfFaceName, 32, L"Courier New");

    return CreateFontIndirectW(&lf);
}

VOID LoadFile(LPWSTR file)
{
    HANDLE hFile;
    hFile = CreateFileW(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    EDITSTREAM es = { 0 };
    es.dwCookie = (DWORD_PTR)hFile;
    es.pfnCallback = &DisplayStreamInCallback;
    SendMessage(hDisplay, EM_STREAMIN, SF_TEXT, (LPARAM)&es);

    CloseHandle(hFile);
    PreLen = 0;
    //清零计分结构体
    memset(&DisplayScore, 0, sizeof(DisplayScore));
}
DWORD CALLBACK DisplayStreamInCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
    HANDLE hFile = (HANDLE)dwCookie;
    DWORD NumberOfBytesRead;
    if (!ReadFile(hFile, pbBuff, cb, &NumberOfBytesRead, NULL))
    {
        return 1;
    }
    *pcb = NumberOfBytesRead;

    return 0;
}

VOID InPuting(UINT YAxis)
{
    INT len = GetWindowTextLengthW(hInPut) + 1;
    WCHAR TempInPut[1000];
    GetWindowTextW(hInPut, TempInPut, len);
    FINDTEXTEX ftex;
    CHARRANGE range;
    WCHAR temptarget[2] = {0};
    int position;
    CHARFORMAT2 cf;
    //修正量
    INT b = 0;
    //统计错误
    INT error = 0;

    for (int i = 1; i < len; i++)
    {
        temptarget[0] = TempInPut[i - 1];

        if (temptarget[0] == '\n')
        {
            b++;
            continue;
        }

        range.cpMin = i - 1 - b;
        range.cpMax = i - b;

        ftex.lpstrText = temptarget;
        ftex.chrg.cpMin = i - 1 - b;
        ftex.chrg.cpMax = i - b;

        position = SendMessage(hDisplay, EM_FINDTEXTEXW, (WPARAM)FR_DOWN | FR_MATCHCASE, (LPARAM)&ftex);
        SendMessage(hDisplay, EM_EXSETSEL, 0, (LPARAM)&range);

        memset(&cf, 0, sizeof cf);
        cf.cbSize = sizeof cf;
        cf.dwMask = CFM_BACKCOLOR;
        if (position == i - 1 - b)
            cf.crBackColor = RGB(192, 192, 192);
        else
        {
            cf.crBackColor = RGB(255, 0, 0);
            error++;
        }
        SendMessage(hDisplay, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    DisplayScore.word_count = len - 1 - 2 * b;
    DisplayScore.error_count = error;
    DisplayScore.accuracy = (float)(DisplayScore.word_count - DisplayScore.error_count) / DisplayScore.word_count;

    if (PreLen > len - 1)
        for (int i = len; i <= PreLen; i++)
        {
            range.cpMin = i - 1 - b;
            range.cpMax = i - b;
            SendMessage(hDisplay, EM_EXSETSEL, 0, (LPARAM)&range);
            memset(&cf, 0, sizeof cf);
            cf.cbSize = sizeof cf;
            cf.dwMask = CFM_BACKCOLOR;
            cf.crBackColor = RGB(255, 255, 255);
            SendMessage(hDisplay, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
        }
    SetFocus(hDisplay);
    SetFocus(hInPut);
    PreLen = len - 1;
}

//TODO: HideCaret