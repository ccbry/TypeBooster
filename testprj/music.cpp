#include"music.h"

/*
* start music
*/
VOID musicPlay() {
	MCIERROR err;
	err = mciSendString(L"play ..\\etc\\music.WAV repeat", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* pause music
*/
VOID musicPause() {
	MCIERROR err;
	err = mciSendString(L"pause ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* continue music
*/
VOID musicResume() {
	MCIERROR err;
	err = mciSendString(L"resume ..\\etc\\music.WAV", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

/*
* catch MCI error and print.
*/
VOID musicErrorCatching(MCIERROR err, HWND hwnd) {
	LPWSTR errorText = { 0 };
	mciGetErrorString(err, errorText, 100);
	MessageBox(hwnd, errorText, L"MCI ERROR", MB_OK);
}

/*
* load music and ready to play
*/
VOID musicLoad() {
	MCIERROR err;
	err = mciSendString(L"open ..\\etc\\music.WAV TYPE MPEGVIDEO", NULL, 0, NULL);
	if (err != 0) {
		musicErrorCatching(err, hwnd);
	}
}

