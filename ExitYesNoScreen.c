#include "Main.h"
#include "ExitYesNoScreen.h"

// Exit Yes or No Screen

MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - ((3 * 6) / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };
MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH / 2) - ((2 * 6) / 2), 115, TRUE, MenuItem_ExitYesNo_No };
MENUITEM* gMI_ExitYesNoItems[] = { &gMI_ExitYesNo_Yes , &gMI_ExitYesNo_No };
MENU gMenu_ExitYesNo = { "Are you sure you want to exit?", 1, _countof(gMI_ExitYesNoItems), gMI_ExitYesNoItems };

void DrawExitYesNoScreen(void)
{
	PIXEL32 White = { 0xFF, 0xFF, 0xFF, 0xFF };
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
	BlitStringToBuffer(gMenu_ExitYesNo.Name, &g6x7Font, &White, (GAME_RES_WIDTH / 2) - (uint16_t)(strlen(gMenu_ExitYesNo.Name) * 6 / 2), 60);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[0]->Name, &g6x7Font, &White, gMenu_ExitYesNo.Items[0]->X, gMenu_ExitYesNo.Items[0]->Y);
	BlitStringToBuffer(gMenu_ExitYesNo.Items[1]->Name, &g6x7Font, &White, gMenu_ExitYesNo.Items[1]->X, gMenu_ExitYesNo.Items[1]->Y);

	BlitStringToBuffer("»",
		&g6x7Font,
		&White,
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
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_ExitYesNo.SelectedItem > 0)
		{
			gMenu_ExitYesNo.SelectedItem--;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_ExitYesNo.Items[gMenu_ExitYesNo.SelectedItem]->Action();
		PlayGameSound(&gSoundMenuChoose);
	}

}


void MenuItem_ExitYesNo_Yes(void)
{
	SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
}

void MenuItem_ExitYesNo_No(void)
{
	gCurrentGameState = gPreviousGameState;
	gPreviousGameState = GS_EXITYESNOSCREEN;
}
