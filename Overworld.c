#include "Main.h"
#include "Overworld.h"

void DrawOverworldScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;

	if (g_performance_data.TotalFramesRendered > (last_frame_seen + 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
	}

	if (local_frame_counter == 60)
	{
		PlayGameMusic(&g_music_overworld01);
	}

	BlitBackgroundToBuffer(&g_overworld01.GameBitmap);

	/*for (uint16_t row = 0; row < GAME_RES_HEIGHT / 16; row++)
	{
		for (uint16_t column = 0; column < GAME_RES_WIDTH / 16; column++)
		{
			char buffer[8] = { 0 };
			_itoa_s(gOverworld01.TileMap.Map[row + (gCamera.Y / 16)][column + (gCamera.X / 16)], buffer, sizeof(buffer), 10);
			BlitStringToBuffer(buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (column * 16) + 5, (row * 16) + 4);
		}
	}*/

	Blit32BppBitmapToBuffer(&g_Player.Sprite[g_Player.CurrentArmor][g_Player.SpriteIndex + g_Player.Direction], g_Player.ScreenPos.X, g_Player.ScreenPos.Y);

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_Overworld(void)
{

	if (!g_Player.MovementRemaining)
	{
		if (g_game_input.DownKeyIsDown)
		{
			// are we at the bottom of the map?
			if (g_Player.WorldPos.Y < g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biHeight - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{

					if (g_overworld01.TileMap.Map[(g_Player.WorldPos.Y / 16) + 1][(g_Player.WorldPos.X / 16)] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}

				if (g_Player.ScreenPos.Y < GAME_RES_HEIGHT - 16 && can_move_to_desired_tile)
				{
					g_Player.MovementRemaining = 16;
					g_Player.Direction = DOWN;
				}

			}
		}

		else if (g_game_input.LeftKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
			{
				if (g_overworld01.TileMap.Map[(g_Player.WorldPos.Y / 16)][(g_Player.WorldPos.X / 16) - 1] == g_passable_tiles[counter])
				{
					can_move_to_desired_tile = TRUE;
					break;
				}
			}
			if (g_Player.ScreenPos.X > 0 && can_move_to_desired_tile)
			{
				g_Player.MovementRemaining = 16;
				g_Player.Direction = LEFT;
			}
		}

		else if (g_game_input.RightKeyIsDown)
		{
			// are we at the right of the map?
			if (g_Player.WorldPos.X < g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biWidth - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{
					if (g_overworld01.TileMap.Map[(g_Player.WorldPos.Y / 16)][(g_Player.WorldPos.X / 16) + 1] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
				if (g_Player.ScreenPos.X < GAME_RES_WIDTH - 16 && can_move_to_desired_tile)
				{
					g_Player.MovementRemaining = 16;
					g_Player.Direction = RIGHT;
				}
			}
		}

		else if (g_game_input.UpKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			if (g_Player.ScreenPos.Y > 0)
			{

				for (uint8_t counter = 0; counter < (uint8_t)_countof(g_passable_tiles); counter++)
				{
					if (g_overworld01.TileMap.Map[(g_Player.WorldPos.Y / 16) - 1][(g_Player.WorldPos.X / 16)] == g_passable_tiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
			}
			if (g_Player.ScreenPos.Y > 0 && can_move_to_desired_tile)
			{
				g_Player.MovementRemaining = 16;
				g_Player.Direction = UP;
			}
		}
	}
	else
	{
		g_Player.MovementRemaining--;
		if (g_Player.Direction == DOWN)
		{
			if (g_Player.ScreenPos.Y < (GAME_RES_HEIGHT - 64) || g_camera.Y == g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biHeight - GAME_RES_HEIGHT)
			{
				g_Player.ScreenPos.Y++;
			}
			else
			{
				g_camera.Y++;
			}
			g_Player.WorldPos.Y++;
		}
		else if (g_Player.Direction == LEFT)
		{
			if (g_Player.ScreenPos.X > 64 || g_camera.X == 0)
			{
				g_Player.ScreenPos.X--;
			}
			else
			{
				g_camera.X--;
			}
			g_Player.WorldPos.X--;
		}
		else if (g_Player.Direction == RIGHT)
		{
			if (g_Player.ScreenPos.X < GAME_RES_WIDTH - 64 || g_camera.X == g_overworld01.GameBitmap.BitmapInfo.bmiHeader.biWidth - GAME_RES_WIDTH)
			{
				g_Player.ScreenPos.X++;
			}
			else
			{
				g_camera.X++;
			}
			g_Player.WorldPos.X++;

		}
		else if (g_Player.Direction == UP)
		{
			if (g_Player.ScreenPos.Y > 64 || g_camera.Y == 0)
			{
				g_Player.ScreenPos.Y--;
			}
			else
			{
				g_camera.Y--;
			}
			g_Player.WorldPos.Y--;
		}

		switch (g_Player.MovementRemaining)
		{
			case 16:
			{
				g_Player.SpriteIndex = 0;
				break;
			}
			case 12:
			{
				g_Player.SpriteIndex = 1;
				break;
			}
			case 8:
			{
				g_Player.SpriteIndex = 0;
				break;
			}
			case 4:
			{
				g_Player.SpriteIndex = 2;
				break;
			}
			case 0:
			{
				g_Player.SpriteIndex = 0;
				break;
			}
			default:
			{

			}
		}
	}

	// TODO remove this - it is just for debugging
	if (PRESSED_ESCAPE)
	{
		SendMessageA(g_game_window, WM_CLOSE, 0, 0);
	}

}
