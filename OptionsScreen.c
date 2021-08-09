#include "Main.h"
#include "OptionsScreen.h"

MENUITEM gMI_OptionsScreen_SFXVolume = { "SFX Volume:", (GAME_RES_WIDTH / 2) - ((11 * 6) / 2) - 16, 100, TRUE, MenuItem_OptionsScreen_SFXVolume };
MENUITEM gMI_OptionsScreen_MusicVolume = { "Music Volume:", (GAME_RES_WIDTH / 2) - ((13 * 6) / 2) - 16, 115, TRUE, MenuItem_OptionsScreen_MusicVolume };
MENUITEM gMI_OptionsScreen_ScreenSize = { "Screen Size:", (GAME_RES_WIDTH / 2) - ((12 * 6) / 2) - 16, 130, TRUE, MenuItem_OptionsScreen_ScreenSize };
MENUITEM gMI_OptionsScreen_Back = { "Back", (GAME_RES_WIDTH / 2) - ((4 * 6) / 2) - 16, 145, TRUE, MenuItem_OptionsScreen_Back };
MENUITEM* gMI_OptionsScreenItems[] = { &gMI_OptionsScreen_SFXVolume, &gMI_OptionsScreen_MusicVolume, &gMI_OptionsScreen_ScreenSize, &gMI_OptionsScreen_Back };
MENU gMenu_OptionsScreen = { "Options", 0, _countof(gMI_OptionsScreenItems), gMI_OptionsScreenItems };

void DrawOptionsScreen(void)
{
	PIXEL32 Grey = { 0x6F, 0x6F, 0x6F, 0x6F };
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	static PIXEL32 TextColor = { 0x00, 0x00, 0x00, 0x00 };
	char ScreenSizeString[64] = { 0 };

	if (gPerformanceData.TotalFramesRendered > LastFrameSeen + 1)
	{
		LocalFrameCounter = 0;
		TextColor.Red = 0;
		TextColor.Green = 0;
		TextColor.Blue = 0;
		gMenu_OptionsScreen.SelectedItem = 0;
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

	for (uint8_t MenuItem = 0; MenuItem < gMenu_OptionsScreen.ItemCount; MenuItem++)
	{

		if (gMenu_OptionsScreen.Items[MenuItem]->Enabled)
		{
			BlitStringToBuffer(gMenu_OptionsScreen.Items[MenuItem]->Name,
				&g6x7Font,
				&TextColor,
				gMenu_OptionsScreen.Items[MenuItem]->X,
				gMenu_OptionsScreen.Items[MenuItem]->Y);
		}
	}

	for (uint8_t Volume = 0; Volume < 10; Volume++)
	{
		if (Volume >= (uint8_t)(gSFXVolume * 10))
		{
			if (TextColor.Red == 255)
			{
				BlitStringToBuffer("\xf2", &g6x7Font, &Grey, 224 + (Volume * 6), gMI_OptionsScreen_SFXVolume.Y);
			}
		}
		else
		{
			BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 224 + (Volume * 6), gMI_OptionsScreen_SFXVolume.Y);
		}
	}

	for (uint8_t Volume = 0; Volume < 10; Volume++)
	{
		if (Volume >= (uint8_t)(gMusicVolume * 10))
		{
			if (TextColor.Red == 255)
			{
				BlitStringToBuffer("\xf2", &g6x7Font, &Grey, 224 + (Volume * 6), gMI_OptionsScreen_MusicVolume.Y);
			}
		}
		else
		{
			BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 224 + (Volume * 6), gMI_OptionsScreen_MusicVolume.Y);
		}
	}

	snprintf(ScreenSizeString, sizeof(ScreenSizeString), "%dx%d", GAME_RES_WIDTH * gPerformanceData.CurrentScaleFactor, GAME_RES_HEIGHT * gPerformanceData.CurrentScaleFactor);
	BlitStringToBuffer(ScreenSizeString, &g6x7Font, &TextColor, 224, gMI_OptionsScreen_ScreenSize.Y);

	BlitStringToBuffer("»",
		&g6x7Font,
		&TextColor,
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->X - 6,
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Y);

	LocalFrameCounter++;
	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}


void PPI_OptionsScreen(void)
{
	if (PRESSED_DOWN)
	{
		if (gMenu_OptionsScreen.SelectedItem < gMenu_OptionsScreen.ItemCount - 1)
		{
			gMenu_OptionsScreen.SelectedItem++;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_OptionsScreen.SelectedItem > 0)
		{
			gMenu_OptionsScreen.SelectedItem--;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
		PlayGameSound(&gSoundMenuChoose);
	}

	if (PRESSED_ESCAPE)
	{
		MenuItem_OptionsScreen_Back();
	}

}


void MenuItem_OptionsScreen_Back(void)
{
	gCurrentGameState = gPreviousGameState;
	gPreviousGameState = GS_OPTIONSSCREEN;
	if (SaveRegistryParameters() != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] SaveRegistryParameters failed!", __FUNCTION__);
	}
}

void MenuItem_OptionsScreen_SFXVolume(void)
{
	gSFXVolume += 0.1f;
	if ((uint8_t)(gSFXVolume * 10) > 10)
	{
		gSFXVolume = 0;
	}
	for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
	{
		gXAudioSFXSourceVoice[Counter]->lpVtbl->SetVolume(gXAudioSFXSourceVoice[Counter], gSFXVolume, XAUDIO2_COMMIT_NOW);
	}
}

void MenuItem_OptionsScreen_MusicVolume(void)
{
	gMusicVolume += 0.1f;
	if ((uint8_t)(gMusicVolume * 10) > 10)
	{
		gMusicVolume = 0;
	}
	gXAudioMusicSourceVoice->lpVtbl->SetVolume(gXAudioMusicSourceVoice, gMusicVolume, XAUDIO2_COMMIT_NOW);
}

void MenuItem_OptionsScreen_ScreenSize(void)
{
	gPerformanceData.CurrentScaleFactor++;
	if (gPerformanceData.CurrentScaleFactor > gPerformanceData.MaxScaleFactor)
	{
		gPerformanceData.CurrentScaleFactor = 1;
	}

	InvalidateRect(gGameWindow, NULL, TRUE);
}