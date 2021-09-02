#include "Main.h"
#include "CharacterNamingScreen.h"

// Character Naming Menu

// A B C D E F G H I J K L M 
// N O P Q R S T U V W X Y Z
// a b c d e f g h i j k l m
// n o p q r s t u v w x y z
// Back                   OK
MENUITEM gMI_CharacterNaming_A = { "A", 118, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_B = { "B", 130, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_C = { "C", 142, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_D = { "D", 154, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_E = { "E", 166, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_F = { "F", 178, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_G = { "G", 190, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_H = { "H", 202, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_I = { "I", 214, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_J = { "J", 226, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_K = { "K", 238, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_L = { "L", 250, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_M = { "M", 262, 110, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_N = { "N", 118, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_O = { "O", 130, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_P = { "P", 142, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Q = { "Q", 154, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_R = { "R", 166, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_S = { "S", 178, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_T = { "T", 190, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_U = { "U", 202, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_V = { "V", 214, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_W = { "W", 226, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_X = { "X", 238, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Y = { "Y", 250, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Z = { "Z", 262, 120, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_a = { "a", 118, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_b = { "b", 130, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_c = { "c", 142, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_d = { "d", 154, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_e = { "e", 166, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_f = { "f", 178, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_g = { "g", 190, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_h = { "h", 202, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_i = { "i", 214, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_j = { "j", 226, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_k = { "k", 238, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_l = { "l", 250, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_m = { "m", 262, 130, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_n = { "n", 118, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_o = { "o", 130, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_p = { "p", 142, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_q = { "q", 154, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_r = { "r", 166, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_s = { "s", 178, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_t = { "t", 190, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_u = { "u", 202, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_v = { "v", 214, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_w = { "w", 226, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_x = { "x", 238, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_y = { "y", 250, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_z = { "z", 262, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Back = { "Back", 118, 150, TRUE, MenuItem_CharacterNaming_Back };
MENUITEM gMI_CharacterNaming_OK = { "OK", 256, 150, TRUE, MenuItem_CharacterNaming_OK };

MENUITEM* gMI_CharacterNamingItems[] = {
	&gMI_CharacterNaming_A, &gMI_CharacterNaming_B, &gMI_CharacterNaming_C, &gMI_CharacterNaming_D, &gMI_CharacterNaming_E, &gMI_CharacterNaming_F,
	&gMI_CharacterNaming_G, &gMI_CharacterNaming_H, &gMI_CharacterNaming_I, &gMI_CharacterNaming_J, &gMI_CharacterNaming_K, &gMI_CharacterNaming_L,
	&gMI_CharacterNaming_M, &gMI_CharacterNaming_N, &gMI_CharacterNaming_O, &gMI_CharacterNaming_P, &gMI_CharacterNaming_Q, &gMI_CharacterNaming_R,
	&gMI_CharacterNaming_S, &gMI_CharacterNaming_T, &gMI_CharacterNaming_U, &gMI_CharacterNaming_V, &gMI_CharacterNaming_W, &gMI_CharacterNaming_X,
	&gMI_CharacterNaming_Y, &gMI_CharacterNaming_Z,
	&gMI_CharacterNaming_a, &gMI_CharacterNaming_b, &gMI_CharacterNaming_c, &gMI_CharacterNaming_d, &gMI_CharacterNaming_e, &gMI_CharacterNaming_f,
	&gMI_CharacterNaming_g, &gMI_CharacterNaming_h, &gMI_CharacterNaming_i, &gMI_CharacterNaming_j, &gMI_CharacterNaming_k, &gMI_CharacterNaming_l,
	&gMI_CharacterNaming_m, &gMI_CharacterNaming_n, &gMI_CharacterNaming_o, &gMI_CharacterNaming_p, &gMI_CharacterNaming_q, &gMI_CharacterNaming_r,
	&gMI_CharacterNaming_s, &gMI_CharacterNaming_t, &gMI_CharacterNaming_u, &gMI_CharacterNaming_v, &gMI_CharacterNaming_w, &gMI_CharacterNaming_x,
	&gMI_CharacterNaming_y, &gMI_CharacterNaming_z,
	&gMI_CharacterNaming_Back, &gMI_CharacterNaming_OK };

MENU gMenu_CharacterNaming = { "What's your name, hero?", 0, _countof(gMI_CharacterNamingItems), gMI_CharacterNamingItems };

void DrawCharacterNamingScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;

	if (g_performance_data.TotalFramesRendered > last_frame_seen + 1)
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		brightness_adjustment = -255;
		gMenu_CharacterNaming.SelectedItem = 0;
		g_input_enabled = FALSE;
	}

	memset(g_back_buffer.Memory, 0x32, GAME_DRAWING_AREA_MEMORY_SIZE);

	ApplyFadeIn(local_frame_counter, COLOR_NES_WHITE, &text_color, &brightness_adjustment);

	DrawWindow(108, 11, 166, 18, &text_color, NULL, &COLOR_NES_BLACK, WF_BORDERED | WF_SHADOWED);
	DrawWindow(108, 105, 166, 60, &text_color, NULL, &COLOR_NES_BLACK, WF_BORDERED | WF_SHADOWED);

	BlitStringToBuffer(gMenu_CharacterNaming.Name, &g_6x7_font, &text_color, (int16_t)((GAME_RES_WIDTH / 2) - (uint16_t)(strlen(gMenu_CharacterNaming.Name) * 6 / 2)), 16);
	Blit32BppBitmapToBuffer(&g_player.Sprite[SUIT_0][FACING_DOWN_0], 153, 80, brightness_adjustment);

	for (uint8_t letter = 0; letter < 8; letter++)
	{
		if (g_player.Name[letter] == '\0')
		{
			BlitStringToBuffer("_", &g_6x7_font, &text_color, (int16_t)(173 + (letter * 6)), 89);
		}
		else
		{
			BlitStringToBuffer(&g_player.Name[letter], &g_6x7_font, &text_color, (int16_t)(173 + (letter * 6)), 89);
		}

	}

	for (uint8_t counter = 0; counter < gMenu_CharacterNaming.ItemCount; counter++)
	{
		BlitStringToBuffer(gMenu_CharacterNaming.Items[counter]->Name,
			&g_6x7_font,
			&text_color,
			gMenu_CharacterNaming.Items[counter]->X,
			gMenu_CharacterNaming.Items[counter]->Y);
	}

	BlitStringToBuffer("\xBB",
		&g_6x7_font,
		&text_color,
		(int16_t)(gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->X - 6),
		gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}


void PPI_CharacterNaming(void)
{
#define BACK_BUTTON		52
#define CAPITAL_M		12
#define LOWERCASE_N		39
#define LOWERCASE_T		46
#define OK_BUTTON		53
#define COLUMN_COUNT	13

	static uint8_t cursor_column = 0;

	// TODO: Implement carat wrap-around

	// UP
	if (PRESSED_UP)
	{
		if (gMenu_CharacterNaming.SelectedItem >= BACK_BUTTON)			// the OK and back buttons
		{
			gMenu_CharacterNaming.SelectedItem = LOWERCASE_N + cursor_column;
			PlayGameSound(&g_sound_menu_navigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem > CAPITAL_M)		// the bottm letter rows
		{
			gMenu_CharacterNaming.SelectedItem -= COLUMN_COUNT;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	// DOWN
	if (PRESSED_DOWN)
	{
		if (gMenu_CharacterNaming.SelectedItem < LOWERCASE_N)		// the top three letter rows
		{
			gMenu_CharacterNaming.SelectedItem += COLUMN_COUNT;
			PlayGameSound(&g_sound_menu_navigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem < LOWERCASE_T)	// The row above the back button
		{
			gMenu_CharacterNaming.SelectedItem = BACK_BUTTON;
			PlayGameSound(&g_sound_menu_navigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)	// the row above the OK button
		{
			gMenu_CharacterNaming.SelectedItem = OK_BUTTON;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	// LEFT 
	if (PRESSED_LEFT)
	{
		if ((((gMenu_CharacterNaming.SelectedItem) % COLUMN_COUNT) != 0) && gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)		// all the rows above Back and OK
		{
			gMenu_CharacterNaming.SelectedItem -= 1;
			cursor_column -= 1;
			PlayGameSound(&g_sound_menu_navigate);
		}

		// OK button
		else if (gMenu_CharacterNaming.SelectedItem == OK_BUTTON)
		{
			gMenu_CharacterNaming.SelectedItem = BACK_BUTTON;
			cursor_column = 0;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	// RIGHT
	if (PRESSED_RIGHT)
	{
		if ((((gMenu_CharacterNaming.SelectedItem + 1) % COLUMN_COUNT) != 0) && gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)		// all the rows above Back and OK
		{
			gMenu_CharacterNaming.SelectedItem += 1;
			cursor_column += 1;
			PlayGameSound(&g_sound_menu_navigate);
		}

		// back button
		else if (gMenu_CharacterNaming.SelectedItem == BACK_BUTTON)
		{
			gMenu_CharacterNaming.SelectedItem = OK_BUTTON;
			cursor_column = COLUMN_COUNT - 1;
			PlayGameSound(&g_sound_menu_navigate);
		}
	}

	// Select button
	if (PRESSED_CHOOSE)
	{
		gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->Action();
	}

	// Escape button
	if (PRESSED_ESCAPE)
	{
		MenuItem_CharacterNaming_Back();
	}

}


void MenuItem_CharacterNaming_Add(void)
{
	if (strlen(g_player.Name) < 8)
	{
		g_player.Name[strlen(g_player.Name)] = gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->Name[0];
		PlayGameSound(&g_sound_menu_choose);
	}
}

void MenuItem_CharacterNaming_Back(void)
{
	if (strlen(g_player.Name) < 1)
	{
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_TITLE_SCREEN;
	}
	else
	{
		g_player.Name[strlen(g_player.Name) - 1] = '\0';
	}

	PlayGameSound(&g_sound_menu_choose);
}

void MenuItem_CharacterNaming_OK(void)
{
	if (strlen(g_player.Name) > 0)
	{
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_OVERWORLD;
		g_player.Active = TRUE;

		PlayGameSound(&g_sound_menu_choose);
	}
}