#pragma once

typedef struct GAME_AREA
{
	char* Name;
	RECT Area;
	GAME_SOUND Music;
} GAME_AREA;

typedef struct PORTAL
{
	UPOINT WorldPos;			// Where is this portal located, in world coordinates
	UPOINT WorldDestination;	// Where should the portal take the player, in world coordinates
	UPOINT ScreenDestination;	// Where should the player be located, in screen coordinates
	UPOINT	CameraPos;			// Where should the camera be located, in world coordinates
	RECT DestinationArea;		// Which area does the portal take you to
} PORTAL;

PORTAL g_portal001;
PORTAL g_portal002;

PORTAL* g_portals[2];

RECT g_current_area;
RECT g_overworld_area;
RECT g_dungeon1_area;

void DrawOverworldScreen(void);
void PPI_Overworld(void);
void PortalHandler(void);
void RandomMonsterEncounter(void);