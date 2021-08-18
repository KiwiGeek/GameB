#include "Main.h"
#include "CharacterNamingScreen.h"
#include "ExitYesNoScreen.h"
#include "GamepadUnplugged.h"
#include "OpeningSplashScreen.h"
#include "OptionsScreen.h"
#include "Overworld.h"
#include "TitleScreen.h"
#include "stb_vorbis.h"
#include "miniz.h"

int g_font_character_pixel_offset[] = {
	/*      .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..*/
	/*    */93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	/*          !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?*/
	/*    */94,64,87,66,67,68,70,85,72,73,71,77,88,74,91,92,52,53,54,55,56,57,58,59,60,61,86,84,89,75,90,93,
	/*       @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _*/
	/*    */65,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,80,78,81,69,76,
	/*       `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~ ..*/
	/*    */62,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,82,79,83,63,93,
	/*      .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..*/
	/*    */93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	/*      .. .. .. .. .. .. .. .. .. .. ..  « .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..  » .. .. .. ..*/
	/*    */93,93,93,93,93,93,93,93,93,93,93,96,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,95,93,93,93,93,
	/*      .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..*/
	/*    */93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	/*      .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. F2 .. .. .. .. .. .. .. .. .. .. .. .. ..*/
	/*    */93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,97,93,93,93,93,93,93,93,93,93,93,93,93,93
};

CRITICAL_SECTION g_log_critical_section;
BOOL g_window_has_focus;
REGISTRY_PARAMS g_registry_params;
XINPUT_STATE g_gamepad_state;
IXAudio2* g_xaudio;
IXAudio2MasteringVoice* g_xaudio_mastering_voice;
uint8_t g_sfx_source_voice_selector;

int WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PreviousInstance, _In_ PSTR CommandLine, _In_ INT CmdShow)  // NOLINT(clang-diagnostic-language-extension-token)
{
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

	MSG message = { 0 };
	int64_t frame_start = 0;
	int64_t frame_end = 0;
	uint64_t elapsed_microseconds_accumulator_raw = 0;
	uint64_t elapsed_microseconds_accumulator_cooked = 0;
	FILETIME process_creation_time = { 0 };
	FILETIME process_exit_time = { 0 };
	int64_t current_user_cpu_time = 0;
	int64_t current_kernel_cpu_time = 0;
	int64_t previous_user_cpu_time = 0;
	int64_t previous_kernel_cpu_time = 0;

#pragma warning(suppress: 6031)
	InitializeCriticalSectionAndSpinCount(&g_log_critical_section, 0x400);
	if ((g_essential_assets_loaded_event = CreateEventA(NULL, TRUE, FALSE, "g_essential_assets_loaded_event")) == NULL)
	{
		goto Exit;
	}

	InitializeGlobals();

	if (LoadRegistryParameters() != ERROR_SUCCESS)
	{
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] %s %s is starting.", __FUNCTION__, GAME_NAME, GAME_VER);

	if (GameIsAlreadyRunning())
	{
		LogMessageA(LL_ERROR, "[%s] Another instance of this program is already running!", __FUNCTION__);
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if ((nt_query_timer_resolution = (NtQueryTimerResolution)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryTimerResolution")) == NULL)
	{
		LogMessageA(LL_ERROR, "[%s] Couldn't find the NtQueryTimerResolution function in ntdll.dll! GetProcAddress failed! Error 0x%081x!", __FUNCTION__, GetLastError());
		MessageBoxA(NULL, "Couldn't find the NtQueryTimerResolution function in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	nt_query_timer_resolution(&g_performance_data.MinimumTimerResolution, &g_performance_data.MaximumTimerResolution, &g_performance_data.CurrentTimerResolution);
	GetSystemInfo(&g_performance_data.SystemInfo);
	LogMessageA(LL_INFO, "[%s] Number of CPUs: %d", __FUNCTION__, g_performance_data.SystemInfo.dwNumberOfProcessors);
	switch (g_performance_data.SystemInfo.wProcessorArchitecture)
	{
		case PROCESSOR_ARCHITECTURE_INTEL:
		{
			LogMessageA(LL_INFO, "[%s] CPU Architecture: x86", __FUNCTION__);
			break;
		}
		case PROCESSOR_ARCHITECTURE_IA64:
		{
			LogMessageA(LL_INFO, "[%s] CPU Architecture: Itanium", __FUNCTION__);
			break;
		}
		case PROCESSOR_ARCHITECTURE_ARM64:
		{
			LogMessageA(LL_INFO, "[%s] CPU Architecture: ARM64", __FUNCTION__);
			break;
		}
		case PROCESSOR_ARCHITECTURE_ARM:
		{
			LogMessageA(LL_INFO, "[%s] CPU Architecture: ARM", __FUNCTION__);
			break;
		}
		case PROCESSOR_ARCHITECTURE_AMD64:
		{
			LogMessageA(LL_INFO, "[%s] CPU Architecture: x64", __FUNCTION__);
			break;
		}
		default:
		{
			LogMessageA(LL_WARNING, "[%s] CPU Architecture: Unknown", __FUNCTION__);
		}
	}

	GetSystemTimeAsFileTime((LPFILETIME)&g_performance_data.PreviousSystemTime);

	if (timeBeginPeriod(1) == TIMERR_NOCANDO)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set global timer resolution!", __FUNCTION__);
		MessageBoxA(NULL, "Failed to set global timer resolution!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] System Timer Resolution: Min %d Max %d Current %d",
		__FUNCTION__,
		g_performance_data.MinimumTimerResolution,
		g_performance_data.MaximumTimerResolution,
		g_performance_data.CurrentTimerResolution);

	if (SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) == 0)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set process priority! Error 0x%081x", __FUNCTION__, GetLastError());
		MessageBoxA(NULL, "Failed to set process priority!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set thread priority! Error 0x%081x", __FUNCTION__, GetLastError());
		MessageBoxA(NULL, "Failed to set thread priority!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (CreateMainGameWindow() != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] CreateMainGameWindow failed!", __FUNCTION__);
		MessageBoxA(NULL, "Failed to create game window!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	DWORD asset_file_attributes = GetFileAttributesA(ASSET_FILE);
	if ((asset_file_attributes == INVALID_FILE_ATTRIBUTES) || (asset_file_attributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		LogMessageA(LL_ERROR, "[%s] The asset file %s was not found! It must reside in the same directory as the game executable.", __FUNCTION__, ASSET_FILE);
		MessageBoxA(NULL, "The asset file was not found! It must reside in the same directory as the game executable.", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if ((g_asset_loading_thread_handle = CreateThread(NULL, 0, AssetLoadingThreadProc, NULL, 0, NULL)) == NULL)
	{
		MessageBox(NULL, "CreateThread failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (InitializeSoundEngine() != S_OK)
	{
		MessageBox(NULL, "InitializeSoundEngine failed", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}


	QueryPerformanceFrequency((LARGE_INTEGER*)&g_performance_data.PerfFrequency);

	g_back_buffer.BitmapInfo.bmiHeader.biSize = sizeof(g_back_buffer.BitmapInfo.bmiHeader);
	g_back_buffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
	g_back_buffer.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
	g_back_buffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	g_back_buffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	g_back_buffer.BitmapInfo.bmiHeader.biPlanes = 1;
	g_back_buffer.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (g_back_buffer.Memory == NULL)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to allocate memory for drawing surface! Error 0x%081x!", __FUNCTION__, GetLastError());
		MessageBox(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	memset(g_back_buffer.Memory, 0x7F, GAME_DRAWING_AREA_MEMORY_SIZE);

	if (InitializeHero() != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to initialize hero!", __FUNCTION__);
		MessageBox(NULL, "Failed to initialize hero!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	g_game_is_running = TRUE;

	while (g_game_is_running)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&frame_start);
		while (PeekMessageA(&message, g_game_window, 0, 0, PM_REMOVE))
		{
			DispatchMessageA(&message);
		}

		ProcessPlayerInput();
		RenderFrameGraphics();

		QueryPerformanceCounter((LARGE_INTEGER*)&frame_end);
		uint64_t elapsed_microseconds = frame_end - frame_start;
		elapsed_microseconds *= 1000000;
		elapsed_microseconds /= g_performance_data.PerfFrequency;
		g_performance_data.TotalFramesRendered++;
		elapsed_microseconds_accumulator_raw += elapsed_microseconds;

		while (elapsed_microseconds < TARGET_MICROSECONDS_PER_FRAME)
		{
			elapsed_microseconds = frame_end - frame_start;
			elapsed_microseconds *= 1000000;
			elapsed_microseconds /= g_performance_data.PerfFrequency;
			QueryPerformanceCounter((LARGE_INTEGER*)&frame_end);

			if ((float)elapsed_microseconds < (TARGET_MICROSECONDS_PER_FRAME * 0.75f))
			{
				Sleep(1);
			}
		}

		elapsed_microseconds_accumulator_cooked += elapsed_microseconds;

		if (g_performance_data.TotalFramesRendered % CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES == 0)
		{
			GetSystemTimeAsFileTime((LPFILETIME)&g_performance_data.CurrentSystemTime);

			FindFirstConnectedGamepad();

			GetProcessTimes(GetCurrentProcess(),
				&process_creation_time,
				&process_exit_time,
				(FILETIME*)&current_kernel_cpu_time,
				(FILETIME*)&current_user_cpu_time);

			g_performance_data.CPUPercent = (double)(current_kernel_cpu_time - previous_kernel_cpu_time) + (double)(current_user_cpu_time - previous_user_cpu_time);
			g_performance_data.CPUPercent /= (double)(g_performance_data.CurrentSystemTime - g_performance_data.PreviousSystemTime);
			g_performance_data.CPUPercent /= g_performance_data.SystemInfo.dwNumberOfProcessors;
			g_performance_data.CPUPercent *= 100;

			GetProcessHandleCount(GetCurrentProcess(), &g_performance_data.HandleCount);
			K32GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&g_performance_data.MemInfo, sizeof(g_performance_data.MemInfo));

			g_performance_data.RawFPSAverage = 1.0f / ((float)elapsed_microseconds_accumulator_raw / (float)CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES * 0.000001f);
			g_performance_data.CookedFPSAverage = 1.0f / ((float)elapsed_microseconds_accumulator_cooked / (float)CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES * 0.000001f);

			elapsed_microseconds_accumulator_raw = 0;
			elapsed_microseconds_accumulator_cooked = 0;

			previous_kernel_cpu_time = current_kernel_cpu_time;
			previous_user_cpu_time = current_user_cpu_time;
			g_performance_data.PreviousSystemTime = g_performance_data.CurrentSystemTime;

		}
	}

Exit:

	LogMessageA(LL_INFO, "Game is exiting.\r\n");
	return 0;
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
	LRESULT result = 0;

	switch (Message)
	{
		case WM_CLOSE:
		{
			g_game_is_running = FALSE;
			PostQuitMessage(0);
			break;
		}

		case WM_ACTIVATE:
		{
			if (WParam == 0)
			{
				// Our window has lost focus
				g_window_has_focus = FALSE;
			}
			else
			{
				// Our window has gained focus
				ShowCursor(FALSE);
				g_window_has_focus = TRUE;
			}
			break;
		}

		default:
		{
			result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
		}
	}

	return result;
}

DWORD CreateMainGameWindow(void)
{

	DWORD result = ERROR_SUCCESS;

	WNDCLASSEXA windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = 0;
	windowClass.lpfnWndProc = MainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(NULL);
	windowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	windowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	//#ifdef _DEBUG
	//	windowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
	//#else
	windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	//#endif
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

	//SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (RegisterClassExA(&windowClass) == 0)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] RegisterClassExA Failed! Error 0x%081x!", __FUNCTION__, result);
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	g_game_window = CreateWindowExA(0, "GAME_B_WINDOWCLASS", "Window Title",
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL,
		GetModuleHandleA(NULL), NULL);

	if (g_game_window == NULL)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] CreateWindowExA Failed! Error 0x%081x!", __FUNCTION__, result);
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	g_performance_data.MonitorInfo.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfoA(MonitorFromWindow(g_game_window, MONITOR_DEFAULTTOPRIMARY), &g_performance_data.MonitorInfo) == 0)
	{
		result = ERROR_MONITOR_NO_DESCRIPTOR;
		LogMessageA(LL_ERROR, "[%s] GetMonitorInfoA(MonitorFromWindow()) failed! Error 0x%081x!", __FUNCTION__, result);
		goto Exit;
	}

	for (uint8_t counter = 1; counter < 12; counter++)
	{
		if ((GAME_RES_WIDTH * counter > (g_performance_data.MonitorInfo.rcMonitor.right - g_performance_data.MonitorInfo.rcMonitor.left)) ||
			(GAME_RES_HEIGHT * counter > (g_performance_data.MonitorInfo.rcMonitor.bottom - g_performance_data.MonitorInfo.rcMonitor.top)))
		{
			g_performance_data.MaxScaleFactor = counter - 1;
			break;
		}
	}

	if (g_registry_params.ScaleFactor == 0)
	{
		g_performance_data.CurrentScaleFactor = g_performance_data.MaxScaleFactor;
	}
	else
	{
		g_performance_data.CurrentScaleFactor = (uint8_t)g_registry_params.ScaleFactor;
	}

	LogMessageA(LL_INFO, "[%s] Current scale factor is %d. Max scale factor is %d. ",
		__FUNCTION__,
		g_performance_data.CurrentScaleFactor,
		g_performance_data.MaxScaleFactor);

	LogMessageA(LL_INFO, "[%s] Will draw at %dx%d.",
		__FUNCTION__,
		GAME_RES_WIDTH * g_performance_data.CurrentScaleFactor,
		GAME_RES_HEIGHT * g_performance_data.CurrentScaleFactor);


	if (SetWindowLongPtrA(g_game_window, GWL_STYLE, WS_VISIBLE) == 0)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetWindowLongPtrA failed! Error 0x%081x!", __FUNCTION__, result);
		goto Exit;
	}

	if (SetWindowPos(g_game_window,
		HWND_TOP,
		g_performance_data.MonitorInfo.rcMonitor.left,
		g_performance_data.MonitorInfo.rcMonitor.top,
		g_performance_data.MonitorInfo.rcMonitor.right - g_performance_data.MonitorInfo.rcMonitor.left,
		g_performance_data.MonitorInfo.rcMonitor.bottom - g_performance_data.MonitorInfo.rcMonitor.top,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetWindowPos failed! Error 0x%081x!", __FUNCTION__, result);
		goto Exit;
	}


Exit:
	return result;
}

BOOL GameIsAlreadyRunning(void)
{
	CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return TRUE;
	}
	return FALSE;
}

void ProcessPlayerInput(void)
{

	if (g_window_has_focus == FALSE || g_input_enabled == FALSE)
	{
		return;
	}

	g_game_input.EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
	g_game_input.DebugKeyIsDown = GetAsyncKeyState(VK_F1);
	g_game_input.LeftKeyIsDown = (int16_t)(GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A'));
	g_game_input.RightKeyIsDown = (int16_t)(GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D'));
	g_game_input.UpKeyIsDown = (int16_t)(GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W'));
	g_game_input.DownKeyIsDown = (int16_t)(GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S'));
	g_game_input.ChooseKeyIsDown = GetAsyncKeyState(VK_RETURN);

	if (g_gamepad_id > -1)
	{
		if (XInputGetState(g_gamepad_id, &g_gamepad_state) == ERROR_SUCCESS)
		{
			g_game_input.EscapeKeyIsDown	= (int16_t)((int)g_game_input.EscapeKeyIsDown	| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK));
			g_game_input.LeftKeyIsDown		= (int16_t)((int)g_game_input.LeftKeyIsDown		| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT));
			g_game_input.RightKeyIsDown		= (int16_t)((int)g_game_input.RightKeyIsDown	| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT));
			g_game_input.UpKeyIsDown		= (int16_t)((int)g_game_input.UpKeyIsDown		| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP));
			g_game_input.DownKeyIsDown		= (int16_t)((int)g_game_input.DownKeyIsDown		| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN));
			g_game_input.ChooseKeyIsDown	= (int16_t)((int)g_game_input.ChooseKeyIsDown	| (g_gamepad_state.Gamepad.wButtons & XINPUT_GAMEPAD_A));
		}
		else
		{
			// Gamepad unplugged?
			g_gamepad_id = -1;
			g_previous_game_state = g_current_game_state;
			g_current_game_state = GS_GAMEPAD_UNPLUGGED;
		}
	}

	if (PRESSED_DEBUG)
	{
		g_performance_data.DisplayDebugInfo = !g_performance_data.DisplayDebugInfo;
	}

	switch (g_current_game_state)
	{
		case GS_OPENING_SPLASH_SCREEN:
		{
			PPI_OpeningSplashScreen();
			break;
		}
		case GS_GAMEPAD_UNPLUGGED:
		{
			PPI_GamepadUnplugged();
			break;
		}
		case GS_TITLE_SCREEN:
		{
			PPI_TitleScreen();
			break;
		}
		case GS_OVERWORLD:
		{
			PPI_Overworld();
			break;
		}
		case GS_BATTLE:
		{
			break;
		}
		case GS_CHARACTER_NAMING:
		{
			PPI_CharacterNaming();
			break;
		}
		case GS_OPTIONS_SCREEN:
		{
			PPI_OptionsScreen();
			break;
		}
		case GS_EXIT_YES_NO_SCREEN:
		{
			PPI_ExitYesNo();
			break;
		}

		default:
		{
			ASSERT(FALSE, "Unknown game state!")
		}
	}

	g_game_input.DebugKeyWasDown = g_game_input.DebugKeyIsDown;
	g_game_input.LeftKeyWasDown = g_game_input.LeftKeyIsDown;
	g_game_input.RightKeyWasDown = g_game_input.RightKeyIsDown;
	g_game_input.UpKeyWasDown = g_game_input.UpKeyIsDown;
	g_game_input.DownKeyWasDown = g_game_input.DownKeyIsDown;
	g_game_input.ChooseKeyWasDown = g_game_input.ChooseKeyIsDown;
	g_game_input.EscapeKeyWasDown = g_game_input.EscapeKeyIsDown;
}

DWORD InitializeHero(void) {
	g_player.ScreenPos.X = 192;
	g_player.ScreenPos.Y = 64;
	g_player.WorldPos.X = 192;
	g_player.WorldPos.Y = 64;
	g_player.CurrentArmor = SUIT_0;
	g_player.Direction = DOWN;
	return 0;
}

void BlitStringToBuffer(_In_ const char* String, _In_ const GAME_BITMAP* FontSheet, _In_ const PIXEL32* Color, _In_ const int16_t X, _In_ const int16_t Y)
{
	const uint16_t char_width = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biWidth / FONT_SHEET_CHARACTERS_PER_ROW;
	const uint16_t char_height = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biHeight;
	const uint16_t bytes_per_character = (char_width * char_height * (FontSheet->BitmapInfo.bmiHeader.biBitCount / 8));
	const uint16_t string_length = (uint16_t)strlen(String);
	GAME_BITMAP string_bitmap = { 0 };
	string_bitmap.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	string_bitmap.BitmapInfo.bmiHeader.biHeight = char_height;
	string_bitmap.BitmapInfo.bmiHeader.biWidth = char_width * string_length;
	string_bitmap.BitmapInfo.bmiHeader.biPlanes = 1;
	string_bitmap.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	string_bitmap.Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (size_t)bytes_per_character * (size_t)string_length);

	for (int character = 0; character < string_length; character++)
	{
		PIXEL32 font_sheet_pixel = { 0 };
		const int starting_font_sheet_pixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight)
			- FontSheet->BitmapInfo.bmiHeader.biWidth + (char_width * g_font_character_pixel_offset[(uint8_t)String[character]]);

		for (int y_pixel = 0; y_pixel <= char_height - 1; y_pixel++)
		{
			for (int x_pixel = 0; x_pixel < char_width - 1; x_pixel++)
			{
				const int font_sheet_offset = starting_font_sheet_pixel + x_pixel - (FontSheet->BitmapInfo.bmiHeader.biWidth * y_pixel);
				const int string_bitmap_offset = (character * char_width) + ((string_bitmap.BitmapInfo.bmiHeader.biWidth * string_bitmap.
					BitmapInfo.bmiHeader.biHeight) -
					string_bitmap.BitmapInfo.bmiHeader.biWidth) + x_pixel - (string_bitmap.BitmapInfo.bmiHeader.biWidth * y_pixel);

				memcpy_s(&font_sheet_pixel, sizeof(PIXEL32), (PIXEL32*)FontSheet->Memory + font_sheet_offset, sizeof(PIXEL32));

				font_sheet_pixel.Red = Color->Red;
				font_sheet_pixel.Green = Color->Green;
				font_sheet_pixel.Blue = Color->Blue;

				memcpy_s((PIXEL32*)string_bitmap.Memory + string_bitmap_offset, sizeof(PIXEL32), &font_sheet_pixel, sizeof(PIXEL32));
			}
		}
	}

	Blit32BppBitmapToBuffer(&string_bitmap, X, Y, 0);

	if (string_bitmap.Memory)
	{
		HeapFree(GetProcessHeap(), 0, string_bitmap.Memory);
	}
}

void RenderFrameGraphics(void)
{

	switch (g_current_game_state)
	{

		case GS_OPENING_SPLASH_SCREEN:
		{
			DrawOpeningSplashScreen();
			break;
		}

		case GS_TITLE_SCREEN:
		{
			DrawTitleScreen();
			break;
		}

		case GS_CHARACTER_NAMING:
		{
			DrawCharacterNamingScreen();
			break;
		}

		case GS_GAMEPAD_UNPLUGGED:
		{
			DrawGamepadUnpluggedScreen();
			break;
		}

		case GS_OVERWORLD:
		{
			DrawOverworldScreen();
			break;
		}

		case GS_BATTLE:
		{
			break;
		}

		case GS_OPTIONS_SCREEN:
		{
			DrawOptionsScreen();
			break;
		}

		case GS_EXIT_YES_NO_SCREEN:
		{
			DrawExitYesNoScreen();
			break;
		}

		default:
		{
			ASSERT(FALSE, "GameState not implemented")
		}
	}

	if (g_performance_data.DisplayDebugInfo == TRUE)
	{
		DrawDebugInfo();
	}

	// ReSharper disable once CppLocalVariableMayBeConst
	HDC device_context = GetDC(g_game_window);

	StretchDIBits(device_context,
		((g_performance_data.MonitorInfo.rcMonitor.right - g_performance_data.MonitorInfo.rcMonitor.left) / 2) - ((GAME_RES_WIDTH * g_performance_data.CurrentScaleFactor) / 2),
		((g_performance_data.MonitorInfo.rcMonitor.bottom - g_performance_data.MonitorInfo.rcMonitor.top) / 2) - ((GAME_RES_HEIGHT * g_performance_data.CurrentScaleFactor) / 2),
		(GAME_RES_WIDTH * g_performance_data.CurrentScaleFactor),
		(GAME_RES_HEIGHT * g_performance_data.CurrentScaleFactor),
		0,
		0,
		GAME_RES_WIDTH,
		GAME_RES_HEIGHT,
		g_back_buffer.Memory,
		&g_back_buffer.BitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	ReleaseDC(g_game_window, device_context);
}

#ifdef AVX
void ClearScreen(_In_ const __m256i* Color)
{
	for (int index = 0; index < (GAME_RES_WIDTH * GAME_RES_HEIGHT) / 8; index++)
	{
		_mm256_store_si256((__m256i*)g_back_buffer.Memory + index, *Color);
	}
}
#elif defined SSE2
void ClearScreen(_In_ const __m128i* Color)
{
	for (int index = 0; index < (GAME_RES_WIDTH * GAME_RES_HEIGHT) / 4; index++)
	{
		_mm_store_si128((__m128i*)g_back_buffer.Memory + index, *Color);
	}
}
#else
void ClearScreen(_In_ const PIXEL32* Pixel)
{
	for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x++)
	{
		memcpy((PIXEL32*)g_back_buffer.Memory + x, Pixel, sizeof(PIXEL32));
	}
}
#endif

void Blit32BppBitmapToBuffer(_In_ const GAME_BITMAP* GameBitmap, _In_ const int16_t X, _In_ const int16_t Y, _In_ int16_t BrightnessAdjustment)
{
	const int32_t starting_screen_pixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * Y) + X;
	const int32_t starting_bitmap_pixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight)
		- GameBitmap->BitmapInfo.bmiHeader.biWidth);
	PIXEL32 bitmap_pixel = { 0 };

	for (int32_t y_pixel = 0; y_pixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; y_pixel++)
	{

		if ((Y + y_pixel < 0) || (Y + y_pixel >= GAME_RES_HEIGHT))
		{
			continue;
		}

		for (int32_t x_pixel = 0; x_pixel < GameBitmap->BitmapInfo.bmiHeader.biWidth; x_pixel++)
		{

			if ((X + x_pixel < 0) || (X + x_pixel >= GAME_RES_WIDTH))
			{
				continue;
			}

			const int32_t memory_offset = starting_screen_pixel + x_pixel - (GAME_RES_WIDTH * y_pixel);
			const int32_t bitmap_offset = starting_bitmap_pixel + x_pixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * y_pixel);

			memcpy_s(&bitmap_pixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + bitmap_offset, sizeof(PIXEL32));

			if (bitmap_pixel.Alpha == 255)
			{
				bitmap_pixel.Red   = (uint8_t)min(255, max(0, bitmap_pixel.Red   + BrightnessAdjustment));
				bitmap_pixel.Blue  = (uint8_t)min(255, max(0, bitmap_pixel.Blue  + BrightnessAdjustment));
				bitmap_pixel.Green = (uint8_t)min(255, max(0, bitmap_pixel.Green + BrightnessAdjustment));
				memcpy_s((PIXEL32*)g_back_buffer.Memory + memory_offset, sizeof(PIXEL32), &bitmap_pixel, sizeof(PIXEL32));
			}
		}
	}

}

void BlitBackgroundToBuffer(_In_ const GAME_BITMAP* GameBitmap, _In_ int16_t BrightnessAdjustment)
{
	const int32_t starting_screen_pixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH);

	const int32_t starting_bitmap_pixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight)
		- GameBitmap->BitmapInfo.bmiHeader.biWidth) + g_camera.X - (GameBitmap->BitmapInfo.bmiHeader.biWidth * g_camera.Y);

	int32_t memory_offset;
	int32_t bitmap_offset;

