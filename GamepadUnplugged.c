#include "Main.h"
#include "GamepadUnplugged.h"

void DrawGamepadUnpluggedScreen(void)
{
	memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
	BlitStringToBuffer("Gamepad Disconnected!",
		&g6x7Font,
		&((PIXEL32) { 0xff, 0xff, 0xff, 0xff }),
		(GAME_RES_WIDTH / 2) - (int16_t)(21 * 6 / 2),
		100);
	BlitStringToBuffer("Reconnect it, or press escape to continue using the keyboard.",
		&g6x7Font,
		&((PIXEL32) { 0xff, 0xff, 0xff, 0xff }),
		(GAME_RES_WIDTH / 2) - (int16_t)(61 * 6 / 2),
		115);
}


void PPI_GamepadUnplugged(void)
{
	if (gGamepadID > -1 || (PRESSED_ESCAPE))
	{
		gCurrentGameState = gPreviousGameState;
		gPreviousGameState = GS_GAMEPADUNPLUGGED;
	}
}