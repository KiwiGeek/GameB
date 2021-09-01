#pragma once

typedef struct MONSTER
{
	char Name[13];
	GAME_BITMAP Sprite;
	int16_t BaseHP;
	int16_t BaseMP;
	int16_t BaseXP;
} MONSTER;

extern MONSTER g_current_monster;

void DrawBattle(void);
void PPI_Battle(void);
void GenerateMonster(void);
