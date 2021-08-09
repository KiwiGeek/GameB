#include "Main.h"
#include "CharacterNamingScreen.h"

// Character Naming Menu

// A B C D E F G H I J K L M 
// N O P Q R S T U V W X Y Z
// a b c d e f g h i j k l m
// n o p q r s t u v w x y z
// Back                   OK

MENUITEM gMI_CharacterNaming_A = { "A", 105, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_B = { "B", 119, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_C = { "C", 133, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_D = { "D", 147, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_E = { "E", 161, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_F = { "F", 175, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_G = { "G", 189, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_H = { "H", 203, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_I = { "I", 217, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_J = { "J", 231, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_K = { "K", 245, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_L = { "L", 259, 110, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_M = { "M", 273, 110, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_N = { "N", 105, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_O = { "O", 119, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_P = { "P", 133, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Q = { "Q", 147, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_R = { "R", 161, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_S = { "S", 175, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_T = { "T", 189, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_U = { "U", 203, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_V = { "V", 217, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_W = { "W", 231, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_X = { "X", 245, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Y = { "Y", 259, 120, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Z = { "Z", 273, 120, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_a = { "a", 105, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_b = { "b", 119, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_c = { "c", 133, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_d = { "d", 147, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_e = { "e", 161, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_f = { "f", 175, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_g = { "g", 189, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_h = { "h", 203, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_i = { "i", 217, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_j = { "j", 231, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_k = { "k", 245, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_l = { "l", 259, 130, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_m = { "m", 273, 130, TRUE, MenuItem_CharacterNaming_Add };

MENUITEM gMI_CharacterNaming_n = { "n", 105, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_o = { "o", 119, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_p = { "p", 133, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_q = { "q", 147, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_r = { "r", 161, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_s = { "s", 175, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_t = { "t", 189, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_u = { "u", 203, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_v = { "v", 217, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_w = { "w", 231, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_x = { "x", 245, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_y = { "y", 259, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_z = { "z", 273, 140, TRUE, MenuItem_CharacterNaming_Add };
MENUITEM gMI_CharacterNaming_Back = { "Back", 105, 150, TRUE, MenuItem_CharacterNaming_Back };
MENUITEM gMI_CharacterNaming_OK = { "OK", 267, 150, TRUE, MenuItem_CharacterNaming_OK };

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
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	static PIXEL32 TextColor = { 0x00, 0x00, 0x00, 0x00 };

	if (gPerformanceData.TotalFramesRendered > LastFrameSeen + 1)
	{
		LocalFrameCounter = 0;
		TextColor.Red = 0;
		TextColor.Green = 0;
		TextColor.Blue = 0;
		gMenu_CharacterNaming.SelectedItem = 0;
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

	BlitStringToBuffer(gMenu_CharacterNaming.Name, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (uint16_t)(strlen(gMenu_CharacterNaming.Name) * 6 / 2), 16);
	Blit32BppBitmapToBuffer(&gPlayer.Sprite[SUIT_0][FACING_DOWN_0], 153, 85);

	for (uint8_t Letter = 0; Letter < 8; Letter++)
	{
		if (gPlayer.Name[Letter] == '\0')
		{
			BlitStringToBuffer("_", &g6x7Font, &TextColor, 173 + (Letter * 6), 93);
		}
		else
		{
			BlitStringToBuffer(&gPlayer.Name[Letter], &g6x7Font, &TextColor, 173 + (Letter * 6), 93);
		}

	}

	for (uint8_t Counter = 0; Counter < gMenu_CharacterNaming.ItemCount; Counter++)
	{
		BlitStringToBuffer(gMenu_CharacterNaming.Items[Counter]->Name,
			&g6x7Font,
			&TextColor,
			gMenu_CharacterNaming.Items[Counter]->X,
			gMenu_CharacterNaming.Items[Counter]->Y);
	}

	BlitStringToBuffer("»",
		&g6x7Font,
		&TextColor,
		gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->X - 6,
		gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->Y);

	LocalFrameCounter++;
	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}


void PPI_CharacterNaming(void)
{
#define BACK_BUTTON		52
#define CAPITAL_M		12
#define LOWERCASE_N		39
#define LOWERCASE_T		46
#define OK_BUTTON		53
#define COLUMN_COUNT	13

	static uint8_t CursorColumn = 0;

	// UP
	if (PRESSED_UP)
	{
		if (gMenu_CharacterNaming.SelectedItem >= BACK_BUTTON)			// the OK and back buttons
		{
			gMenu_CharacterNaming.SelectedItem = LOWERCASE_N + CursorColumn;
			PlayGameSound(&gSoundMenuNavigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem > CAPITAL_M)		// the bottm letter rows
		{
			gMenu_CharacterNaming.SelectedItem -= COLUMN_COUNT;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	// DOWN
	if (PRESSED_DOWN)
	{
		if (gMenu_CharacterNaming.SelectedItem < LOWERCASE_N)		// the top three letter rows
		{
			gMenu_CharacterNaming.SelectedItem += COLUMN_COUNT;
			PlayGameSound(&gSoundMenuNavigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem < LOWERCASE_T)	// The row above the back button
		{
			gMenu_CharacterNaming.SelectedItem = BACK_BUTTON;
			PlayGameSound(&gSoundMenuNavigate);
		}
		else if (gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)	// the row above the OK button
		{
			gMenu_CharacterNaming.SelectedItem = OK_BUTTON;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	// LEFT 
	if (PRESSED_LEFT)
	{
		if ((((gMenu_CharacterNaming.SelectedItem) % COLUMN_COUNT) != 0) && gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)		// all the rows above Back and OK
		{
			gMenu_CharacterNaming.SelectedItem -= 1;
			CursorColumn -= 1;
			PlayGameSound(&gSoundMenuNavigate);
		}

		// OK button
		else if (gMenu_CharacterNaming.SelectedItem == OK_BUTTON)
		{
			gMenu_CharacterNaming.SelectedItem = BACK_BUTTON;
			CursorColumn = 0;
			PlayGameSound(&gSoundMenuNavigate);
		}
	}

	// RIGHT
	if (PRESSED_RIGHT)
	{
		if ((((gMenu_CharacterNaming.SelectedItem + 1) % COLUMN_COUNT) != 0) && gMenu_CharacterNaming.SelectedItem < BACK_BUTTON)		// all the rows above Back and OK
		{
			gMenu_CharacterNaming.SelectedItem += 1;
			CursorColumn += 1;
			PlayGameSound(&gSoundMenuNavigate);
		}

		// back button
		else if (gMenu_CharacterNaming.SelectedItem == BACK_BUTTON)
		{
			gMenu_CharacterNaming.SelectedItem = OK_BUTTON;
			CursorColumn = COLUMN_COUNT - 1;
			PlayGameSound(&gSoundMenuNavigate);
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
	if (strlen(gPlayer.Name) < 8)
	{
		gPlayer.Name[strlen(gPlayer.Name)] = gMenu_CharacterNaming.Items[gMenu_CharacterNaming.SelectedItem]->Name[0];
		PlayGameSound(&gSoundMenuChoose);
	}
}

void MenuItem_CharacterNaming_Back(void)
{
	if (strlen(gPlayer.Name) < 1)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GS_TITLESCREEN;
	}
	else
	{
		gPlayer.Name[strlen(gPlayer.Name) - 1] = '\0';
	}

	PlayGameSound(&gSoundMenuChoose);
}

void MenuItem_CharacterNaming_OK(void)
{
	if (strlen(gPlayer.Name) > 0)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GS_OVERWORLD;
		gPlayer.Active = TRUE;
		PlayGameSound(&gSoundMenuChoose);
	}
}