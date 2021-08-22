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
	PIXEL32 grey = { 0x6F, 0x6F, 0x6F, 0x6F };
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color = { 0x00, 0x00, 0x00, 0x00 };
	char screen_size_string[64] = { 0 };

	if (g_performance_data.TotalFramesRendered > last_frame_seen + 1)
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		gMenu_OptionsScreen.SelectedItem = 0;
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

	for (uint8_t menu_item = 0; menu_item < gMenu_OptionsScreen.ItemCount; menu_item++)
	{

		if (gMenu_OptionsScreen.Items[menu_item]->Enabled)
		{
			BlitStringToBuffer(gMenu_OptionsScreen.Items[menu_item]->Name,
				&g_6x7_font,
				&text_color,
				gMenu_OptionsScreen.Items[menu_item]->X,
				gMenu_OptionsScreen.Items[menu_item]->Y);
		}
	}

	for (uint8_t volume = 0; volume < 10; volume++)
	{
		if (volume >= (uint8_t)(g_sfx_volume * 10))
		{
			if (text_color.colors.Red == 255)
			{
				BlitStringToBuffer("\xf2", &g_6x7_font, &grey, 224 + (volume * 6), gMI_OptionsScreen_SFXVolume.Y);
			}
		}
		else
		{
			BlitStringToBuffer("\xf2", &g_6x7_font, &text_color, 224 + (volume * 6), gMI_OptionsScreen_SFXVolume.Y);
		}
	}

	for (uint8_t volume = 0; volume < 10; volume++)
	{
		if (volume >= (uint8_t)(g_music_volume * 10))
		{
			if (text_color.colors.Red == 255)
			{
				BlitStringToBuffer("\xf2", &g_6x7_font, &grey, 224 + (volume * 6), gMI_OptionsScreen_MusicVolume.Y);
			}
		}
		else
		{
			BlitStringToBuffer("\xf2", &g_6x7_font, &text_color, 224 + (volume * 6), gMI_OptionsScreen_MusicVolume.Y);
		}
	}

	snprintf(screen_size_string, sizeof(screen_size_string), "%dx%d", GAME_RES_WIDTH * g_performance_data.CurrentScaleFactor, GAME_RES_HEIGHT * g_performance_data.CurrentScaleFactor);
	BlitStringToBuffer(screen_size_string, &g_6x7_font, &text_color, 224, gMI_OptionsScreen_ScreenSize.Y);

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&text_color,
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->X - 6,
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}


void PPI_OptionsScreen(void)
{
	if (PRESSED_DOWN)
	{
		if (gMenu_OptionsScreen.SelectedItem < gMenu_OptionsScreen.ItemCount - 1)
		{
			gMenu_OptionsScreen.SelectedItem++;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_OptionsScreen.SelectedItem > 0)
		{
			gMenu_OptionsScreen.SelectedItem--;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
		PlayGameSound(&g_sound_menu_choose);
	}

	if (PRESSED_ESCAPE)
	{
		MenuItem_OptionsScreen_Back();
	}

}


void MenuItem_OptionsScreen_Back(void)
{
	g_current_game_state = g_previous_game_state;
	g_previous_game_state = GS_OPTIONS_SCREEN;
	if (SaveRegistryParameters() != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] SaveRegistryParameters failed!", __FUNCTION__);
	}
}

void MenuItem_OptionsScreen_SFXVolume(void)
{
	g_sfx_volume += 0.1f;
	if ((uint8_t)(g_sfx_volume * 10) > 10)
	{
		g_sfx_volume = 0;
	}
	for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
	{
		g_xaudio_sfx_source_voice[Counter]->lpVtbl->SetVolume(g_xaudio_sfx_source_voice[Counter], g_sfx_volume, XAUDIO2_COMMIT_NOW);
	}
}

void MenuItem_OptionsScreen_MusicVolume(void)
{
	g_music_volume += 0.1f;
	if ((uint8_t)(g_music_volume * 10) > 10)
	{
		g_music_volume = 0;
	}
	g_xaudio_music_source_voice->lpVtbl->SetVolume(g_xaudio_music_source_voice, g_music_volume, XAUDIO2_COMMIT_NOW);
}

void MenuItem_OptionsScreen_ScreenSize(void)
{
	g_performance_data.CurrentScaleFactor++;
	if (g_performance_data.CurrentScaleFactor > g_performance_data.MaxScaleFactor)
	{
		g_performance_data.CurrentScaleFactor = 1;
	}

	InvalidateRect(g_game_window, NULL, TRUE);
}