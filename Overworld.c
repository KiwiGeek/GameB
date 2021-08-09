#include "Main.h"
#include "Overworld.h"

void DrawOverworldScreen(void)
{
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	static PIXEL32 TextColor;

	if (gPerformanceData.TotalFramesRendered > (LastFrameSeen + 1))
	{
		LocalFrameCounter = 0;
		memset(&TextColor, 0, sizeof(PIXEL32));
	}

	BlitTilemapToBuffer(&gOverworld01.GameBitmap);

	Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentArmor][gPlayer.SpriteIndex + gPlayer.Direction], gPlayer.ScreenPos.X, gPlayer.ScreenPos.Y);

	for(uint16_t Row = 0; Row < GAME_RES_HEIGHT / 16; Row++) 
	{
		for (uint16_t Column = 0; Column < GAME_RES_WIDTH / 16; Column++)
		{
			char Buffer[8] = { 0 };
			_itoa_s(gOverworld01.TileMap.Map[Row][Column], Buffer, sizeof(Buffer), 10);
			BlitStringToBuffer(Buffer, &g6x7Font, &(PIXEL32) { 0xFF, 0xFF, 0xFF, 0xFF }, (Column * 16)+5, (Row * 16)+5);
		}
	}

	LocalFrameCounter++;

	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}



void PPI_Overworld(void)
{

	if (!gPlayer.MovementRemaining)
	{
		if (gGameInput.DownKeyIsDown)
		{
			BOOL CanMoveToDesiredTile = FALSE;
			for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
			{
				if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16) + 1][(gPlayer.WorldPos.X / 16)] == gPassableTiles[Counter])
				{
					CanMoveToDesiredTile = TRUE;
					break;
				}
			}
			if (gPlayer.ScreenPos.Y < GAME_RES_HEIGHT - 16 && CanMoveToDesiredTile)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = DOWN;
			}
		}

		else if (gGameInput.LeftKeyIsDown)
		{
			BOOL CanMoveToDesiredTile = FALSE;
			for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
			{
				if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16)][(gPlayer.WorldPos.X / 16) - 1] == gPassableTiles[Counter])
				{
					CanMoveToDesiredTile = TRUE;
					break;
				}
			}
			if (gPlayer.ScreenPos.X > 0 && CanMoveToDesiredTile)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = LEFT;
			}
		}

		else if (gGameInput.RightKeyIsDown)
		{
			BOOL CanMoveToDesiredTile = FALSE;
			for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
			{
				if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16)][(gPlayer.WorldPos.X / 16) + 1] == gPassableTiles[Counter])
				{
					CanMoveToDesiredTile = TRUE;
					break;
				}
			}
			if (gPlayer.ScreenPos.X < GAME_RES_WIDTH - 16 && CanMoveToDesiredTile)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = RIGHT;
			}
		}

		else if (gGameInput.UpKeyIsDown)
		{
			BOOL CanMoveToDesiredTile = FALSE;
			if (gPlayer.ScreenPos.Y > 0)
			{

				for (uint8_t Counter = 0; Counter < _countof(gPassableTiles); Counter++)
				{
					if (gOverworld01.TileMap.Map[(gPlayer.WorldPos.Y / 16) - 1][(gPlayer.WorldPos.X / 16)] == gPassableTiles[Counter])
					{
						CanMoveToDesiredTile = TRUE;
						break;
					}
				}
			}
			if (gPlayer.ScreenPos.Y > 0 && CanMoveToDesiredTile)
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
			gPlayer.ScreenPos.Y++;
			gPlayer.WorldPos.Y++;
		}
		else if (gPlayer.Direction == LEFT)
		{
			gPlayer.ScreenPos.X--;
			gPlayer.WorldPos.X--;
		}
		else if (gPlayer.Direction == RIGHT)
		{
			gPlayer.ScreenPos.X++;
			gPlayer.WorldPos.X++;
		}
		else if (gPlayer.Direction == UP)
		{
			gPlayer.ScreenPos.Y--;
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

	/*if (PRESSED_ESCAPE)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GS_TITLESCREEN;
	}*/

}