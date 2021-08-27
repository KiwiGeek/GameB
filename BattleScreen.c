#include "Main.h"
#include "BattleScreen.h"

void DrawBattleScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;

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

	if (local_frame_counter == 10)
	{
		brightness_adjustment = -128;
	}
	if (local_frame_counter == 20)
	{
		brightness_adjustment = -64;
	}
	if (local_frame_counter == 30)
	{
		brightness_adjustment = -32;
	}
	if (local_frame_counter == 40)
	{
		brightness_adjustment = 0;
		g_input_enabled = TRUE;
	}

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	DrawWindow(0, 0, 96, 96, (PIXEL32) { { 0x00, 0x00, 0x00 , 0xFF} }, WF_HORIZONTALLY_CENTERED | WF_VERTICALLY_CENTERED);

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