#include "Main.h"
#include "TitleScreen.h"

// Title screen

MENUITEM gMI_ResumeGame = { "Resume", (GAME_RES_WIDTH / 2) - ((6 * 6) / 2), 100, FALSE, MenuItem_TitleScreen_Resume };
MENUITEM gMI_StartNewGame = { "Start New Game", (GAME_RES_WIDTH / 2) - ((14 * 6) / 2), 115, TRUE, MenuItem_TitleScreen_StartNew };
MENUITEM gMI_Options = { "Options", (GAME_RES_WIDTH / 2) - ((7 * 6) / 2), 130, TRUE, MenuItem_TitleScreen_Options };
MENUITEM gMI_Exit = { "Exit", (GAME_RES_WIDTH / 2) - ((4 * 6) / 2), 145, TRUE, MenuItem_TitleScreen_Exit };
MENUITEM* gMI_TitleScreenItems[] = { &gMI_ResumeGame, &gMI_StartNewGame, &gMI_Options, &gMI_Exit };
MENU gMenu_TitleScreen = { "Title Screen Menu", 1, _countof(gMI_TitleScreenItems), gMI_TitleScreenItems };

void DrawTitleScreen(void)
{
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	static PIXEL32 TextColor = { 0x00, 0x00, 0x00, 0x00 };

	if (gPerformanceData.TotalFramesRendered > LastFrameSeen + 1)
	{
		LocalFrameCounter = 0;
		TextColor.Red = 0;
		TextColor.Green = 0;
		TextColor.Blue = 0;
		if (gPlayer.Active)
		{
			gMenu_TitleScreen.SelectedItem = 0;
		}
		else
		{
			gMenu_TitleScreen.SelectedItem = 1;
		}
	}

	memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	if ((LocalFrameCounter > 0) && (LocalFrameCounter <= 45) && (LocalFrameCounter % 15 == 0))
	{
		TextColor.Red += 64;
		TextColor.Green += 64;
		TextColor.Blue += 64;
	}
	if (LocalFrameCounter == 60)
	{
		TextColor.Red = 255;
		TextColor.Green = 255;
		TextColor.Blue = 255;
	}

	BlitStringToBuffer(GAME_NAME, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (uint16_t)(strlen(GAME_NAME) * 6 / 2), 60);

	for (uint8_t MenuItem = 0; MenuItem < gMenu_TitleScreen.ItemCount; MenuItem++)
	{

		if (gMenu_TitleScreen.Items[MenuItem]->Enabled)
		{
			BlitStringToBuffer(gMenu_TitleScreen.Items[MenuItem]->Name,
				&g6x7Font,
				&TextColor,
				gMenu_TitleScreen.Items[MenuItem]->X,
				gMenu_TitleScreen.Items[MenuItem]->Y);
		}
	}

	BlitStringToBuffer("�",
		&g6x7Font,
		&TextColor,
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->X - 6,
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Y);

	LocalFrameCounter++;
	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}

void PPI_TitleScreen(void)
{

	if (PRESSED_DOWN)
	{
		if (gMenu_TitleScreen.SelectedItem < gMenu_TitleScreen.ItemCount - 1)
		{
			gMenu_TitleScreen.SelectedItem++;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_TitleScreen.SelectedItem > 0)
		{
			if (gMenu_TitleScreen.SelectedItem == 1)	// Don't move to "Resume" if there's no game currently in progress
			{
				if (gPlayer.Active)
				{
					gMenu_TitleScreen.SelectedItem--;
					PlayGameSound(&gSoundMenuNavigate);
				}
			}
			else
			{
				gMenu_TitleScreen.SelectedItem--;
				PlayGameSound(&gSoundMenuNavigate);
			}
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
		PlayGameSound(&gSoundMenuChoose);
	}

}


void MenuItem_TitleScreen_Options(void)
{
	gPreviousGameState = gCurrentGameState;
	gCurrentGameState = GS_OPTIONSSCREEN;
}

void MenuItem_TitleScreen_Exit(void)
{
	gPreviousGameState = gCurrentGameState;
	gCurrentGameState = GS_EXITYESNOSCREEN;
}


void MenuItem_TitleScreen_Resume(void)
{

}

void MenuItem_TitleScreen_StartNew(void)
{
	// prompt for new game if they're already in a game, otherwise just go to the character naming screen.
	gPreviousGameState = gCurrentGameState;
	gCurrentGameState = GS_CHARACTERNAMING;
}