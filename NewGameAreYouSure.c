#include "Main.h"
#include "NewGameAreYouSure.h"

#define PROGRESS_WILL_BE_LOAD "All unsaved progress will be lost!"

MENUITEM gMI_NewGameAreYouSure_Yes = { "Yes", (GAME_RES_WIDTH / 2) - ((3 * 6) / 2), 100, TRUE, MenuItem_NewGameAreYouSure_Yes };
MENUITEM gMI_NewGameAreYouSure_No = { "No", (GAME_RES_WIDTH / 2) - ((2 * 6) / 2), 115, TRUE, MenuItem_NewGameAreYouSure_No };
MENUITEM* gMI_NewGameAreYouSureItems[] = { &gMI_NewGameAreYouSure_Yes , &gMI_NewGameAreYouSure_No };
MENU gMenu_NewGameAreYouSure = { "Are you sure you want to start a new game?", 1, _countof(gMI_NewGameAreYouSureItems), gMI_NewGameAreYouSureItems };

void DrawNewGameAreYouSureScreen(void)
{

	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen = 0;
	static PIXEL32 text_color;

	if (g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		g_input_enabled = FALSE;
		gMenu_NewGameAreYouSure.SelectedItem = 1;
	}

	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	ApplyFadeIn(local_frame_counter, COLOR_NES_WHITE, &text_color, NULL);

	BlitStringToBuffer(gMenu_NewGameAreYouSure.Name, &g_6x7_font, &text_color, (int16_t)((GAME_RES_WIDTH / 2) - (strlen(gMenu_NewGameAreYouSure.Name) * 6 / 2)), 60);
	BlitStringToBuffer(PROGRESS_WILL_BE_LOAD, &g_6x7_font, &text_color, (int16_t)((GAME_RES_WIDTH / 2) - (strlen(PROGRESS_WILL_BE_LOAD) * 6 / 2)), 75);
	BlitStringToBuffer(gMenu_NewGameAreYouSure.Items[0]->Name, &g_6x7_font, &text_color, gMenu_NewGameAreYouSure.Items[0]->X, gMenu_NewGameAreYouSure.Items[0]->Y);
	BlitStringToBuffer(gMenu_NewGameAreYouSure.Items[1]->Name, &g_6x7_font, &text_color, gMenu_NewGameAreYouSure.Items[1]->X, gMenu_NewGameAreYouSure.Items[1]->Y);

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&text_color,
		(int16_t)(gMenu_NewGameAreYouSure.Items[gMenu_NewGameAreYouSure.SelectedItem]->X - 6),
		gMenu_NewGameAreYouSure.Items[gMenu_NewGameAreYouSure.SelectedItem]->Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_NewGameAreYouSure(void)
{
	if (PRESSED_DOWN)
	{
		if (gMenu_NewGameAreYouSure.SelectedItem < gMenu_NewGameAreYouSure.ItemCount - 1)
		{
			gMenu_NewGameAreYouSure.SelectedItem++;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_NewGameAreYouSure.SelectedItem > 0)
		{
			gMenu_NewGameAreYouSure.SelectedItem--;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_NewGameAreYouSure.Items[gMenu_NewGameAreYouSure.SelectedItem]->Action();
		PlayGameSound(&g_sound_menu_choose);
	}
}

void MenuItem_NewGameAreYouSure_Yes(void)
{
	// TODO: RESET EVERYTHING, including reset hero to all defaults
	// TODO: Flush sound buffers
	ASSERT(g_current_game_state == GS_NEW_GAME_ARE_YOU_SURE, "Invalid game state!");
	ResetEverythingForNewGame();
}

void MenuItem_NewGameAreYouSure_No(void)
{
	ASSERT(g_current_game_state == GS_NEW_GAME_ARE_YOU_SURE, "Invalid game state!");
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_TITLE_SCREEN;
	LogMessageA(LL_INFO, "[%s] Transitioning from game state %d to %d. Player selected '%s' from '%s' menu.", __FUNCTION__, g_previous_game_state, g_current_game_state, gMenu_NewGameAreYouSure.Items[gMenu_NewGameAreYouSure.SelectedItem]->Name, gMenu_NewGameAreYouSure.Name);
}