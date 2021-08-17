#include "Main.h"
#include "Overworld.h"

void DrawOverworldScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;
	static int16_t brightness_adjustment = -255;

	if (g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
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
		PlayGameMusic(&g_music_overworld01);
	}

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap, brightness_adjustment);

	Blit32BppBitmapToBuffer(&g_player.Sprite[g_player.CurrentArmor][g_player.SpriteIndex + g_player.Direction], 
							(int16_t)g_player.ScreenPos.X, 
							(int16_t)g_player.ScreenPos.Y,
							brightness_adjustment);

	if (g_performance_data.DisplayDebugInfo)
	{
		char buffer[4] = { 0 };
		// the tile the player is currently on
		_itoa_s(g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][g_player.WorldPos.X / 16], buffer, sizeof(buffer), 10);
		BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5), (int16_t)(g_player.ScreenPos.Y + 4));

		// the tile above the player
		if (g_player.ScreenPos.Y >= 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) - 1][g_player.WorldPos.X / 16], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5), (int16_t)(g_player.ScreenPos.Y + 4 - 16));
		}

		// the tile below the player
		if (g_player.ScreenPos.Y < GAME_RES_HEIGHT - 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) + 1][g_player.WorldPos.X / 16], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5), (int16_t)(g_player.ScreenPos.Y + 4 + 16));
		}

		// the tile to the right of the player
		if (g_player.ScreenPos.X <= GAME_RES_WIDTH - 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][(g_player.WorldPos.X / 16) + 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 + 16), (int16_t)(g_player.ScreenPos.Y + 4));
		}

		// the tile to the left of the player
		if (g_player.ScreenPos.X >= 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][(g_player.WorldPos.X / 16) - 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 - 16), (int16_t)(g_player.ScreenPos.Y + 4));
		}

		// the tile to the upper left of the player
		if (g_player.ScreenPos.X >= 16 && g_player.ScreenPos.Y >= 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) - 1][(g_player.WorldPos.X / 16) - 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 - 16), (int16_t)(g_player.ScreenPos.Y + 4 - 16));
		}

		// the tile to the upper right of the player
		if (g_player.ScreenPos.X <= GAME_RES_WIDTH - 26 && g_player.ScreenPos.Y >= 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) - 1][(g_player.WorldPos.X / 16) + 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 + 16), (int16_t)(g_player.ScreenPos.Y + 4 - 16));
		}

		// the tile to the bottom left of the player
		if (g_player.ScreenPos.X >= 16 && g_player.ScreenPos.Y < GAME_RES_HEIGHT - 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) + 1][(g_player.WorldPos.X / 16) - 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 - 16), (int16_t)(g_player.ScreenPos.Y + 4 + 16));
		}

		// the tile to the bottom right of the player
		if (g_player.ScreenPos.X <= GAME_RES_WIDTH - 16 && g_player.ScreenPos.Y < GAME_RES_HEIGHT - 16)
		{
			_itoa_s(g_overworld01.TileMap.Map[(g_player.WorldPos.Y / 16) + 1][(g_player.WorldPos.X / 16) + 1], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g_6x7_font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (int16_t)(g_player.ScreenPos.X + 5 + 16), (int16_t)(g_player.ScreenPos.Y + 4 + 16));
		}
	}

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_Overworld(void)
{

	// TODO remove this - it is just for debugging
	if (PRESSED_ESCAPE)
	{
		SendMessageA(g_game_window, WM_CLOSE, 0, 0);
	}

	ASSERT((g_camera.X <= g_current_area.right - GAME_RES_WIDTH) && (g_camera.Y <= g_current_area.bottom - GAME_RES_HEIGHT), "Camera is out of bounds!")

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
				if (g_player.ScreenPos.Y < GAME_RES_HEIGHT - 64 || g_camera.Y >= g_current_area.bottom - GAME_RES_HEIGHT)
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
				if (g_player.ScreenPos.X > 64 || g_camera.X == g_current_area.left)
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
				if (g_player.ScreenPos.X < GAME_RES_WIDTH - 64 || g_camera.X >= g_current_area.right - GAME_RES_WIDTH)
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
				if (g_player.ScreenPos.Y > 64 || g_camera.Y == g_current_area.top)
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
					ASSERT(g_player.WorldPos.X % 16 == 0, "Player did not land on a position that is a multiple of 16.")
					ASSERT(g_player.WorldPos.Y % 16 == 0, "Player did not land on a position that is a multiple of 16.")

					g_player.SpriteIndex = 0;

					// Is the player standing on a portal (or do, or staircase, etc)?
					if (g_overworld01.TileMap.Map[g_player.WorldPos.Y / 16][g_player.WorldPos.X / 16] == TILE_PORTAL_01)
					{
						if (g_player.HasPlayerMovedSincePortal == TRUE)
						{
							PortalHandler();
						}
					}

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
			g_player.WorldPos = g_portals[counter]->WorldDestination;
			g_player.ScreenPos = g_portals[counter]->ScreenDestination;
			g_camera = g_portals[counter]->CameraPos;
			g_current_area = g_portals[counter]->DestinationArea;
			return;
		}
	}

	ASSERT(FALSE, "Player is standing on a portal, but we do not have a portal handler for it")
}
