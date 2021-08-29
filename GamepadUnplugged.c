#include "Main.h"
#include "GamepadUnplugged.h"

void DrawGamepadUnpluggedScreen(void)
{
	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
	BlitStringToBuffer("Gamepad Disconnected!",
		&g_6x7_font,
		&((PIXEL32) {{0xff, 0xff, 0xff, 0xff}}),
		(GAME_RES_WIDTH / 2) - (int16_t)(21 * 6 / 2),
		100);
	BlitStringToBuffer("Reconnect it, or press escape to continue using the keyboard.",
		&g_6x7_font,
		&((PIXEL32) {{0xff, 0xff, 0xff, 0xff}}),
		(GAME_RES_WIDTH / 2) - (int16_t)(61 * 6 / 2),
		115);
}


void PPI_GamepadUnplugged(void)
{
	if (g_gamepad_id > -1 || (PRESSED_ESCAPE))
	{
		g_current_game_state = g_previous_game_state;
		g_previous_game_state = GS_GAMEPAD_UNPLUGGED;
	}
}