#ifdef AVX
	for (int16_t y_pixel = 0; y_pixel < GAME_RES_HEIGHT; y_pixel++)
	{
		for (int16_t x_pixel = 0; x_pixel < GAME_RES_WIDTH; x_pixel += 8)
		{
			memory_offset = starting_screen_pixel + x_pixel - (GAME_RES_WIDTH * y_pixel);
			bitmap_offset = starting_bitmap_pixel + x_pixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * y_pixel);
			__m256i bitmap_octo_pixel = _mm256_loadu_si256((const __m256i*)((PIXEL32*)GameBitmap->Memory + bitmap_offset));  // NOLINT(clang-diagnostic-cast-align)

			__m256i half_1 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(bitmap_octo_pixel, 0));
			half_1 = _mm256_add_epi16(half_1, _mm256_set1_epi16(BrightnessAdjustment));

			__m256i half_2 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(bitmap_octo_pixel, 1));
			half_2 = _mm256_add_epi16(half_2, _mm256_set1_epi16(BrightnessAdjustment));

			const __m256i recombined = _mm256_packus_epi16(half_1, half_2);
			bitmap_octo_pixel = _mm256_permute4x64_epi64(recombined, _MM_SHUFFLE(3, 1, 2, 0));

			_mm256_store_si256((__m256i*)((PIXEL32*)g_back_buffer.Memory + memory_offset), bitmap_octo_pixel);  // NOLINT(clang-diagnostic-cast-align)
		}
	}
