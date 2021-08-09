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

	LocalFrameCounter++;

	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}



void PPI_Overworld(void)
{

	if (!gPlayer.MovementRemaining)
	{
		if (gGameInput.DownKeyIsDown)
		{
			if (gPlayer.ScreenPos.Y < GAME_RES_HEIGHT - 16)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = DOWN;
			}
		}

		else if (gGameInput.LeftKeyIsDown)
		{
			if (gPlayer.ScreenPos.X > 0)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = LEFT;
			}
		}

		else if (gGameInput.RightKeyIsDown)
		{
			if (gPlayer.ScreenPos.X < GAME_RES_WIDTH - 16)
			{
				gPlayer.MovementRemaining = 16;
				gPlayer.Direction = RIGHT;
			}
		}

		else if (gGameInput.UpKeyIsDown)
		{
			if (gPlayer.ScreenPos.Y > 0)
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
		}
		else if (gPlayer.Direction == LEFT)
		{
			gPlayer.ScreenPos.X--;
		}
		else if (gPlayer.Direction == RIGHT)
		{
			gPlayer.ScreenPos.X++;
		}
		else if (gPlayer.Direction == UP)
		{
			gPlayer.ScreenPos.Y--;
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