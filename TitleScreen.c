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
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color = { 0x00, 0x00, 0x00, 0x00 };

	if (g_performance_data.TotalFramesRendered > last_frame_seen + 1)
	{
		local_frame_counter = 0;
		text_color.Red = 0;
		text_color.Green = 0;
		text_color.Blue = 0;
		if (g_player.Active)
		{
			gMenu_TitleScreen.SelectedItem = 0;
		}
		else
		{
			gMenu_TitleScreen.SelectedItem = 1;
		}
	}

	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	if ((local_frame_counter > 0) && (local_frame_counter <= 45) && (local_frame_counter % 15 == 0))
	{
		text_color.Red += 64;
		text_color.Green += 64;
		text_color.Blue += 64;
	}
	if (local_frame_counter == 60)
	{
		text_color.Red = 255;
		text_color.Green = 255;
		text_color.Blue = 255;
	}

	BlitStringToBuffer(GAME_NAME, &g_6x7_font, &text_color, (GAME_RES_WIDTH / 2) - (uint16_t)(strlen(GAME_NAME) * 6 / 2), 60);

	for (uint8_t menu_item = 0; menu_item < gMenu_TitleScreen.ItemCount; menu_item++)
	{

		if (gMenu_TitleScreen.Items[menu_item]->Enabled)
		{
			BlitStringToBuffer(gMenu_TitleScreen.Items[menu_item]->Name,
				&g_6x7_font,
				&text_color,
				gMenu_TitleScreen.Items[menu_item]->X,
				gMenu_TitleScreen.Items[menu_item]->Y);
		}
	}

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&text_color,
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->X - 6,
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_TitleScreen(void)
{

	if (PRESSED_DOWN)
	{
		if (gMenu_TitleScreen.SelectedItem < gMenu_TitleScreen.ItemCount - 1)
		{
			gMenu_TitleScreen.SelectedItem++;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	if (PRESSED_UP)
	{
		if (gMenu_TitleScreen.SelectedItem > 0)
		{
			if (gMenu_TitleScreen.SelectedItem == 1)	// Don't move to "Resume" if there's no game currently in progress
			{
				if (g_player.Active)
				{
					gMenu_TitleScreen.SelectedItem--;
					PlayGameSound(&g_sound_menu_navigate);
				}
			}
			else
			{
				gMenu_TitleScreen.SelectedItem--;
				PlayGameSound(&g_sound_menu_navigate);
			}
		}
	}

	if (PRESSED_CHOOSE) {
		gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
		PlayGameSound(&g_sound_menu_choose);
	}

}


void MenuItem_TitleScreen_Options(void)
{
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_OPTIONS_SCREEN;
}

void MenuItem_TitleScreen_Exit(void)
{
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_EXIT_YES_NO_SCREEN;
}


void MenuItem_TitleScreen_Resume(void)
{

}

void MenuItem_TitleScreen_StartNew(void)
{
	// prompt for new game if they're already in a game, otherwise just go to the character naming screen.
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_CHARACTER_NAMING;
}
