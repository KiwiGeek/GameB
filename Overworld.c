#include "Main.h"
#include "Overworld.h"

BOOL fade;
PORTAL g_portal001 = { 0 };
PORTAL g_portal002 = { 0 };

PORTAL g_portals[2] = { 0 };

GAME_AREA g_current_area = { 0 };
GAME_AREA g_overworld_area = { 0 };
GAME_AREA g_dungeon1_area = { 0 };

void DrawOverworldScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen = 0;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;

	if (fade == TRUE || g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		fade = FALSE;
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		brightness_adjustment = -255;
	}

	ApplyFadeIn(local_frame_counter, COLOR_NES_WHITE, &text_color, &brightness_adjustment);

	if (local_frame_counter == 60)
	{
		if (MusicIsPlaying() == FALSE)
		{
			PlayGameMusic(g_current_area.Music, TRUE, TRUE);
		}
	}

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	Blit32BppBitmapToBuffer(&g_player.Sprite[g_player.CurrentArmor][g_player.SpriteIndex + g_player.Direction],
		(int16_t)g_player.ScreenPos.X,
		(int16_t)g_player.ScreenPos.Y,
		brightness_adjustment);

	//DrawWindow(32, 200, 128, 32, (PIXEL32) { { 0x00, 0x00, 0x00, 0x00 } }, WF_BORDERED | WF_HORIZONTALLY_CENTERED | WF_SHADOWED);

	DrawPlayerStatsWindow(&text_color);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_Overworld(void)
{

	if (PRESSED_ESCAPE)
	{
		ASSERT(g_current_game_state == GS_OVERWORLD, "Invalid game state!")
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_TITLE_SCREEN;
		LogMessageA(LL_INFO, "[%s] Transitioning from game state %d to %d.", __FUNCTION__, g_previous_game_state, g_current_game_state);
		PauseMusic();
		return;
	}

	ASSERT((g_camera.X <= g_current_area.Area.right - GAME_RES_WIDTH) && (g_camera.Y <= g_current_area.Area.bottom - GAME_RES_HEIGHT), "Camera is out of bounds!");  // NOLINT(clang-diagnostic-extra-semi-stmt)

	if (!g_player.MovementRemaining)
	{
		if (g_game_input.DownKeyIsDown)
		{
			// are we at the bottom of the map?
			if (g_player.WorldPos.Y < g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biHeight - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{

					if (g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) + 1][(g_player.WorldPos.X / 16)] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}

				if (g_player.ScreenPos.Y < GAME_RES_HEIGHT - 16 && can_move_to_desired_tile)
				{
					g_player.MovementRemaining = 16;
					g_player.Direction = DOWN;
				}

			}
		}

		else if (g_game_input.LeftKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
			{
				if (g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16)][(g_player.WorldPos.X / 16) - 1] == g_passable_tiles[counter])
				{
					can_move_to_desired_tile = TRUE;
					break;
				}
			}
			if (g_player.ScreenPos.X > 0 && can_move_to_desired_tile)
			{
				g_player.MovementRemaining = 16;
				g_player.Direction = LEFT;
			}
		}

		else if (g_game_input.RightKeyIsDown)
		{
			// are we at the right of the map?
			if (g_player.WorldPos.X < g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biWidth - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{
					if (g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16)][(g_player.WorldPos.X / 16) + 1] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
				if (g_player.ScreenPos.X < GAME_RES_WIDTH - 16 && can_move_to_desired_tile)
				{
					g_player.MovementRemaining = 16;
					g_player.Direction = RIGHT;
				}
			}
		}

		else if (g_game_input.UpKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			if (g_player.ScreenPos.Y > 0)
			{

				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{
					if (g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) - 1][(g_player.WorldPos.X / 16)] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
			}
			if (g_player.ScreenPos.Y > 0 && can_move_to_desired_tile)
			{
				g_player.MovementRemaining = 16;
				g_player.Direction = UP;
			}
		}
	}
	else
	{
		g_player.MovementRemaining--;
		if (g_player.Direction == DOWN)
		{
			if (g_player.ScreenPos.Y < GAME_RES_HEIGHT - 64 || g_camera.Y >= g_current_area.Area.bottom - GAME_RES_HEIGHT)
			{
				g_player.ScreenPos.Y++;
			}
			else
			{
				g_camera.Y++;
			}
			g_player.WorldPos.Y++;
		}
		else if (g_player.Direction == LEFT)
		{
			if (g_player.ScreenPos.X > 64 || g_camera.X == g_current_area.Area.left)
			{
				g_player.ScreenPos.X--;
			}
			else
			{
				g_camera.X--;
			}
			g_player.WorldPos.X--;
		}
		else if (g_player.Direction == RIGHT)
		{
			if (g_player.ScreenPos.X < GAME_RES_WIDTH - 64 || g_camera.X >= g_current_area.Area.right - GAME_RES_WIDTH)
			{
				g_player.ScreenPos.X++;
			}
			else
			{
				g_camera.X++;
			}
			g_player.WorldPos.X++;

		}
		else if (g_player.Direction == UP)
		{
			if (g_player.ScreenPos.Y > 64 || g_camera.Y == g_current_area.Area.top)
			{
				g_player.ScreenPos.Y--;
			}
			else
			{
				g_camera.Y--;
			}
			g_player.WorldPos.Y--;
		}

		switch (g_player.MovementRemaining)
		{
			case 15:
			{
				g_player.HasPlayerMovedSincePortal = TRUE;
				g_player.SpriteIndex = 0;
				break;
			}
			case 11:
			{
				g_player.SpriteIndex = 1;
				break;
			}
			case 7:
			{
				g_player.SpriteIndex = 0;
				break;
			}
			case 3:
			{
				g_player.SpriteIndex = 2;
				break;
			}
			case 0:
			{
				ASSERT(g_player.ScreenPos.X % 16 == 0, "Player did not land on a position that is a multiple of 16.");  // NOLINT(clang-diagnostic-extra-semi-stmt)
				ASSERT(g_player.ScreenPos.Y % 16 == 0, "Player did not land on a position that is a multiple of 16.");  // NOLINT(clang-diagnostic-extra-semi-stmt)
				ASSERT(g_player.WorldPos.X % 16 == 0, "Player did not land on a position that is a multiple of 16.");  // NOLINT(clang-diagnostic-extra-semi-stmt)
				ASSERT(g_player.WorldPos.Y % 16 == 0, "Player did not land on a position that is a multiple of 16.");  // NOLINT(clang-diagnostic-extra-semi-stmt)

				g_player.SpriteIndex = 0;

				// Is the player standing on a portal (or do, or staircase, etc)?
				if (g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][g_player.WorldPos.X / 16] == TILE_PORTAL_01)
				{
					if (g_player.HasPlayerMovedSincePortal == TRUE)
					{
						PortalHandler();
					}
				}
				else
				{
					if (g_player.StepsTaken - g_player.StepsSinceLastRandomMonsterEncounter > RANDOM_MONSTER_GRACE_PERIOD_STEPS)
					{
						// Random monster encounter?
						DWORD random = 0;
						rand_s((unsigned int*)&random);
						random = random % 100;
						if (random > g_player.RandomEncounterPercentage)
						{
							g_player.StepsSinceLastRandomMonsterEncounter = g_player.StepsTaken;
							RandomMonsterEncounter();
						}
					}
				}

				g_player.StepsTaken++;

				break;
			}
			default:
			{

			}
		}
	}
}

void PortalHandler(void)
{
	g_player.HasPlayerMovedSincePortal = FALSE;

	for (uint32_t counter = 0; counter < _countof(g_portals); counter++)
	{
		if (g_player.WorldPos.X == g_portals[counter].WorldPos.X && g_player.WorldPos.Y == g_portals[counter].WorldPos.Y)
		{
			StopMusic();
			g_player.WorldPos = g_portals[counter].WorldDestination;
			g_player.ScreenPos = g_portals[counter].ScreenDestination;
			g_camera = g_portals[counter].CameraPos;
			g_current_area = g_portals[counter].DestinationArea;
			fade = TRUE;
			return;
		}
	}

	ASSERT(FALSE, "Player is standing on a portal, but we do not have a portal handler for it");  // NOLINT(clang-diagnostic-extra-semi-stmt)
}

void RandomMonsterEncounter(void)
{
	ASSERT(g_current_game_state == GS_OVERWORLD, "Invalid game state!");
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_BATTLE;
	LogMessageA(LL_INFO, "[%s] Transitioning from game state %d to %d.", __FUNCTION__, g_previous_game_state, g_current_game_state);
}

