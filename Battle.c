#include "Main.h"
#include "Battle.h"

MONSTER g_current_monster = {0 };
const MONSTER g_slime001 = {
	.Name = "Slime",
	.Sprite = &g_monster_sprite_slime_001,
	.BaseHP = 5,
	.BaseMP = 0,
	.BaseGP = 10,
	.BaseDamage = 1,
	.RegularAttackChance = 99,
	.RegularCastSpellChance = 0,
	.RegularRunChance = 1,
	.LowHPAttackChance = 80,
	.LowHPCastSpellChance = 0,
	.LowHPRunChance = 20,
	.LowHPThresholdPercent = 10,
	.CriticalChance = 5,
	.CriticalMultiplier = 2.0f,
	.DodgeChance = 5,
	.Armor = 0,
	.RunSuccessChance = 25,
	.Emotes = { "*jiggles menacingly*", "*spits and gurgles*", "*cold, dead-eyed stare*" },
	.KnownSpells = { 0 }
};

const MONSTER g_rat001 = {
	.Name = "Rat",
	.Sprite = &g_monster_sprite_rat_001,
	.BaseHP = 10,
	.BaseMP = 0,
	.BaseXP = 15,
	.BaseGP = 1,
	.BaseDamage = 1,
	.RegularAttackChance = 99,
	.RegularCastSpellChance = 0,
	.RegularRunChance = 1,
	.LowHPAttackChance = 80,
	.LowHPCastSpellChance = 0,
	.LowHPRunChance = 20,
	.LowHPThresholdPercent = 10,
	.CriticalChance = 5,
	.CriticalMultiplier = 2.0f,
	.DodgeChance = 5,
	.Armor = 0,
	.RunSuccessChance = 50,
	.Emotes = { "*squeak squeak*", "*whiskers twitching angrily*", "*looks like it might have rabies*" },
	.KnownSpells = { 0 }
};

const MONSTER* g_outdoor_monsters[] = { &g_slime001, &g_rat001 };
char g_battle_text_line1[64];
char g_battle_text_line2[64];
char g_battle_text_line3[64];
BOOL g_monsters_turn;

MENUITEM gMI_PlayerBattleAction_Attack = { "Attack", (GAME_RES_WIDTH / 2) - ((6 * 6) / 2), 191, TRUE, MenuItem_PlayerBattleAction_Attack };
MENUITEM gMI_PlayerBattleAction_Spell = { "Spell", 143, 200, TRUE, MenuItem_PlayerBattleAction_Spell };
MENUITEM gMI_PlayerBattleAction_Run = { "Run", 232, 200, TRUE, MenuItem_PlayerBattleAction_Run };
MENUITEM gMI_PlayerBattleAction_Wait = { "Wait", (GAME_RES_WIDTH / 2) - ((6 * 4) / 2), 210, TRUE, MenuItem_PlayerBattleAction_Wait };

MENUITEM* gMI_PlayerBattleActionItems[] = {
	&gMI_PlayerBattleAction_Attack,
	&gMI_PlayerBattleAction_Spell,
	&gMI_PlayerBattleAction_Run,
	&gMI_PlayerBattleAction_Wait };

MENU gMenu_PlayerBattleAction = { "Your action:", 0, _countof(gMI_PlayerBattleActionItems), gMI_PlayerBattleActionItems };

BOOL g_waiting_for_player_input;

