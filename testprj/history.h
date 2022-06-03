#pragma once
#include"IniIo.h"
#include<Windows.h>
#include<strsafe.h>
typedef struct SCORE {
	UINT word_count;
	UINT error_count;
	FLOAT accuracy;
	UINT back;
	UINT speed;
	UINT time;
}score;
VOID addHistory(score toAdd, UINT ID);
score* readHistory(score* out, UINT ID);
