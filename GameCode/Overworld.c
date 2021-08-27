#include "Main.h"

_declspec(dllexport) void RandomMonsterEncounter(_In_ GAME_STATE* PreviousGameState, _Inout_ GAME_STATE* CurrentGameState)
{
	PreviousGameState = CurrentGameState;
	*CurrentGameState = GS_BATTLE;
}