#elif defined SSE2
	for (int16_t y_pixel = 0; y_pixel < GAME_RES_HEIGHT; y_pixel++)
	{
		for (int16_t x_pixel = 0; x_pixel < GAME_RES_WIDTH; x_pixel += 4)
		{
			memory_offset = starting_screen_pixel + x_pixel - (GAME_RES_WIDTH * y_pixel);
			bitmap_offset = starting_bitmap_pixel + x_pixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * y_pixel);
			__m128i bitmap_quad_pixel = _mm_load_si128((const __m128i*)((PIXEL32*)GameBitmap->Memory + bitmap_offset));
			for (int8_t i = 0; i < 16; i++)
			{
				bitmap_quad_pixel.m128i_u8[i] = (uint8_t)min(255, max(0, bitmap_quad_pixel.m128i_u8[i] + BrightnessAdjustment));
			}
			_mm_store_si128((__m128i*)((PIXEL32*)g_back_buffer.Memory + memory_offset), bitmap_quad_pixel);
		}
	}
#else
	PIXEL32 bitmap_pixel = { 0 };
	for (int16_t y_pixel = 0; y_pixel < GAME_RES_HEIGHT; y_pixel++)
	{
		for (int16_t x_pixel = 0; x_pixel < GAME_RES_WIDTH; x_pixel++)
		{
			memory_offset = starting_screen_pixel + x_pixel - (GAME_RES_WIDTH * y_pixel);
			bitmap_offset = starting_bitmap_pixel + x_pixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * y_pixel);
			memcpy_s(&bitmap_pixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + bitmap_offset, sizeof(PIXEL32));
			bitmap_pixel.Red = (uint8_t)min(255, max(0, bitmap_pixel.Red + BrightnessAdjustment));
			bitmap_pixel.Green = (uint8_t)min(255, max(0, bitmap_pixel.Green + BrightnessAdjustment));
			bitmap_pixel.Blue = (uint8_t)min(255, max(0, bitmap_pixel.Blue + BrightnessAdjustment));
			bitmap_pixel.Alpha = (uint8_t)min(255, max(0, bitmap_pixel.Alpha + BrightnessAdjustment));
			memcpy_s((PIXEL32*)g_back_buffer.Memory + memory_offset, sizeof(PIXEL32), &bitmap_pixel, sizeof(PIXEL32));
		}
	}
#endif
}

