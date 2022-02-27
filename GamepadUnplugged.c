#include "Main.h"
#include "GamepadUnplugged.h"

void DrawGamepadUnpluggedScreen(void)
{
	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
	BlitStringToBuffer("Gamepad Disconnected!",
		&g_6x7_font,
		&COLOR_NES_WHITE,
		(GAME_RES_WIDTH / 2) - (int16_t)(21 * 6 / 2),
		100);
	BlitStringToBuffer("Reconnect it, or press escape to continue using the keyboard.",
		&g_6x7_font,
		&COLOR_NES_WHITE,
		(GAME_RES_WIDTH / 2) - (int16_t)(61 * 6 / 2),
		115);
}


void PPI_GamepadUnplugged(void)
{
	if (g_gamepad_id > -1 || (PRESSED_ESCAPE))
	{
		g_current_game_state = g_previous_game_state;
		g_previous_game_state = GS_GAMEPAD_UNPLUGGED;
		LogMessageA(LL_INFO, "[%s] Transitioning from game state %d to %d. Either a gamepad was reconnected or the player chose to continue without one.", __FUNCTION__, g_previous_game_state, g_current_game_state);
	}
}