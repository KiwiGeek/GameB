#include "Main.h"
#include "OpeningSplashScreen.h"

void DrawOpeningSplashScreen(void)
{
	static uint64_t LocalFrameCounter;
	static uint64_t LastFrameSeen;
	static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

	if (gPerformanceData.TotalFramesRendered > LastFrameSeen + 1)
	{
		LocalFrameCounter = 0;
	}

	memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	if (LocalFrameCounter >= 120)
	{
		if (LocalFrameCounter == 120)
		{
			PlayGameSound(&gSoundSplashScreen);
		}

		if ((LocalFrameCounter >= 180) && (LocalFrameCounter <= 210) && (LocalFrameCounter % 15 == 0))
		{
			TextColor.Red -= 64;
			TextColor.Green -= 64;
			TextColor.Blue -= 64;
		}
		if (LocalFrameCounter == 225)
		{
			TextColor.Red = 0;
			TextColor.Green = 0;
			TextColor.Blue = 0;
		}

		if (LocalFrameCounter >= 240)
		{
			if (WaitForSingleObject(gAssetLoadingThreadHandle, 0) == WAIT_OBJECT_0)
			{
				gPreviousGameState = gCurrentGameState;
				gCurrentGameState = GS_TITLESCREEN;			
			}
			else
			{
				// TODO: Draw loading text.
			}
		}

		BlitStringToBuffer("-Game Studio-",
			&g6x7Font,
			&TextColor,
			(GAME_RES_WIDTH / 2) - (int16_t)(13 * 6 / 2),
			100);
		BlitStringToBuffer("Presents",
			&g6x7Font,
			&TextColor,
			(GAME_RES_WIDTH / 2) - (int16_t)(8 * 6 / 2),
			115);
	}

	LocalFrameCounter++;
	LastFrameSeen = gPerformanceData.TotalFramesRendered;
}

void PPI_OpeningSplashScreen(void)
{
	if (PRESSED_ESCAPE)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GS_TITLESCREEN;
	}
}