void GenerateMonster(void)
{
	unsigned int random_value = 0;
	rand_s(&random_value);

	memcpy(&g_current_monster, g_outdoor_monsters[random_value % _countof(g_outdoor_monsters)], sizeof(MONSTER));

	rand_s(&random_value);

	switch (random_value % 5)
	{
		case 0:
		{
			sprintf_s(g_battle_text_line1, sizeof(g_battle_text_line1), "A wild %s approaches!", g_current_monster.Name);
			break;	
		}
		case 1:
		{
			sprintf_s(g_battle_text_line1, sizeof(g_battle_text_line1), "A %s crosses your path!", g_current_monster.Name);
			break;
		}
		case 2:
		{
			sprintf_s(g_battle_text_line1, sizeof(g_battle_text_line1), "A mean-looking %s lurches toward you!", g_current_monster.Name);
			break;
		}
		case 3:
		{
			sprintf_s(g_battle_text_line1, sizeof(g_battle_text_line1), "A %s is coming right at you!", g_current_monster.Name);
			break;
		}
		case 4:
		{
			sprintf_s(g_battle_text_line1, sizeof(g_battle_text_line1), "A %s jumps out at you!", g_current_monster.Name);
			break;
		}
		default:
		{
			ASSERT(FALSE, "random_value mod x exceeded the number of random battle texts!");
		}
	}

	ASSERT(strlen(g_battle_text_line1) > 0, "Error generating battle text!");

	rand_s(&random_value);

	if (random_value % 3 == 0)
	{
		g_monsters_turn = TRUE;
		sprintf_s(g_battle_text_line2, sizeof(g_battle_text_line2), "You are caught off guard!");
	}
	else
	{
		g_monsters_turn = FALSE;
		sprintf_s(g_battle_text_line2, sizeof(g_battle_text_line2), "You are ready for battle!");
	}

	ASSERT(strlen(g_battle_text_line2) > 0, "Error generating battle text!");

	rand_s(&random_value);

	sprintf_s(g_battle_text_line3, sizeof(g_battle_text_line3), g_current_monster.Emotes[(random_value % 3)]);

	ASSERT(strlen(g_battle_text_line3) > 0, "Error generating battle text!");

}

void PPI_Battle(void)
{

	if (g_waiting_for_player_input)
	{
		if (PRESSED_LEFT)
		{
			gMenu_PlayerBattleAction.SelectedItem = 1;
		}
		else if (PRESSED_UP)
		{
			gMenu_PlayerBattleAction.SelectedItem = 0;
		}
		else if (PRESSED_DOWN)
		{
			gMenu_PlayerBattleAction.SelectedItem = 3;
		}
		else if (PRESSED_RIGHT)
		{
			gMenu_PlayerBattleAction.SelectedItem = 2;
		}
	}

	if (PRESSED_ESCAPE)
	{
		ASSERT(g_current_game_state == GS_BATTLE, "Invalid game state!");
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_OVERWORLD;
		LogMessageA(LL_DEBUG, "[%s] Transitioning from game state %d to %d. Player hit escape while in battle.", __FUNCTION__, g_previous_game_state, g_current_game_state);
		StopMusic();
	}
}

