#pragma once

typedef struct PORTAL
{
	UPOINT WorldPos;			// Where is this portal located, in world coordinates
	UPOINT WorldDestination;	// Where should the portal take the player, in world coordinates
	UPOINT ScreenDestination;	// Where should the player be located, in screen coordinates
	UPOINT	CameraPos;			// Where should the camera be located, in world coordinates
	GAME_AREA DestinationArea;	// Which area does the portal take you to
} PORTAL;

extern PORTAL g_portal001;
extern PORTAL g_portal002;

extern PORTAL* g_portals[2];

extern GAME_AREA g_current_area;
extern GAME_AREA g_overworld_area;
extern GAME_AREA g_dungeon1_area;

void DrawOverworldScreen(void);
void PPI_Overworld(void);
void PortalHandler(void);
void RandomMonsterEncounter(void);