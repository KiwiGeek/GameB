#pragma once

#include <stdint.h>

typedef struct MENUITEM
{
	char* Name;
	int16_t X;
	int16_t Y;
	BOOL Enabled;
	void (*Action)(void);
} MENUITEM;

typedef struct MENU
{
	char* Name;
	uint8_t SelectedItem;
	uint8_t ItemCount;
	MENUITEM** Items;
} MENU;

void MenuItem_TitleScreen_Resume(void);
void MenuItem_TitleScreen_StartNew(void);
void MenuItem_TitleScreen_Options(void);
void MenuItem_TitleScreen_Exit(void);
void MenuItem_ExitYesNo_Yes(void);
void MenuItem_ExitYesNo_No(void);
void MenuItem_OptionsScreen_SFXVolume(void);
void MenuItem_OptionsScreen_MusicVolume(void);
void MenuItem_OptionsScreen_ScreenSize(void);
void MenuItem_OptionsScreen_Back(void);
void MenuItem_CharacterNaming_Add(void);
void MenuItem_CharacterNaming_Back(void);
void MenuItem_CharacterNaming_OK(void);

// Title screen

MENUITEM gMI_ResumeGame = { "Resume", (GAME_RES_WIDTH / 2) - ((6 * 6) / 2), 100, FALSE, MenuItem_TitleScreen_Resume };
MENUITEM gMI_StartNewGame = { "Start New Game", (GAME_RES_WIDTH / 2) - ((14 * 6) / 2), 115, TRUE, MenuItem_TitleScreen_StartNew };
MENUITEM gMI_Options = { "Options", (GAME_RES_WIDTH / 2) - ((7 * 6) / 2), 130, TRUE, MenuItem_TitleScreen_Options };
MENUITEM gMI_Exit = { "Exit", (GAME_RES_WIDTH / 2) - ((4 * 6) / 2), 145, TRUE, MenuItem_TitleScreen_Exit };
MENUITEM* gMI_TitleScreenItems[] = { &gMI_ResumeGame, &gMI_StartNewGame, &gMI_Options, &gMI_Exit };
MENU gMenu_TitleScreen = { "Title Screen Menu", 1, _countof(gMI_TitleScreenItems), gMI_TitleScreenItems };


// Exit Yes or No Screen

MENUITEM gMI_ExitYesNo_Yes = { "Yes", (GAME_RES_WIDTH / 2) - ((3 * 6) / 2), 100, TRUE, MenuItem_ExitYesNo_Yes };
MENUITEM gMI_ExitYesNo_No = { "No", (GAME_RES_WIDTH / 2) - ((2 * 6) / 2), 115, TRUE, MenuItem_ExitYesNo_No };
MENUITEM* gMI_ExitYesNoItems[] = { &gMI_ExitYesNo_Yes , &gMI_ExitYesNo_No };
MENU gMenu_ExitYesNo = { "Are you sure you want to exit?", 1, _countof(gMI_ExitYesNoItems), gMI_ExitYesNoItems };


// Options screen

MENUITEM gMI_OptionsScreen_SFXVolume = { "SFX Volume:", (GAME_RES_WIDTH / 2) - ((11 * 6) / 2) - 16, 100, TRUE, MenuItem_OptionsScreen_SFXVolume };
MENUITEM gMI_OptionsScreen_MusicVolume = { "Music Volume:", (GAME_RES_WIDTH / 2) - ((13 * 6) / 2) - 16, 115, TRUE, MenuItem_OptionsScreen_MusicVolume };
MENUITEM gMI_OptionsScreen_ScreenSize = { "Screen Size:", (GAME_RES_WIDTH / 2) - ((12 * 6) / 2) - 16, 130, TRUE, MenuItem_OptionsScreen_ScreenSize };
MENUITEM gMI_OptionsScreen_Back = { "Back", (GAME_RES_WIDTH / 2) - ((4 * 6) / 2) - 16, 145, TRUE, MenuItem_OptionsScreen_Back };
MENUITEM* gMI_OptionsScreenItems[] = { &gMI_OptionsScreen_SFXVolume, &gMI_OptionsScreen_MusicVolume, &gMI_OptionsScreen_ScreenSize, &gMI_OptionsScreen_Back };
MENU gMenu_OptionsScreen = { "Options", 0, _countof(gMI_OptionsScreenItems), gMI_OptionsScreenItems };


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