void DrawBattle(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen = 0;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;
	static GAME_BITMAP* battle_scene = NULL;
	static uint16_t battle_text_line1_characters_to_show = 0;
	static uint16_t battle_text_line2_characters_to_show = 0;
	static uint16_t battle_text_line3_characters_to_show = 0;
	char battle_text_line1_scratch[64] = { 0 };
	char battle_text_line2_scratch[64] = { 0 };
	char battle_text_line3_scratch[64] = { 0 };

	if (g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		brightness_adjustment = -255;
		g_input_enabled = FALSE;
		battle_text_line1_characters_to_show = 0;
		battle_text_line2_characters_to_show = 0;
		battle_text_line3_characters_to_show = 0;
		g_waiting_for_player_input = FALSE;
	}

	if (local_frame_counter == 0)
	{
		StopMusic();
		PlayGameMusic(&g_music_battle_intro01, FALSE, TRUE);
		PlayGameMusic(&g_music_battle01, TRUE, FALSE);
		GenerateMonster();

		switch (g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][g_player.WorldPos.X / 16])
		{
		case TILE_GRASS_01:
		{
			battle_scene = &g_battle_scene_grasslands01;
			break;
		}
		case TILE_BRICK_01:
		{
			battle_scene = &g_battle_scene_dungeon01;
			break;
		}
		default:
		{
			ASSERT(FALSE, "Random monster encountered on unknown tile!");
		}
		}
	}

	ApplyFadeIn(local_frame_counter, COLOR_NES_WHITE, &text_color, &brightness_adjustment);

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	//DrawWindow(0, 0, 96, 96, COLOR_NES_WHITE, WF_HORIZONTALLY_CENTERED | WF_VERTICALLY_CENTERED | WF_BORDERED);

	DrawWindow(0,
		14,
		100,
		100,
		&text_color,
		&COLOR_NES_BLACK,
		&COLOR_NES_BLACK,
		WF_OPAQUE | WF_BORDERED | WF_HORIZONTALLY_CENTERED | WF_THICK | WF_ROUNDED_CORNERS | WF_SHADOWED);


	if (battle_scene != 0)
	{
		Blit32BppBitmapToBuffer(battle_scene,
			GAME_RES_WIDTH / 2 - (int16_t)battle_scene->BitmapInfo.bmiHeader.biWidth / 2,
			16,
			brightness_adjustment);
	}
	else
	{
		ASSERT(FALSE, "Battle Scene is NULL!");  // NOLINT(clang-diagnostic-extra-semi-stmt)
	}

	if (g_current_monster.Sprite->Memory != NULL)
	{
		Blit32BppBitmapToBuffer(g_current_monster.Sprite,
			GAME_RES_WIDTH / 2 - (int16_t)g_current_monster.Sprite->BitmapInfo.bmiHeader.biWidth / 2,
			48,
			brightness_adjustment);
	}

	DrawWindow(0,
		128,
		256,
		96,
		&text_color,
		&COLOR_NES_BLACK,
		&COLOR_NES_BLACK,
		WF_OPAQUE | WF_BORDERED | WF_HORIZONTALLY_CENTERED | WF_THICK | WF_ROUNDED_CORNERS | WF_SHADOWED);

	if (local_frame_counter % 3 == 0)
	{
		if (battle_text_line1_characters_to_show <= strlen(g_battle_text_line1))
		{
			battle_text_line1_characters_to_show++;
		}
	}

	snprintf(battle_text_line1_scratch, battle_text_line1_characters_to_show, "%s", g_battle_text_line1);

	BlitStringToBuffer(battle_text_line1_scratch, &g_6x7_font, &text_color, 67, 132);

	if (strlen(battle_text_line1_scratch) == strlen(g_battle_text_line1))
	{
		if (local_frame_counter % 3 == 0)
		{
			if (battle_text_line2_characters_to_show <= strlen(g_battle_text_line2))
			{
				battle_text_line2_characters_to_show++;
			}
		}

		snprintf(battle_text_line2_scratch, battle_text_line2_characters_to_show, "%s", g_battle_text_line2);

		BlitStringToBuffer(battle_text_line2_scratch, &g_6x7_font, &text_color, 67, 141);
	}

	if (strlen(battle_text_line2_scratch) == strlen(g_battle_text_line2))
	{
		if (local_frame_counter % 3 == 0)
		{
			if (battle_text_line3_characters_to_show <= strlen(g_battle_text_line3))
			{
				battle_text_line3_characters_to_show++;
			}
		}

		snprintf(battle_text_line3_scratch, battle_text_line3_characters_to_show, "%s", g_battle_text_line3);

		BlitStringToBuffer(battle_text_line3_scratch, &g_6x7_font, &text_color, 67, 150);
	}

	if (strlen(battle_text_line3_scratch) == strlen(g_battle_text_line3))
	{
		if (g_monsters_turn)
		{
			// Monster chooses an action
			// Monster can try to hit player, cast a spell on player, or run away from player.

		}
		else
		{
			g_waiting_for_player_input = TRUE;

			BlitStringToBuffer(gMenu_PlayerBattleAction.Name, &g_6x7_font, &COLOR_NES_WHITE, 67, 175);

			DrawWindow(0, 188, 120, 32, &COLOR_NES_WHITE, &COLOR_NES_BLACK, NULL,
				WF_BORDERED | WF_THICK | WF_HORIZONTALLY_CENTERED | WF_OPAQUE | WF_ROUNDED_CORNERS);

			for (uint8_t counter = 0; counter < gMenu_PlayerBattleAction.ItemCount; counter++)
			{
				BlitStringToBuffer(
					gMenu_PlayerBattleAction.Items[counter]->Name,
					&g_6x7_font,
					&COLOR_NES_WHITE,
					gMenu_PlayerBattleAction.Items[counter]->X,
					gMenu_PlayerBattleAction.Items[counter]->Y);
			}

			BlitStringToBuffer("\xBB",
				&g_6x7_font,
				&COLOR_NES_WHITE,
				gMenu_PlayerBattleAction.Items[gMenu_PlayerBattleAction.SelectedItem]->X - 6,
				gMenu_PlayerBattleAction.Items[gMenu_PlayerBattleAction.SelectedItem]->Y);
		}

	}

	DrawPlayerStatsWindow(&text_color);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void MenuItem_PlayerBattleAction_Attack(void)
{

}

void MenuItem_PlayerBattleAction_Spell(void)
{

}

void MenuItem_PlayerBattleAction_Run(void)
{

}

void MenuItem_PlayerBattleAction_Wait(void)
{

}