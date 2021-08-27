#pragma once

#ifdef _WIN32
#pragma warning(disable: 4668)
#pragma warning(push, 0)
// ReSharper disable once CppUnusedIncludeDirective
#include <Windows.h>
#pragma warning(pop)
#endif

typedef enum GAME_STATE
{
	GS_OPENING_SPLASH_SCREEN,
	GS_TITLE_SCREEN,
	GS_CHARACTER_NAMING,
	GS_OVERWORLD,
	GS_BATTLE,
	GS_OPTIONS_SCREEN,
	GS_EXIT_YES_NO_SCREEN,
	GS_GAMEPAD_UNPLUGGED,
	GS_NEW_GAME_ARE_YOU_SURE
} GAME_STATE;