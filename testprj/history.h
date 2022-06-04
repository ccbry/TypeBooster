#pragma once
#include"IniIo.h"
#include<Windows.h>
#include<strsafe.h>
#include<stdlib.h>
typedef struct SCORE {
	UINT word_count;
	UINT error_count;
	DOUBLE accuracy;
	UINT key_count;
	UINT back;
	DOUBLE speed;
	UINT time;
}score;
VOID addHistory(score toAdd, UINT ID);
score* readHistory(score* out, UINT ID);
