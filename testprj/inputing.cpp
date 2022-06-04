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
        50, (rect.bottom - YAxis) / 2 + 70, rect.right - 100, rect.bottom - 400,
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
    INT lenDisplay = GetWindowTextLengthW(hDisplay) + 1;
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

    SetTimer(GetParent(hInPut), 1, 100, (TIMERPROC)TimerProc);

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
    DisplayScore.accuracy = (DOUBLE)(DisplayScore.word_count - DisplayScore.error_count) / DisplayScore.word_count;
    

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
    if (len == lenDisplay && DisplayScore.error_count == 0)
    {
        KillTimer(hwnd, 1);
        WCHAR scoreOut[30];
        StringCchPrintf(scoreOut, 30, L"您的用时为：%.1fs，输入速度为：%f字/秒，再接再厉！", (DOUBLE)DisplayScore.time / 1000, DisplayScore.speed);
        MessageBox(hwnd, scoreOut, L"Tip", MB_OK);
        SetWindowTextW(hInPut, L"");
        addHistory(DisplayScore, ID);
    }
    SetFocus(hDisplay);
    SetFocus(hInPut);
    PreLen = len - 1;
}

VOID ShowScore(HWND hwnd)
{
    WCHAR ScoreString[100];
    StringCchPrintf(ScoreString,
        100,
        L"字数：%d，错误数：%d，准确度：%.2f，按键次数：%d，退格次数：%d，速度：%.2f，时间：%.2f",
        DisplayScore.word_count, DisplayScore.error_count, DisplayScore.accuracy,
        DisplayScore.key_count, DisplayScore.back / 2, DisplayScore.speed, (FLOAT)DisplayScore.time / 1000);
    SetWindowText(hStatic, ScoreString);
    memset(ScoreString, 0, sizeof(ScoreString));
}


VOID CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
    DisplayScore.time += 100;
    if (DisplayScore.time == 0) {
        DisplayScore.speed = 0;
    }
    else
    {
        DOUBLE timeInSec = DisplayScore.time / 1000.0;
        DisplayScore.speed = (DOUBLE)(DisplayScore.word_count / timeInSec);
    }
    ShowScore(hwnd);
}
//TODO: HideCaret