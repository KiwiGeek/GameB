#include "Main.h"
#include "ExitYesNoScreen.h"

// Exit Yes or No Screen

MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - ((3 * 6) / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };
MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH / 2) - ((2 * 6) / 2), 115, TRUE, MenuItem_ExitYesNo_No };
MENUITEM* gMI_ExitYesNoItems[] = { &gMI_ExitYesNo_Yes , &gMI_ExitYesNo_No };
MENU gMenu_ExitYesNo = { "Are you sure you want to exit?", 1, _countof(gMI_ExitYesNoItems), gMI_ExitYesNoItems };

void DrawExitYesNoScreen(void)
{

	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;

	if (g_performance_data.TotalFramesRendered > (last_frame_seen+ 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		g_input_enabled = FALSE;
	}

	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	if (local_frame_counter == 10)
	{
		text_color.colors.Red = 64;
		text_color.colors.Green = 64;
		text_color.colors.Blue = 64;
	}

	if (local_frame_counter == 20)
	{
		text_color.colors.Red = 128;
		text_color.colors.Green = 128;
		text_color.colors.Blue = 128;
	}

	if (local_frame_counter == 30)
	{
		text_color.colors.Red = 192;
		text_color.colors.Green = 192;
		text_color.colors.Blue = 192;
	}

	if (local_frame_counter == 40)
	{
		text_color.colors.Red = 255;
		text_color.colors.Green = 255;
		text_color.colors.Blue = 255;
		g_input_enabled = TRUE;
	}

	BlitStringToBuffer(gMenu_ExitYesNo.Name, &g_6x7_font, &text_color, (int16_t)((GAME_RES_WIDTH / 2) - (uint16_t)(strlen(gMenu_ExitYesNo.Name) * 6 / 2)), 60);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[0]->Name, &g_6x7_font, &text_color, gMenu_ExitYesNo.Items[0]->X, gMenu_ExitYesNo.Items[0]->Y);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[1]->Name, &g_6x7_font, &text_color, gMenu_ExitYesNo.Items[1]->X, gMenu_ExitYesNo.Items[1]->Y);

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&text_color,
		(int16_t)(gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->X - 6),
		gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_ExitYesNo(void)
{

	if (PRESSED_DOWN)
	{
		if (gMenu_ExitYesNo.SelectedItem < gMenu_ExitYesNo.ItemCount - 1)
		{
			gMenu_ExitYesNo.SelectedItem++;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_ExitYesNo.SelectedItem > 0)
		{
			gMenu_ExitYesNo.SelectedItem--;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->Action();
		PlayGameSound(&g_sound_menu_choose);
	}

}


void MenuItem_ExitYesNo_Yes(void)
{
	SendMessageA(g_game_window, WM_CLOSE, 0, 0);
}

void MenuItem_ExitYesNo_No(void)
{
	g_current_game_state = g_previous_game_state;
	g_previous_game_state = GS_EXIT_YES_NO_SCREEN;
}
