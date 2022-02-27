#pragma once

typedef struct SPELL
{
	const char Name[13];
	int16_t BaseDamage;
	int16_t MPCost;
} SPELL;

typedef struct MONSTER
{
	const char Name[13];
	GAME_BITMAP* Sprite;
	int16_t BaseHP;
	int16_t BaseMP;
	int16_t BaseXP;
	int16_t BaseGP;
	int16_t BaseDamage;
	uint8_t RegularAttackChance;
	uint8_t RegularCastSpellChance;
	uint8_t RegularRunChance;
	uint8_t LowHPAttackChance;
	uint8_t LowHPCastSpellChance;
	uint8_t LowHPRunChance;
	uint8_t LowHPThresholdPercent;
	uint8_t CriticalChance;
	float CriticalMultiplier;
	uint8_t DodgeChance;
	uint8_t Armor;
	uint8_t RunSuccessChance;
	const char* Emotes[3];
	const SPELL* KnownSpells[];
} MONSTER;

extern const MONSTER g_slime001;
extern const MONSTER g_rat001; 
extern MONSTER g_current_monster;

void GenerateMonster(void);
void PPI_Battle(void);
void DrawBattle(void);
void MenuItem_PlayerBattleAction_Attack(void);
void MenuItem_PlayerBattleAction_Spell(void);
void MenuItem_PlayerBattleAction_Run(void);
void MenuItem_PlayerBattleAction_Wait(void);