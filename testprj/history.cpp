#include"history.h"
VOID addHistory(score toadd, UINT ID) {
	score pre[5] = { 0 };
	readHistory(pre, ID);
	pre[4] = pre[3];
	pre[3] = pre[2];
	pre[2] = pre[1];
	pre[1] = pre[0];
	pre[0] = toadd;
	for (UINT i = 0; i < 5; i++) {
		WCHAR hst[10] = { 0 };
		StringCchPrintf(hst, 10, L"%dhis%d", ID, i);
		WCHAR accuracy[10];
		StringCchPrintf(accuracy, 10, L"%f", pre[i].accuracy);
		WriteProfile(hst, L"accuracy", accuracy, L"..\\etc\\history.ini");
		WCHAR back[5];
		StringCchPrintf(back, 5, L"%d", pre[i].back);
		WriteProfile(hst, L"back", back, L"..\\etc\\history.ini");
		WCHAR error_count[5];
		StringCchPrintf(error_count, 5, L"%d", pre[i].error_count);
		WriteProfile(hst, L"error_count", error_count, L"..\\etc\\history.ini");
		WCHAR speed[5];
		StringCchPrintf(speed, 5, L"%d", pre[i].speed);
		WriteProfile(hst, L"speed", speed, L"..\\etc\\history.ini");
		WCHAR time[5];
		StringCchPrintf(time, 5, L"%d", pre[i].time);
		WriteProfile(hst, L"speed", time, L"..\\etc\\history.ini");
		WCHAR word_count[5];
		StringCchPrintf(word_count, 5, L"%d", pre[i].word_count);
		WriteProfile(hst, L"word_count", word_count, L"..\\etc\\history.ini");
	}
}	

score* readHistory(score* out, UINT ID) {

	for (UINT i = 0; i < 5; i++) {
		WCHAR hst[10] = { 0 };
		StringCchPrintf(hst, 10, L"%dhis%d", ID, i);
		WCHAR accur[10];
		GetProfileString(hst, L"accuracy", L"0.0", accur, 10);
		out[i].accuracy = wcstof(accur, NULL);
		out[i].back = GetPrivateProfileInt(hst, L"back", 0, L"..\\etc\\history.ini");
		out[i].error_count = GetPrivateProfileInt(hst, L"error_count", 0, L"..\\etc\\history.ini");
		out[i].speed = GetPrivateProfileInt(hst, L"speed", 0, L"..\\etc\\history.ini");
		out[i].time = GetPrivateProfileInt(hst, L"time", 0, L"..\\etc\\history.ini");
		out[i].word_count = GetPrivateProfileInt(hst, L"word_count", 0, L"..\\etc\\history.ini");
	}
	return out;
}