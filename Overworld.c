#include "Main.h"
#include "Overworld.h"

BOOL fade;

void DrawOverworldScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;

	if (fade == TRUE || g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		fade = FALSE;
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		brightness_adjustment = -255;
		g_input_enabled = FALSE;
	}

	if (local_frame_counter == 10)
	{
		brightness_adjustment = -128;
	}
	if (local_frame_counter == 20)
	{
		brightness_adjustment = -64;
	}
	if (local_frame_counter == 30)
	{
		brightness_adjustment = -32;
	}
	if (local_frame_counter == 40)
	{
		brightness_adjustment = 0;
		g_input_enabled = TRUE;
	}

	if (local_frame_counter == 60)
	{
		if (MusicIsPlaying() == FALSE)
		{
			PlayGameMusic(g_current_area.Music);
		}
	}

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	Blit32BppBitmapToBuffer(&g_player.Sprite[g_player.CurrentArmor][g_player.SpriteIndex + g_player.Direction], 
							(int16_t)g_player.ScreenPos.X, 
							(int16_t)g_player.ScreenPos.Y,
							brightness_adjustment);

	DrawWindow(32, 200, 128, 32, (PIXEL32) { { 0x00, 0x00, 0x00, 0x00 } }, WF_BORDERED | WF_HORIZONTALLY_CENTERED);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_Overworld(void)
{

	if (PRESSED_ESCAPE)
	{
		g_previous_game_state = g_current_game_state;
		g_current_game_state = GS_TITLE_SCREEN;
		PauseMusic();
		return;
	}

	ASSERT((g_camera.X <= g_current_area.Area.right - GAME_RES_WIDTH) && (g_camera.Y <= g_current_area.Area.bottom - GAME_RES_HEIGHT), "Camera is out of bounds!")

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
					ASSERT(g_player.ScreenPos.X % 16 == 0, "Player did not land on a position that is a multiple of 16.")
					ASSERT(g_player.ScreenPos.Y % 16 == 0, "Player did not land on a position that is a multiple of 16.")
					ASSERT(g_player.WorldPos.X  % 16 == 0, "Player did not land on a position that is a multiple of 16.")
					ASSERT(g_player.WorldPos.Y  % 16 == 0, "Player did not land on a position that is a multiple of 16.")

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
						// Random monster encounter?
						DWORD random = 0;
						rand_s((unsigned int*)&random);
						random = random % 100;
						if (random > g_player.RandomEncounterPercentage)
						{
							RandomMonsterEncounter();
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
		if (g_player.WorldPos.X == g_portals[counter]->WorldPos.X && g_player.WorldPos.Y == g_portals[counter]->WorldPos.Y)
		{
			StopMusic();
			g_player.WorldPos = g_portals[counter]->WorldDestination;
			g_player.ScreenPos = g_portals[counter]->ScreenDestination;
			g_camera = g_portals[counter]->CameraPos;
			g_current_area = g_portals[counter]->DestinationArea;
			fade = TRUE;
			return;
		}
	}

	ASSERT(FALSE, "Player is standing on a portal, but we do not have a portal handler for it")
}

void RandomMonsterEncounter(void)
{
	g_previous_game_state = g_current_game_state;
	g_current_game_state = GS_BATTLE;
}