DWORD LoadRegistryParameters(void)
{
	HKEY reg_key = NULL;
	DWORD reg_disposition = 0;
	DWORD reg_bytes_read = sizeof(DWORD);
	DWORD result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL,
		&reg_key, &reg_disposition);

	if (result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	if (reg_disposition == REG_CREATED_NEW_KEY)
	{
		LogMessageA(LL_INFO, "[%s] Registry key did not exist; created new key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
	}
	else
	{
		LogMessageA(LL_INFO, "[%s] Opened existing registry key HKCU\\SOFTWARE\\%s", __FUNCTION__, GAME_NAME);
	}

	result = RegGetValueA(reg_key, NULL, "LogLevel", RRF_RT_DWORD, NULL, (BYTE*)&g_registry_params.LogLevel, &reg_bytes_read);
	if (result != ERROR_SUCCESS)
	{
		if (result == ERROR_FILE_NOT_FOUND)
		{
			LogMessageA(LL_INFO, "[%s] Registry value 'LogLevel' not found. Using default of 0. (LOG_LEVEL_NONE)", __FUNCTION__);
			g_registry_params.LogLevel = LL_NONE;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'LogLevel' registry value. Error 0x%08lx!", __FUNCTION__, result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] LogLevel is %d.", __FUNCTION__, g_registry_params.LogLevel);

	result = RegGetValueA(reg_key, NULL, "ScaleFactor", RRF_RT_DWORD, NULL, (BYTE*)&g_registry_params.ScaleFactor, &reg_bytes_read);
	if (result != ERROR_SUCCESS)
	{
		if (result == ERROR_FILE_NOT_FOUND)
		{
			LogMessageA(LL_INFO, "[%s] Registry value 'ScaleFactor' not found. Using default of 0.", __FUNCTION__);
			g_registry_params.ScaleFactor = 0;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'ScaleFactor' registry value. Error 0x%08lx!", __FUNCTION__, result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] ScaleFactor is %d.", __FUNCTION__, g_registry_params.ScaleFactor);

	result = RegGetValueA(reg_key, NULL, "SFXVolume", RRF_RT_DWORD, NULL, (BYTE*)&g_registry_params.SFXVolume, &reg_bytes_read);
	if (result != ERROR_SUCCESS)
	{
		if (result == ERROR_FILE_NOT_FOUND)
		{
			LogMessageA(LL_INFO, "[%s] Registry value 'SFXVolume' not found. Using default of 0.5", __FUNCTION__);
			g_registry_params.SFXVolume = 50;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'SFXVolume' registry value. Error 0x%08lx!", __FUNCTION__, result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] SFXVolume is %.1f.", __FUNCTION__, (double)g_registry_params.SFXVolume / 100.0);
	g_sfx_volume = ((float)g_registry_params.SFXVolume / 100.0f);

	result = RegGetValueA(reg_key, NULL, "MusicVolume", RRF_RT_DWORD, NULL, (BYTE*)&g_registry_params.MusicVolume, &reg_bytes_read);
	if (result != ERROR_SUCCESS)
	{
		if (result == ERROR_FILE_NOT_FOUND)
		{
			result = ERROR_SUCCESS;
			LogMessageA(LL_INFO, "[%s] Registry value 'MusicVolume' not found. Using default of 0.5", __FUNCTION__);
			g_registry_params.MusicVolume = 50;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'MusicVolume' registry value. Error 0x%08lx!", __FUNCTION__, result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] MusicVolume is %.1f.", __FUNCTION__, (double)g_registry_params.MusicVolume / 100.0);
	g_music_volume = (float)(g_registry_params.MusicVolume / 100.0);

Exit:

	if (reg_key)
	{
		RegCloseKey(reg_key);
	}
	return result;
}

DWORD SaveRegistryParameters(void)
{
	HKEY reg_key = NULL;
	DWORD reg_disposition = 0;
	const DWORD sfx_volume = (DWORD)(g_sfx_volume * 100.0f);
	const DWORD music_volume = (DWORD)(g_music_volume * 100.0f);
	DWORD result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL,
		&reg_key, &reg_disposition);

	if (result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] Reg key open for save. ", __FUNCTION__);

	result = RegSetValueExA(reg_key, "SFXVolume", 0, REG_DWORD, (const BYTE*)&sfx_volume, sizeof(DWORD));
	if (result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'SFXVolume' in registry. Error code 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] SFXVolume saved %d. ", __FUNCTION__, sfx_volume);

	result = RegSetValueExA(reg_key, "MusicVolume", 0, REG_DWORD, (const BYTE*)&music_volume, sizeof(DWORD));
	if (result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'MusicVolume' in registry. Error code 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] MusicVolume saved %d. ", __FUNCTION__, music_volume);

	result = RegSetValueExA(reg_key, "ScaleFactor", 0, REG_DWORD, &g_performance_data.CurrentScaleFactor, sizeof(DWORD));
	if (result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'ScaleFactor' in registry. Error code 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] ScaleFactor saved %d. ", __FUNCTION__, g_performance_data.CurrentScaleFactor);

Exit:
	if (reg_key)
	{
		RegCloseKey(reg_key);
	}
	return result;
}

void LogMessageA(_In_ LOG_LEVEL LogLevel, _In_ char* Message, _In_ ...)
{
	const size_t message_length = strlen(Message);
	SYSTEMTIME time = { 0 };
	HANDLE log_file_handle;
	DWORD number_of_bytes_written = 0;
	char date_time_string[96] = { 0 };
	char severity_string[8] = { 0 };
	char formatted_string[4096] = { 0 };
	if ((LOG_LEVEL)g_registry_params.LogLevel < LogLevel)
	{
		return;
	}

	if (message_length < 1 || message_length > 4096)
	{
		ASSERT(FALSE, "Message was either too short or too long!")
			return;
	}

	switch (LogLevel)
	{
		case LL_NONE:
		{
			return;
		}
		case LL_INFO:
		{
			strcpy_s(severity_string, sizeof(severity_string), "[INFO] ");
			break;
		}
		case LL_WARNING:
		{
			strcpy_s(severity_string, sizeof(severity_string), "[WARN] ");
			break;
		}
		case LL_ERROR:
		{
			strcpy_s(severity_string, sizeof(severity_string), "[ERROR]");
			break;
		}
		case LL_DEBUG:
		{
			strcpy_s(severity_string, sizeof(severity_string), "[DEBUG]");
			break;
		}
		default:
		{
			ASSERT(FALSE, "LogLevel was unrecognized.")
		}
	}

	GetLocalTime(&time);

	va_list ArgPointer = NULL;
	va_start(ArgPointer, Message);
	_vsnprintf_s(formatted_string, sizeof(formatted_string), _TRUNCATE, Message, ArgPointer);
	va_end(ArgPointer);
	_snprintf_s(date_time_string, sizeof(date_time_string), _TRUNCATE, "\r\n[%02u/%02u/%u %02u:%02u:%02u.%03u]", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	EnterCriticalSection(&g_log_critical_section);

	if ((log_file_handle = CreateFileA(LOG_FILE_NAME, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE, "Failed to access log file!")
	}

	SetFilePointer(log_file_handle, 0, NULL, FILE_END);

	WriteFile(log_file_handle, date_time_string, (DWORD)strlen(date_time_string), &number_of_bytes_written, NULL);
	WriteFile(log_file_handle, severity_string, (DWORD)strlen(severity_string), &number_of_bytes_written, NULL);
	WriteFile(log_file_handle, formatted_string, (DWORD)strlen(formatted_string), &number_of_bytes_written, NULL);
	if (log_file_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(log_file_handle);
	}

	LeaveCriticalSection(&g_log_critical_section);
}

void DrawDebugInfo(void)
{
	char debug_text_buffer[64] = { 0 };
	const PIXEL32 white = { 0xFF,0xFF, 0xFF, 0xFF };
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "FPSRaw:  %.01f", (double)g_performance_data.RawFPSAverage);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 0);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "FPSCookd:%.01f", (double)g_performance_data.CookedFPSAverage);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 8);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "MinTimer:%.02f", (double)g_performance_data.MinimumTimerResolution / 10000.0);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 16);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "MaxTimer:%.02f", (double)g_performance_data.MaximumTimerResolution / 10000.0);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 24);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "CurTimer:%.02f", (double)g_performance_data.CurrentTimerResolution / 10000.0);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 32);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "Handles: %lu", g_performance_data.HandleCount);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 40);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "Memory:  %i KB", (int)(g_performance_data.MemInfo.PrivateUsage / 1024));
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 48);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "CPU:     %.02f%%", g_performance_data.CPUPercent);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 56);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "FramesT: %llu", g_performance_data.TotalFramesRendered);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 64);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "ScreenXY:%hu,%hu", g_player.ScreenPos.X, g_player.ScreenPos.Y);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 72);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "WorldXY: %hu,%hu", g_player.WorldPos.X, g_player.WorldPos.Y);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 80);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "CameraXY:%hu,%hu", g_camera.X, g_camera.Y);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 88);
	sprintf_s(debug_text_buffer, _countof(debug_text_buffer), "Movement:%u", g_player.MovementRemaining);
	BlitStringToBuffer(debug_text_buffer, &g_6x7_font, &white, 0, 96);
}

