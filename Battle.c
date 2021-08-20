#include "Main.h"
#include "Battle.h"

void PPI_Battle(void)
{
	if (PRESSED_ESCAPE)
	{
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_OVERWORLD;
	}
}

void DrawBattleScreen(void)
{
	__stosd(g_back_buffer.Memory, 0xFF00FF00, GAME_DRAWING_AREA_MEMORY_SIZE/sizeof(DWORD));
}