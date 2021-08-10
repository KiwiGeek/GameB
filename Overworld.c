#include "Main.h"
#include "Overworld.h"

void DrawOverworldScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color;

	if (gPerformanceData.TotalFramesRendered > (last_frame_seen + 1))
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
	}

	if (local_frame_counter == 60)
	{
		PlayGameMusic(&gMusicOverworld01);
	}

	BlitBackgroundToBuffer(&gOverworld01.GameBitmap);

	/*for (uint16_t Row = 0; Row < GAME_RES_HEIGHT / 16; Row++)
	{
		for (uint16_t Column = 0; Column < GAME_RES_WIDTH / 16; Column++)
		{
			char Buffer[8] = { 0 };
			_itoa_s(gOverworld01.TileMap.Map[Row + (gCamera.Y / 16)][Column + (gCamera.X / 16)], Buffer, sizeof(Buffer), 10);
			BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (Column * 16) + 5, (Row * 16) + 4);
		}
	}*/

	Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentArmor][gPlayer.SpriteIndex + gPlayer.Direction], gPlayer.ScreenPos.X, gPlayer.ScreenPos.Y);

	local_frame_counter++;
	last_frame_seen = gPerformanceData.TotalFramesRendered;
}

void PPI_Overworld(void)
{

	if (!gPlayer.MovementRemaining)
	{
		if (gGameInput.DownKeyIsDown)
		{
			// are we at the bottom of the map?
			if (gPlayer.WorldPos.Y < gOverworld01.GameBitmap.BitmapInfo.bmiHeader.biHeight - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(gPassableTiles); counter++)
				{

					if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16) + 1][(gPlayer.WorldPos.X / 16)] == gPassableTiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}

				if (gPlayer.ScreenPos.Y < GAME_RES_HEIGHT - 16 && can_move_to_desired_tile)
				{
					gPlayer.MovementRemaining = 16;
					gPlayer.Direction = DOWN;
				}

			}
		}

		else if (gGameInput.LeftKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			for (uint8_t counter = 0; counter < (uint8_t)_countof(gPassableTiles); counter++)
			{
				if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16)][(gPlayer.WorldPos.X / 16) - 1] == gPassableTiles[counter])
				{
					can_move_to_desired_tile = TRUE;
					break;
				}
			}
			if (gPlayer.ScreenPos.X > 0 && can_move_to_desired_tile)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = LEFT;
			}
		}

		else if (gGameInput.RightKeyIsDown)
		{
			// are we at the right of the map?
			if (gPlayer.WorldPos.X < gOverworld01.GameBitmap.BitmapInfo.bmiHeader.biWidth - 16)
			{
				BOOL can_move_to_desired_tile = FALSE;
				for (uint8_t counter = 0; counter < (uint8_t)_countof(gPassableTiles); counter++)
				{
					if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16)][(gPlayer.WorldPos.X / 16) + 1] == gPassableTiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
				if (gPlayer.ScreenPos.X < GAME_RES_WIDTH - 16 && can_move_to_desired_tile)
				{
					gPlayer.MovementRemaining = 16;
					gPlayer.Direction = RIGHT;
				}
			}
		}

		else if (gGameInput.UpKeyIsDown)
		{
			BOOL can_move_to_desired_tile = FALSE;
			if (gPlayer.ScreenPos.Y > 0)
			{

				for (uint8_t counter = 0; counter < (uint8_t)_countof(gPassableTiles); counter++)
				{
					if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16) - 1][(gPlayer.WorldPos.X / 16)] == gPassableTiles[counter])
					{
						can_move_to_desired_tile = TRUE;
						break;
					}
				}
			}
			if (gPlayer.ScreenPos.Y > 0 && can_move_to_desired_tile)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = UP;
			}
		}
	}
	else
	{
		gPlayer.MovementRemaining--;
		if (gPlayer.Direction == DOWN)
		{
			if (gPlayer.ScreenPos.Y < (GAME_RES_HEIGHT - 64) || gCamera.Y == gOverworld01.GameBitmap.BitmapInfo.bmiHeader.biHeight - GAME_RES_HEIGHT)
			{
				gPlayer.ScreenPos.Y++;
			}
			else
			{
				gCamera.Y++;
			}
			gPlayer.WorldPos.Y++;
		}
		else if (gPlayer.Direction == LEFT)
		{
			if (gPlayer.ScreenPos.X > 64 || gCamera.X == 0)
			{
				gPlayer.ScreenPos.X--;
			}
			else
			{
				gCamera.X--;
			}
			gPlayer.WorldPos.X--;
		}
		else if (gPlayer.Direction == RIGHT)
		{
			if (gPlayer.ScreenPos.X < GAME_RES_WIDTH - 64 || gCamera.X == gOverworld01.GameBitmap.BitmapInfo.bmiHeader.biWidth - GAME_RES_WIDTH)
			{
				gPlayer.ScreenPos.X++;
			}
			else
			{
				gCamera.X++;
			}
			gPlayer.WorldPos.X++;

		}
		else if (gPlayer.Direction == UP)
		{
			if (gPlayer.ScreenPos.Y > 64 || gCamera.Y == 0)
			{
				gPlayer.ScreenPos.Y--;
			}
			else
			{
				gCamera.Y--;
			}
			gPlayer.WorldPos.Y--;
		}

		switch (gPlayer.MovementRemaining)
		{
			case 16:
			{
				gPlayer.SpriteIndex = 0;
				break;
			}
			case 12:
			{
				gPlayer.SpriteIndex = 1;
				break;
			}
			case 8:
			{
				gPlayer.SpriteIndex = 0;
				break;
			}
			case 4:
			{
				gPlayer.SpriteIndex = 2;
				break;
			}
			case 0:
			{
				gPlayer.SpriteIndex = 0;
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
		SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
	}

}
