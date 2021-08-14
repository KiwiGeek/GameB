#include "Main.h"
#include "ExitYesNoScreen.h"

// Exit Yes or No Screen

MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - ((3 * 6) / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };
MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH / 2) - ((2 * 6) / 2), 115, TRUE, MenuItem_ExitYesNo_No };
MENUITEM* gMI_ExitYesNoItems[] = { &gMI_ExitYesNo_Yes , &gMI_ExitYesNo_No };
MENU gMenu_ExitYesNo = { "Are you sure you want to exit?", 1, _countof(gMI_ExitYesNoItems), gMI_ExitYesNoItems };

void DrawExitYesNoScreen(void)
{
	PIXEL32 white = { 0xFF, 0xFF, 0xFF, 0xFF };
	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
	BlitStringToBuffer(gMenu_ExitYesNo.Name, &g_6x7_font, &white, (GAME_RES_WIDTH / 2) - (uint16_t)(strlen(gMenu_ExitYesNo.Name) * 6 / 2), 60);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[0]->Name, &g_6x7_font, &white, gMenu_ExitYesNo.Items[0]->X, gMenu_ExitYesNo.Items[0]->Y);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[1]->Name, &g_6x7_font, &white, gMenu_ExitYesNo.Items[1]->X, gMenu_ExitYesNo.Items[1]->Y);

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&white,
		gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->X - 6,
		gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->Y);
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
	g_previous_game_state = GS_EXITYESNOSCREEN;
}
