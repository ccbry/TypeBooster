#pragma once
#include<Windows.h>
#include<windowsx.h>
// Below are function prototypes and constants used in "ini" file IO;
VOID WriteProfile(LPCWSTR section, LPCWSTR item, LPCWSTR info, LPCWSTR relectivePath);
typedef struct {
	WCHAR nameArray[64][128];
}Infile;
Infile* readProfile(Infile* ret);