void FindFirstConnectedGamepad(void)
{
	g_gamepad_id = -1;
	for (int8_t gamepad_index = 0; gamepad_index < XUSER_MAX_COUNT && g_gamepad_id == -1; gamepad_index++)
	{
		XINPUT_STATE state = { 0 };
		if (XInputGetState(gamepad_index, &state) == ERROR_SUCCESS)
		{
			g_gamepad_id = gamepad_index;
		}
	}
}

HRESULT InitializeSoundEngine(void)
{
	WAVEFORMATEX sfx_wave_format;
	WAVEFORMATEX music_wave_format;

	HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (result != S_OK)
	{
		LogMessageA(LL_ERROR, "[%s] CoInitializeEx failed with 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	result = XAudio2Create(&g_xaudio, 0, XAUDIO2_ANY_PROCESSOR);

	if (FAILED(result))
	{
		LogMessageA(LL_ERROR, "[%s] XAudio2Create failed with 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	result = g_xaudio->lpVtbl->CreateMasteringVoice(g_xaudio, &g_xaudio_mastering_voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0);
	if (FAILED(result))
	{
		LogMessageA(LL_ERROR, "[%s] CreateMasteringVoice failed with 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}

	sfx_wave_format.wFormatTag = WAVE_FORMAT_PCM;
	sfx_wave_format.nChannels = 1;
	sfx_wave_format.nSamplesPerSec = 44100;
	sfx_wave_format.nAvgBytesPerSec = sfx_wave_format.nSamplesPerSec * sfx_wave_format.nChannels * 2;
	sfx_wave_format.nBlockAlign = sfx_wave_format.nChannels * 2;
	sfx_wave_format.wBitsPerSample = 16;
	sfx_wave_format.cbSize = 0x6164;

	for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
	{
		result = g_xaudio->lpVtbl->CreateSourceVoice(g_xaudio, &g_xaudio_sfx_source_voice[Counter], &sfx_wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
		if (result != S_OK)
		{
			LogMessageA(LL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, result);
			goto Exit;
		}
		g_xaudio_sfx_source_voice[Counter]->lpVtbl->SetVolume(g_xaudio_sfx_source_voice[Counter], g_sfx_volume, XAUDIO2_COMMIT_NOW);
	}

	music_wave_format.wFormatTag = WAVE_FORMAT_PCM;
	music_wave_format.nChannels = 2;
	music_wave_format.nSamplesPerSec = 44100;
	music_wave_format.nAvgBytesPerSec = music_wave_format.nSamplesPerSec * music_wave_format.nChannels * 2;
	music_wave_format.nBlockAlign = music_wave_format.nChannels * 2;
	music_wave_format.wBitsPerSample = 16;
	music_wave_format.cbSize = 0;

	result = g_xaudio->lpVtbl->CreateSourceVoice(g_xaudio, &g_xaudio_music_source_voice, &music_wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
	if (result != S_OK)
	{
		LogMessageA(LL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, result);
		goto Exit;
	}
	g_xaudio_music_source_voice->lpVtbl->SetVolume(g_xaudio_music_source_voice, g_music_volume, XAUDIO2_COMMIT_NOW);

Exit:
	return result;
}

DWORD LoadWavFromMemory(_In_ void* Buffer, _Inout_ GAME_SOUND* GameSound)
{
	DWORD error = ERROR_SUCCESS;
	DWORD riff = 0;
	uint16_t data_chunk_offset = 0;
	DWORD data_chunk_searcher = 0;
	DWORD data_chunk_size = 0;

	memcpy(&riff, Buffer, sizeof(DWORD));
	if (riff != 0x46464952) // "RIFF" Backwards
	{
		error = ERROR_FILE_INVALID;
		LogMessageA(LL_ERROR, "[%s] First four bytes of memory buffer are not 'RIFF'", __FUNCTION__);
		goto Exit;
	}

	memcpy(&GameSound->WaveFormat, (BYTE*)Buffer + 20, sizeof(WAVEFORMATEX));

	if (GameSound->WaveFormat.nBlockAlign != ((GameSound->WaveFormat.nChannels * GameSound->WaveFormat.wBitsPerSample) / 8) ||
		(GameSound->WaveFormat.wFormatTag != WAVE_FORMAT_PCM) ||
		(GameSound->WaveFormat.wBitsPerSample != 16))
	{
		error = ERROR_DATATYPE_MISMATCH;
		LogMessageA(LL_ERROR, "[%s] This wav data in the memory buffer did not meet the format requirements! Only PCM format, 44.1Khz, 16 bits per sample wav files are supported. 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	while (data_chunk_offset < 260)
	{
		memcpy(&data_chunk_searcher, (BYTE*)Buffer + data_chunk_offset, sizeof(DWORD));
		if (data_chunk_searcher == 0x61746164)	// 'data' backwards
		{
			break;
		}
		else
		{
			data_chunk_offset += 4;
		}

		if (data_chunk_offset > 256)
		{
			error = ERROR_DATATYPE_MISMATCH;
			LogMessageA(LL_ERROR, "[%s] Data chunk not found within first 256 bytes of the memory buffer! 0x%08lx!", __FUNCTION__, error);
			goto Exit;
		}
	}

	memcpy(&data_chunk_size, (BYTE*)Buffer + data_chunk_offset + 4, sizeof(DWORD));

	GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
	GameSound->Buffer.AudioBytes = data_chunk_size;
	GameSound->Buffer.pAudioData = (BYTE*)Buffer + data_chunk_offset + 8;

Exit:

	if (error == ERROR_SUCCESS)
	{
		LogMessageA(LL_INFO, "[%s] Successfully loaded wav from memory!", __FUNCTION__);
	}
	else
	{
		LogMessageA(LL_ERROR, "[%s] Failed to load wav from memory! Error: 0x%08lx!", __FUNCTION__, error);
	}

	return error;
}

DWORD LoadOggFromMemory(_In_ const void* Buffer, _In_ const uint64_t BufferSize, _Inout_ GAME_SOUND* GameSound)
{
	DWORD error = ERROR_SUCCESS;
	int channels = 0;
	int sample_rate = 0;
	short* decoded_audio = NULL;

	const int samples_decoded = stb_vorbis_decode_memory(Buffer, (int)BufferSize, &channels, &sample_rate, &decoded_audio);
	if (samples_decoded < 1)
	{
		error = ERROR_BAD_COMPRESSION_BUFFER;
		LogMessageA(LL_ERROR, "[%s] stb_vorbis_decode_memory failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	GameSound->WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	GameSound->WaveFormat.nChannels = (WORD)channels;
	GameSound->WaveFormat.nSamplesPerSec = sample_rate;
	GameSound->WaveFormat.nAvgBytesPerSec = GameSound->WaveFormat.nSamplesPerSec * GameSound->WaveFormat.nChannels * 2;
	GameSound->WaveFormat.nBlockAlign = GameSound->WaveFormat.nChannels * 2;
	GameSound->WaveFormat.wBitsPerSample = 16;
	GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
	GameSound->Buffer.AudioBytes = samples_decoded * GameSound->WaveFormat.nChannels * 2;
	GameSound->Buffer.pAudioData = (BYTE*)decoded_audio;

Exit:

	return error;
}

DWORD LoadTileMapFromMemory(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap)
{
	DWORD error = ERROR_SUCCESS;
	char* cursor;
	char temp_buffer[16] = { 0 };

	if (BufferSize < 300)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Buffer is too small to be a valid tile map! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((cursor = strstr(Buffer, "width=")) == NULL)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Could not locate the width attribute! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	DWORD bytes_read = 0;
	for (;;)
	{
		if (bytes_read > 8)
		{
			// We should have found the opening quotation mark by now.
			error = ERROR_INVALID_DATA;
			LogMessageA(LL_ERROR, "[%s] Could not locate the opening quotation mark before the width attribute! 0x%08lx!", __FUNCTION__, error);
			goto Exit;
		}

		if (*cursor == '\"')
		{
			cursor++;
			break;
		}
		else
		{
			cursor++;
		}

		bytes_read++;
	}

	for (uint8_t counter = 0; counter < 6; counter++)
	{
		if (*cursor == '\"')
		{
			break;
		}
		else
		{
			temp_buffer[counter] = *cursor;
			cursor++;
		}
	}

	TileMap->Width = (uint16_t)strtol(temp_buffer, NULL, 10);

	if (TileMap->Width == 0)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Width attribute was 0! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	memset(temp_buffer, 0, sizeof(temp_buffer));

	if ((cursor = strstr(Buffer, "height=")) == NULL)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Could not locate the height attribute! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	bytes_read = 0;
	for (;;)
	{
		if (bytes_read > 8)
		{
			// We should have found the opening quotation mark by now.
			error = ERROR_INVALID_DATA;
			LogMessageA(LL_ERROR, "[%s] Could not locate the opening quotation mark before the height attribute! 0x%08lx!", __FUNCTION__, error);
			goto Exit;
		}

		if (*cursor == '\"')
		{
			cursor++;
			break;
		}
		else
		{
			cursor++;
		}

		bytes_read++;
	}

	for (uint8_t counter = 0; counter < 6; counter++)
	{
		if (*cursor == '\"')
		{
			break;
		}
		else
		{
			temp_buffer[counter] = *cursor;
			cursor++;
		}
	}

	TileMap->Height = (uint16_t)strtol(temp_buffer, NULL, 10);

	if (TileMap->Height == 0)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Height attribute was 0! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] TileMap dimensions: %dx%d", __FUNCTION__, TileMap->Width, TileMap->Height);
	const uint16_t rows = TileMap->Height;
	const uint16_t columns = TileMap->Width;
	TileMap->Map = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, rows * sizeof(void*));

	if (TileMap->Map == NULL)
	{
		error = ERROR_OUTOFMEMORY;
		LogMessageA(LL_ERROR, "[%s] HeapAlloc failed! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	for (uint16_t counter = 0; counter < TileMap->Height; counter++)
	{
		TileMap->Map[counter] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, columns * sizeof(void*));
		if (TileMap->Map[counter] == NULL)
		{
			error = ERROR_OUTOFMEMORY;
			LogMessageA(LL_ERROR, "[%s] HeapAlloc failed! 0x%08lx!", __FUNCTION__, error);
			goto Exit;
		}
	}

	bytes_read = 0;
	memset(temp_buffer, 0, sizeof(temp_buffer));

	if ((cursor = strstr(Buffer, ",")) == NULL)
	{
		error = ERROR_INVALID_DATA;
		LogMessageA(LL_ERROR, "[%s] Could not find a comma character! 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	while (*cursor != '\r' && *cursor != '\n')
	{
		if (bytes_read > 3)
		{
			error = ERROR_INVALID_DATA;
			LogMessageA(LL_ERROR, "[%s] Could not find a new line character at the beginning of the tile map data! 0x%08lx!", __FUNCTION__, error);
			goto Exit;
		}

		bytes_read++;
		cursor--;
	}
	cursor++;

	for (uint16_t row = 0; row < rows; row++)
	{
		for (uint16_t column = 0; column < columns; column++)
		{
			memset(temp_buffer, 0, sizeof(temp_buffer));

			while (*cursor == '\r' || *cursor == '\n')
			{
				cursor++;
			}

			for (uint8_t counter = 0; counter < 8; counter++)
			{
				if (*cursor == ',' || *cursor == '<')
				{
					if (((TileMap->Map[row][column]) = (uint8_t)strtol(temp_buffer, NULL, 10)) == 0)
					{
						error = ERROR_INVALID_DATA;
						LogMessageA(LL_ERROR, "[%s] strtol failed while converting tile map data! 0x%08lx!", __FUNCTION__, error);
						goto Exit;
					}

					cursor++;
					break;
				}

				temp_buffer[counter] = *cursor;
				cursor++;
			}
		}
	}

Exit:

	if (Buffer)
	{
		mz_free(Buffer);
	}

	return error;
}

DWORD Load32BppBitmapFromMemory(_In_ void* Buffer, _Inout_ GAME_BITMAP* GameBitmap)
{
	DWORD error = ERROR_SUCCESS;
	WORD bitmap_header = 0;
	DWORD pixel_data_offset = 0;

	memcpy(&bitmap_header, Buffer, sizeof(WORD));

	if (bitmap_header != 0x4D42) // "BM" Backwards
	{
		error = ERROR_INVALID_DATA;
		goto Exit;
	}

	memcpy(&pixel_data_offset, (BYTE*)Buffer + 0xA, sizeof(DWORD));
	memcpy(&GameBitmap->BitmapInfo.bmiHeader, (BYTE*)Buffer + 0xE, sizeof(BITMAPINFOHEADER));

	GameBitmap->Memory = (BYTE*)Buffer + pixel_data_offset;

Exit:

	if (error == ERROR_SUCCESS)
	{
		LogMessageA(LL_INFO, "[%s] Loading successful.", __FUNCTION__);
	}
	else
	{
		LogMessageA(LL_ERROR, "[%s] Loading failed: Error 0x%081x!", __FUNCTION__, error);
	}

	return error;
}

void PlayGameSound(_In_ const GAME_SOUND* GameSound)
{
	g_xaudio_sfx_source_voice[g_sfx_source_voice_selector]->lpVtbl->SubmitSourceBuffer(g_xaudio_sfx_source_voice[g_sfx_source_voice_selector], &GameSound->Buffer, NULL);
	g_xaudio_sfx_source_voice[g_sfx_source_voice_selector]->lpVtbl->Start(g_xaudio_sfx_source_voice[g_sfx_source_voice_selector], 0, XAUDIO2_COMMIT_NOW);
	g_sfx_source_voice_selector++;
	if (g_sfx_source_voice_selector > (NUMBER_OF_SFX_SOURCE_VOICES - 1))
	{
		g_sfx_source_voice_selector = 0;
	}
}

void PlayGameMusic(_In_ GAME_SOUND* GameSound)
{
	g_xaudio_music_source_voice->lpVtbl->Stop(g_xaudio_music_source_voice, 0, 0);
	g_xaudio_music_source_voice->lpVtbl->FlushSourceBuffers(g_xaudio_music_source_voice);
	GameSound->Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	g_xaudio_music_source_voice->lpVtbl->SubmitSourceBuffer(g_xaudio_music_source_voice, &GameSound->Buffer, NULL);
	g_xaudio_music_source_voice->lpVtbl->Start(g_xaudio_music_source_voice, 0, XAUDIO2_COMMIT_NOW);
}

DWORD LoadAssetFromArchive(_In_ char* ArchiveName, _In_ char* AssetFileName, _In_ RESOURCE_TYPE ResourceType, _Inout_ void* Resource)
{
	DWORD error = ERROR_SUCCESS;
	mz_zip_archive archive = { 0 };
	BYTE* decompressed_buffer = NULL;
	size_t decompressed_size = 0;
	BOOL file_found_in_archive = FALSE;

	if (mz_zip_reader_init_file(&archive, ArchiveName, 0) == FALSE)
	{
		error = mz_zip_get_last_error(&archive);
		LogMessageA(LL_ERROR, "[%s] mz_zip_reader_init_file failed with 0x%08lx on archive file %s! Error: %s", __FUNCTION__, error, ArchiveName, mz_zip_get_error_string(error));
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] Archive %s opened.", __FUNCTION__, ArchiveName);

	for (uint32_t file_index = 0; file_index < mz_zip_reader_get_num_files(&archive); file_index++)
	{
		mz_zip_archive_file_stat compressed_file_statistics = { 0 };
		if (mz_zip_reader_file_stat(&archive, file_index, &compressed_file_statistics) == FALSE)
		{
			error = mz_zip_get_last_error(&archive);
			LogMessageA(LL_ERROR, "[%s] mz_zip_reader_file_stat failed with 0x%08lx on archive file %s! Error: %s", __FUNCTION__, error, ArchiveName, mz_zip_get_error_string(error));
			goto Exit;
		}

		if (_stricmp(compressed_file_statistics.m_filename, AssetFileName) == 0)
		{
			file_found_in_archive = TRUE;

			if ((decompressed_buffer = mz_zip_reader_extract_to_heap(&archive, file_index, &decompressed_size, 0)) == NULL)
			{
				error = mz_zip_get_last_error(&archive);
				LogMessageA(LL_ERROR, "[%s] mz_zip_reader_file_stat failed with 0x%08lx! Archive: %s File: %s Error: %s", __FUNCTION__, error, ArchiveName, AssetFileName, mz_zip_get_error_string(error));
				goto Exit;
			}

			LogMessageA(LL_INFO, "[%s] File %s found in asset file %s and extracted to heap.", __FUNCTION__, AssetFileName, ArchiveName);

			break;
		}

	}

	if (file_found_in_archive == FALSE)
	{
		error = ERROR_FILE_NOT_FOUND;
		LogMessageA(LL_ERROR, "[%s] File %s was not found in archive %s! 0x%08lx", __FUNCTION__, AssetFileName, ArchiveName, error);
		goto Exit;
	}

	switch (ResourceType)
	{
		case RT_WAV:
		{
			error = LoadWavFromMemory(decompressed_buffer, Resource);
			break;
		}
		case RT_OGG:
		{
			error = LoadOggFromMemory(decompressed_buffer, decompressed_size, Resource);
			break;
		}
		case RT_TILEMAP:
		{
			error = LoadTileMapFromMemory(decompressed_buffer, (uint32_t)decompressed_size, Resource);
			break;
		}
		case RT_BMPX:
		{
			error = Load32BppBitmapFromMemory(decompressed_buffer, Resource);
			break;
		}
		default:
		{
			ASSERT(FALSE, "Unknown resource type!")
		}
	}

Exit:

	mz_zip_reader_end(&archive);

	return error;
}

DWORD AssetLoadingThreadProc(_In_ LPVOID Param)
{

	UNREFERENCED_PARAMETER(Param);

	DWORD error;

	if ((error = LoadAssetFromArchive(ASSET_FILE, "6x7Font.bmpx", RT_BMPX, &g_6x7_font)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading 6x7font.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "SplashScreen.wav", RT_WAV, &g_sound_splash_screen)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading SplashScreen.wav failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	SetEvent(g_essential_assets_loaded_event);

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Overworld01.bmpx", RT_BMPX, &g_overworld01.GameBitmap)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Overworld01.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Overworld01.tmx", RT_TILEMAP, &g_overworld01.TileMap)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Overworld01.tmx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "MenuNavigate.wav", RT_WAV, &g_sound_menu_navigate)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading MenuNavigate.wav failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "MenuChoose.wav", RT_WAV, &g_sound_menu_choose)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading MenuChoose.wav failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Overworld01.ogg", RT_OGG, &g_music_overworld01)) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Overworld01.ogg failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Down_Standing.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Down_Standing.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Down_Walk1.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Down_Walk1.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Down_Walk2.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Down_Walk2.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Left_Standing.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Left_Standing.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Left_Walk1.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Left_Walk1.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Left_Walk2.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Left_Walk2.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Right_Standing.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Right_Standing.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Right_Walk1.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Right_Walk1.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Right_Walk2.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Right_Walk2.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Up_Standing.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_UPWARD_0])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Up_Standing.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Up_Walk1.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_UPWARD_1])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Up_Walk1.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}
	if ((error = LoadAssetFromArchive(ASSET_FILE, "Hero_Suit0_Up_Walk2.bmpx", RT_BMPX, &g_player.Sprite[SUIT_0][FACING_UPWARD_2])) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Hero_Suit0_Up_Walk2.bmpx failed with 0x%08lx!", __FUNCTION__, error);
		goto Exit;
	}

Exit:
	return error;
}

void InitializeGlobals(void)
{
	g_current_game_state = GS_OPENING_SPLASH_SCREEN;
	g_game_is_running = TRUE;
	g_gamepad_id = -1;

#pragma warning(suppress: 4127)
	ASSERT((_countof(g_passable_tiles) == 3), "Wrong count of passable tiles!")
	g_passable_tiles[0] = TILE_GRASS_01;
	g_passable_tiles[1] = TILE_BRICK_01;
	g_passable_tiles[2] = TILE_PORTAL_01;

	g_overworld_area = (RECT){ .left = 0,		.top = 0,	.right = 3840,	.bottom = 2400 };
	g_dungeon1_area = (RECT){ .left = 3856,	.top = 0,	.right = 4240,	.bottom = 240 };

	g_current_area = g_overworld_area;

#pragma warning(suppress: 4127)
	ASSERT((_countof(g_portals) == 2), "Wrong count of portals!")

	g_portal001 = (PORTAL){
		.DestinationArea	= g_dungeon1_area,
		.CameraPos			= (UPOINT) {.X = 3856,.Y = 0},
		.ScreenDestination	= (UPOINT) {.X = 64,	.Y = 32},
		.WorldDestination	= (UPOINT) {.X = 3920,.Y = 32},
		.WorldPos			= (UPOINT) {.X = 272,	.Y = 80}
	};

	g_portal002 = (PORTAL){
		.DestinationArea	= g_overworld_area,
		.CameraPos			= (UPOINT) {.X = 0,	.Y = 0},
		.ScreenDestination	= (UPOINT) {.X = 272,	.Y = 80},
		.WorldDestination	= (UPOINT) {.X = 272,	.Y = 80},
		.WorldPos			= (UPOINT) {.X = 3920,.Y = 32}
	};

	g_portals[0] = &g_portal001;
	g_portals[1] = &g_portal002;
}

// HasPlayerMovedSincePortal is unnecessary
// ScreenDestination can be calculated by subtracting CameraPos from WorldDestination, so is unnecessary
// Can remove check for assets loaded in PPI_OpeningSplashScreen as input is locked until it is.
// put in a starting call to find gamepads before 2seconds pass.