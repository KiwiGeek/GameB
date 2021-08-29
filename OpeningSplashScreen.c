#include "Main.h"
#include "OpeningSplashScreen.h"

void DrawOpeningSplashScreen(void)
{
	static uint64_t local_frame_counter;
	static uint64_t last_frame_seen;
	static PIXEL32 text_color = {{0xFF, 0xFF, 0xFF, 0xFF}};
	static BOOL blink;

	if (WaitForSingleObject(g_essential_assets_loaded_event, 0) != WAIT_OBJECT_0)
	{
		// make sure it hasn't been more than 5 seconds since we started loading. Something would be wrong
		if (g_performance_data.TotalFramesRendered > 300)
		{
			LogMessageA(LL_ERROR, "[%s] Essential assets not loaded yet after 5 seconds. Unable to continue!", __FUNCTION__);

			g_game_is_running = FALSE;

			MessageBoxA(g_game_window, "Essential assets not loaded yet after 5 seconds. Unable to continue!", "Error", MB_OK | MB_ICONERROR);
		}
		return;
	}

	if (g_performance_data.TotalFramesRendered > last_frame_seen + 1)
	{
		local_frame_counter = 0;
		memset(&text_color, 0, sizeof(PIXEL32));
		g_input_enabled = FALSE;
	}

	if (g_performance_data.TotalFramesRendered % 30 == 0)
	{
		blink = !blink;
	}

	memset(g_back_buffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	if (WaitForSingleObject(g_asset_loading_thread_handle, 0) != WAIT_OBJECT_0)
	{
		BlitStringToBuffer("Loading...", &g_6x7_font, &(PIXEL32){{32, 32, 32, 255}}, (GAME_RES_WIDTH - (6 * 11)), (GAME_RES_HEIGHT - 7));

		if (blink)
		{
			BlitStringToBuffer("\xf2", &g_6x7_font, &(PIXEL32) {{32, 32, 32, 255}}, GAME_RES_WIDTH - 6, GAME_RES_HEIGHT - 7);
		}
	}
	else
	{
		g_input_enabled = TRUE;
	}


	if (local_frame_counter >= 120)
	{
		if (local_frame_counter == 120)
		{
			PlayGameSound(&g_sound_splash_screen);
		}

		if ((local_frame_counter >= 180) && (local_frame_counter <= 210) && (local_frame_counter % 15 == 0))
		{
			text_color.colors.Red -= 64;
			text_color.colors.Green -= 64;
			text_color.colors.Blue -= 64;
		}
		if (local_frame_counter == 225)
		{
			text_color.colors.Red = 0;
			text_color.colors.Green = 0;
			text_color.colors.Blue = 0;
		}

		if (local_frame_counter >= 240)
		{
			if (WaitForSingleObject(g_asset_loading_thread_handle, 0) == WAIT_OBJECT_0)
			{
				DWORD thread_exit_code = ERROR_SUCCESS;
				GetExitCodeThread(g_asset_loading_thread_handle, &thread_exit_code);
				if (thread_exit_code != ERROR_SUCCESS)
				{
					LogMessageA(LL_ERROR, "[%s] Asset Loading Thread failed with 0x%08lx!", __FUNCTION__, thread_exit_code);
					g_game_is_running = FALSE;
					MessageBoxA(g_game_window, "Asset loading failed! Check log for more details.", "Error", MB_OK | MB_ICONERROR);
				}

				g_previous_game_state = g_current_game_state;
				g_current_game_state = GS_TITLE_SCREEN;
			}
		}

		BlitStringToBuffer("-Game Studio-",
			&g_6x7_font,
			&text_color,
			(GAME_RES_WIDTH / 2) - (int16_t)(13 * 6 / 2),
			100);
		BlitStringToBuffer("Presents",
			&g_6x7_font,
			&text_color,
			(GAME_RES_WIDTH / 2) - (int16_t)(8 * 6 / 2),
			115);
	}

	local_frame_counter++;
	last_frame_seen = g_performance_data.TotalFramesRendered;
}

void PPI_OpeningSplashScreen(void)
{
	if (PRESSED_ESCAPE)
	{
		if (WaitForSingleObject(g_asset_loading_thread_handle, 0) == WAIT_OBJECT_0)
		{
			DWORD thread_exit_code = ERROR_SUCCESS;
			GetExitCodeThread(g_asset_loading_thread_handle, &thread_exit_code);
			if (thread_exit_code == ERROR_SUCCESS)
			{
				g_previous_game_state = g_current_game_state;
				g_current_game_state = GS_TITLE_SCREEN;
			}

		}
	}
}
