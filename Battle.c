#include "Main.h"
#include "Battle.h"

void GenerateMonster(void)
{
	
}

void DrawBattle(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;
	GAME_BITMAP* battle_scene = NULL;

	if (g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		brightness_adjustment = -255;
		g_input_enabled = FALSE;
	}

	if (local_frame_counter == 0)
	{
		StopMusic();
		PlayGameMusic(&g_music_battle_intro01, FALSE, TRUE);
		PlayGameMusic(&g_music_battle01, TRUE, FALSE);
	}

	ApplyFadeIn(local_frame_counter, COLOR_NES_WHITE, &text_color, &brightness_adjustment);

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	//DrawWindow(0, 0, 96, 96, COLOR_NES_WHITE, WF_HORIZONTALLY_CENTERED | WF_VERTICALLY_CENTERED | WF_BORDERED);

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
			ASSERT(FALSE, "Random monster encounter on an unknown tile!");  // NOLINT(clang-diagnostic-extra-semi-stmt)
		}
	}

	if (battle_scene != 0)
	{
		Blit32BppBitmapToBuffer(battle_scene, 144, 72, brightness_adjustment);
	}
	else
	{
		ASSERT(FALSE, "Battle Scene is NULL!");  // NOLINT(clang-diagnostic-extra-semi-stmt)
	}


	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_Battle(void)
{
	if (PRESSED_ESCAPE)
	{
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_OVERWORLD;
		StopMusic();
	}
}
