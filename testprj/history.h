#pragma once
#include"IniIo.h"
#include<Windows.h>
#include<strsafe.h>
#include<stdlib.h>
typedef struct SCORE {
	UINT word_count;
	UINT error_count;
	FLOAT accuracy;
	UINT key_count;
	UINT back;
	UINT speed;
	UINT time;
}score;
VOID addHistory(score toAdd, UINT ID);
score* readHistory(score* out, UINT ID);
