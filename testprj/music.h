#pragma once
#include<Windows.h>
#include<mmsystem.h>
#include"globle.h"
#pragma comment(lib,"WINMM.LIB")
//Below are function prototypes and constants used in multimedia
VOID musicLoad();
VOID musicPlay();
VOID musicPause();
VOID musicResume();
VOID musicErrorCatching(MCIERROR err, HWND hwnd);
