#include "Main.h"

#include "CharacterNamingScreen.h"
#include "ExitYesNoScreen.h"
#include "GamepadUnplugged.h"
#include "OpeningSplashScreen.h"
#include "OptionsScreen.h"
#include "TitleScreen.h"

BOOL gGameIsRunning;
GAMEBITMAP gOverworld01;
UPOINT gCamera;
int gFontCharacterPixelOffset[] = {
	//	.. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
		93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	//	   !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
		94,64,87,66,67,68,70,85,72,73,71,77,88,74,91,92,52,53,54,55,56,57,58,59,60,61,86,84,89,75,90,93,
	//	@  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
		65,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,80,78,81,69,76,
	//	 `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  ..
		62,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,82,79,83,63,93,
	//	.. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
		93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	//	.. .. .. .. .. .. .. .. .. .. .. «  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. »  .. .. .. ..
		93,93,93,93,93,93,93,93,93,93,93,96,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,95,93,93,93,93,
	//	.. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
		93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
	//	.. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. F2 .. .. .. .. .. .. .. .. .. .. .. .. ..
		93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,97,93,93,93,93,93,93,93,93,93,93,93,93,93
};

BOOL gWindowHasFocus;
REGISTRYPARAMS gRegistryParams;
XINPUT_STATE gGamepadState;
IXAudio2* gXAudio;
IXAudio2MasteringVoice* gXAudioMasteringVoice;
uint8_t gSFXSourceVoiceSelector;


int _stdcall WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PreviousInstance, _In_ PSTR CommandLine, _In_ INT CmdShow)
{
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);
	MSG message = { 0 };
	int64_t FrameStart = 0;
	int64_t FrameEnd = 0;
	int64_t ElapsedMicroseconds = 0;
	int64_t ElapsedMicrosecondsAccumulatorRaw = 0;
	int64_t ElapsedMicrosecondsAccumulatorCooked = 0;
	HMODULE NtDllModuleHandle = NULL;
	FILETIME ProcessCreationTime = { 0 };
	FILETIME ProcessExitTime = { 0 };
	int64_t CurrentUserCPUTime = 0;
	int64_t CurrentKernelCPUTime = 0;
	int64_t PreviousUserCPUTime = 0;
	int64_t PreviousKernelCPUTime = 0;
	HANDLE ProcessHandle = GetCurrentProcess();
	gGamepadID = -1;

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

	if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
	{
		LogMessageA(LL_ERROR, "[%s] Couldn't load ntdll.dll! Error 0x%081x!", __FUNCTION__, GetLastError());
		MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
	{
		LogMessageA(LL_ERROR, "[%s] Couldn't find the NtQueryTimerResolution function in ntdll.dll! GetProcAddress failed! Error 0x%081x!", __FUNCTION__, GetLastError());
		MessageBoxA(NULL, "Couldn't find the NtQueryTimerResolution function in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	NtQueryTimerResolution(&gPerformanceData.MinimumTimerResolution, &gPerformanceData.MaximumTimerResolution, &gPerformanceData.CurrentTimerResolution);
	GetSystemInfo(&gPerformanceData.SystemInfo);
	LogMessageA(LL_INFO, "[%s] Number of CPUs: %d", __FUNCTION__, gPerformanceData.SystemInfo.dwNumberOfProcessors);
	switch (gPerformanceData.SystemInfo.wProcessorArchitecture)
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
			LogMessageA(LL_INFO, "[%s] CPU Architecture: Unknown", __FUNCTION__);
		}
	}

	GetSystemTimeAsFileTime((LPFILETIME)&gPerformanceData.PreviousSystemTime);

	if (timeBeginPeriod(1) == TIMERR_NOCANDO)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set global timer resolution!", __FUNCTION__);
		MessageBoxA(NULL, "Failed to set global timer resolution!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] System Timer Resolution: Min %d Max %d Current %d",
		__FUNCTION__,
		gPerformanceData.MinimumTimerResolution,
		gPerformanceData.MaximumTimerResolution,
		gPerformanceData.CurrentTimerResolution);

	if (SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS) == 0)
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
		goto Exit;
	}

	if (Load32BbpBitmapFromFile(".\\Assets\\6x7Font.bmpx", &g6x7Font) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading 6x7Font.bmpx failed!", __FUNCTION__);
		MessageBox(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (Load32BbpBitmapFromFile(".\\Assets\\Maps\\Overworld01.bmpx", &gOverworld01) != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Loading Overworld01.bmpx failed!", __FUNCTION__);
		MessageBox(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}
	
	if (InitializeSoundEngine() != S_OK)
	{
		MessageBox(NULL, "InitializeSoundEngine failed", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (LoadWavFromFile(".\\Assets\\SplashScreen.wav", &gSoundSplashScreen) != ERROR_SUCCESS)
	{
		MessageBox(NULL, "LoadWaveFromFile.wav failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (LoadWavFromFile(".\\Assets\\MenuNavigate.wav", &gSoundMenuNavigate) != ERROR_SUCCESS)
	{
		MessageBox(NULL, "LoadWaveFromFile.wav failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	if (LoadWavFromFile(".\\Assets\\MenuChoose.wav", &gSoundMenuChoose) != ERROR_SUCCESS)
	{
		MessageBox(NULL, "LoadWaveFromFile.wav failed!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	QueryPerformanceFrequency((LARGE_INTEGER*)&gPerformanceData.PerfFrequency);
	//gPerformanceData.DisplayDebugInfo = TRUE;

	gBackBuffer.BitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.BitmapInfo.bmiHeader);
	gBackBuffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
	gBackBuffer.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
	gBackBuffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	gBackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	gBackBuffer.BitmapInfo.bmiHeader.biPlanes = 1;
	gBackBuffer.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (gBackBuffer.Memory == NULL)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to allocate memory for drawing surface! Error 0x%081x!", __FUNCTION__, GetLastError());
		MessageBox(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONERROR | MB_OK);
		goto Exit;
	}

	memset(gBackBuffer.Memory, 0x7F, GAME_DRAWING_AREA_MEMORY_SIZE);

	if (InitializeHero() != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to initialize hero!", __FUNCTION__);
		MessageBox(NULL, "Failed to initialize hero!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	gGameIsRunning = TRUE;

	while (gGameIsRunning)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&FrameStart);
		while (PeekMessageA(&message, gGameWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}

		ProcessPlayerInput();
		RenderFrameGraphics();

		QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);
		ElapsedMicroseconds = FrameEnd - FrameStart;
		ElapsedMicroseconds *= 1000000;
		ElapsedMicroseconds /= gPerformanceData.PerfFrequency;
		gPerformanceData.TotalFramesRendered++;
		ElapsedMicrosecondsAccumulatorRaw += ElapsedMicroseconds;

		while (ElapsedMicroseconds < TARGET_MICROSECONDS_PER_FRAME)
		{
			ElapsedMicroseconds = FrameEnd - FrameStart;
			ElapsedMicroseconds *= 1000000;
			ElapsedMicroseconds /= gPerformanceData.PerfFrequency;
			QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);

			if (ElapsedMicroseconds < (TARGET_MICROSECONDS_PER_FRAME * 0.75f))
			{
				Sleep(1);
			}
		}

		ElapsedMicrosecondsAccumulatorCooked += ElapsedMicroseconds;

		if (gPerformanceData.TotalFramesRendered % CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES == 0)
		{
			GetSystemTimeAsFileTime((LPFILETIME)&gPerformanceData.CurrentSystemTime);

			FindFirstConnectedGamepad();

			GetProcessTimes(ProcessHandle,
				&ProcessCreationTime,
				&ProcessExitTime,
				(FILETIME*)&CurrentKernelCPUTime,
				(FILETIME*)&CurrentUserCPUTime);

			gPerformanceData.CPUPercent = (double)(CurrentKernelCPUTime - PreviousKernelCPUTime) + (double)(CurrentUserCPUTime - PreviousUserCPUTime);
			gPerformanceData.CPUPercent /= (gPerformanceData.CurrentSystemTime - gPerformanceData.PreviousSystemTime);
			gPerformanceData.CPUPercent /= gPerformanceData.SystemInfo.dwNumberOfProcessors;
			gPerformanceData.CPUPercent *= 100;

			GetProcessHandleCount(ProcessHandle, &gPerformanceData.HandleCount);
			K32GetProcessMemoryInfo(ProcessHandle, (PROCESS_MEMORY_COUNTERS*)&gPerformanceData.MemInfo, sizeof(gPerformanceData.MemInfo));

			gPerformanceData.RawFPSAverage = 1.0f / ((ElapsedMicrosecondsAccumulatorRaw / CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES) * 0.000001f);
			gPerformanceData.CookedFPSAverage = 1.0f / ((ElapsedMicrosecondsAccumulatorCooked / CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES) * 0.000001f);

			ElapsedMicrosecondsAccumulatorRaw = 0;
			ElapsedMicrosecondsAccumulatorCooked = 0;

			PreviousKernelCPUTime = CurrentKernelCPUTime;
			PreviousUserCPUTime = CurrentUserCPUTime;
			gPerformanceData.PreviousSystemTime = gPerformanceData.CurrentSystemTime;

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
			gGameIsRunning = FALSE;
			PostQuitMessage(0);
			break;
		}

		case WM_ACTIVATE:
		{
			if (WParam == 0)
			{
				// Our window has lost focus
				gWindowHasFocus = FALSE;
			}
			else
			{
				// Our window has gained focus
				ShowCursor(FALSE);
				gWindowHasFocus = TRUE;
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

	WNDCLASSEXA windowClass = { 0 };

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

	gGameWindow = CreateWindowExA(0, "GAME_B_WINDOWCLASS", "Window Title",
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL,
		GetModuleHandleA(NULL), NULL);

	if (gGameWindow == NULL)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] CreateWindowExA Failed! Error 0x%081x!", __FUNCTION__, result);
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	gPerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gPerformanceData.MonitorInfo) == 0)
	{
		result = ERROR_MONITOR_NO_DESCRIPTOR;
		LogMessageA(LL_ERROR, "[%s] GetMonitorInfoA(MonitorFromWindow()) failed! Error 0x%081x!", __FUNCTION__, result);
		goto Exit;
	}

	//gPerformanceData.MonitorSize.x = gPerformanceData.MonitorInfo.rcMonitor.right - gPerformanceData.MonitorInfo.rcMonitor.left;
	//gPerformanceData.MonitorSize.y = gPerformanceData.MonitorInfo.rcMonitor.bottom - gPerformanceData.MonitorInfo.rcMonitor.top;

	//if (gPerformanceData.WindowWidth == 0)
	//{
	//	gPerformanceData.WindowWidth = gPerformanceData.MonitorWidth;
	//}
	//if (gPerformanceData.WindowHeight == 0)
	//{
	//	gPerformanceData.WindowHeight = gPerformanceData.MonitorHeight;
	//}


	//if (gPerformanceData.WindowHeight > gPerformanceData.MonitorHeight ||
	//	gPerformanceData.WindowWidth  > gPerformanceData.MonitorWidth)
	//{
	//	LogMessageA(LL_INFO, "[%s] The WindowWidth or WindowHeight retrieved from the registry was larger than the current monitor size.  Resetting to default.", __FUNCTION__);
	//	gPerformanceData.WindowHeight = gPerformanceData.MonitorHeight;
	//	gPerformanceData.WindowWidth = gPerformanceData.MonitorWidth;
	//}

	for (uint8_t Counter = 1; Counter < 12; Counter++)
	{
		if ((GAME_RES_WIDTH * Counter > (gPerformanceData.MonitorInfo.rcMonitor.right - gPerformanceData.MonitorInfo.rcMonitor.left)) ||
			(GAME_RES_HEIGHT * Counter > (gPerformanceData.MonitorInfo.rcMonitor.bottom - gPerformanceData.MonitorInfo.rcMonitor.top)))
		{
			gPerformanceData.MaxScaleFactor = Counter - 1;
			break;
		}
	}

	if (gRegistryParams.ScaleFactor == 0)
	{
		gPerformanceData.CurrentScaleFactor = gPerformanceData.MaxScaleFactor;
	}
	else
	{
		gPerformanceData.CurrentScaleFactor = (uint8_t)gRegistryParams.ScaleFactor;
	}

	LogMessageA(LL_INFO, "[%s] Current scale factor is %d. Max scale factor is %d. ",
		__FUNCTION__,
		gPerformanceData.CurrentScaleFactor,
		gPerformanceData.MaxScaleFactor);

	LogMessageA(LL_INFO, "[%s] Will draw at %dx%d.",
		__FUNCTION__,
		GAME_RES_WIDTH * gPerformanceData.CurrentScaleFactor,
		GAME_RES_HEIGHT * gPerformanceData.CurrentScaleFactor);


	if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, WS_VISIBLE) == 0)
	{
		result = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetWindowLongPtrA failed! Error 0x%081x!", __FUNCTION__, result);
		goto Exit;
	}

	if (SetWindowPos(gGameWindow,
		HWND_TOP,
		gPerformanceData.MonitorInfo.rcMonitor.left,
		gPerformanceData.MonitorInfo.rcMonitor.top,
		gPerformanceData.MonitorInfo.rcMonitor.right - gPerformanceData.MonitorInfo.rcMonitor.left,
		gPerformanceData.MonitorInfo.rcMonitor.bottom - gPerformanceData.MonitorInfo.rcMonitor.top,
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
	HANDLE mutex = NULL;
	mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return TRUE;
	}
	return FALSE;
}

void ProcessPlayerInput(void)
{

	if (gWindowHasFocus == FALSE)
	{
		return;
	}

	gGameInput.EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
	gGameInput.DebugKeyIsDown = GetAsyncKeyState(VK_F1);
	gGameInput.LeftKeyIsDown = GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A');
	gGameInput.RightKeyIsDown = GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D');
	gGameInput.UpKeyIsDown = GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W');
	gGameInput.DownKeyIsDown = GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S');
	gGameInput.ChooseKeyIsDown = GetAsyncKeyState(VK_RETURN);

	if (gGamepadID > -1)
	{
		if (XInputGetState(gGamepadID, &gGamepadState) == ERROR_SUCCESS)
		{
			gGameInput.EscapeKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
			gGameInput.LeftKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			gGameInput.RightKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			gGameInput.UpKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
			gGameInput.DownKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
			gGameInput.ChooseKeyIsDown |= (gGamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_A);
		}
		else
		{
			// Gamepad unplugged?
			gGamepadID = -1;
			gPreviousGameState = gCurrentGameState;
			gCurrentGameState = GS_GAMEPADUNPLUGGED;
		}
	}

	if (PRESSED_DEBUG)
	{
		gPerformanceData.DisplayDebugInfo = !gPerformanceData.DisplayDebugInfo;
	}

	switch (gCurrentGameState)
	{
		case GS_OPENINGSPLASHSCREEN:
		{
			PPI_OpeningSplashScreen();
			break;
		}
		case GS_GAMEPADUNPLUGGED:
		{
			PPI_GamepadUnplugged();
			break;
		}
		case GS_TITLESCREEN:
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
		case GS_CHARACTERNAMING:
		{
			PPI_CharacterNaming();
			break;
		}
		case GS_OPTIONSSCREEN:
		{
			PPI_OptionsScreen();
			break;
		}
		case GS_EXITYESNOSCREEN:
		{
			PPI_ExitYesNo();
			break;
		}

		default:
		{
			ASSERT(FALSE, "Unknown game state!");
		}
	}

	gGameInput.DebugKeyWasDown = gGameInput.DebugKeyIsDown;
	gGameInput.LeftKeyWasDown = gGameInput.LeftKeyIsDown;
	gGameInput.RightKeyWasDown = gGameInput.RightKeyIsDown;
	gGameInput.UpKeyWasDown = gGameInput.UpKeyIsDown;
	gGameInput.DownKeyWasDown = gGameInput.DownKeyIsDown;
	gGameInput.ChooseKeyWasDown = gGameInput.ChooseKeyIsDown;
	gGameInput.EscapeKeyWasDown = gGameInput.EscapeKeyIsDown;
}

DWORD Load32BbpBitmapFromFile(_In_ char* Filename, _Inout_ GAMEBITMAP* GameBitmap)
{
	DWORD Error = ERROR_SUCCESS;
	HANDLE FileHandle = INVALID_HANDLE_VALUE;
	WORD BitmapHeader = 0;
	DWORD PixelDataOffset = 0;
	DWORD NumberOfBytesRead = 2;

	if ((FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (ReadFile(FileHandle, &BitmapHeader, 2, &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (BitmapHeader != 0x4D42) // "BM" Backwards
	{
		Error = ERROR_FILE_INVALID;
		goto Exit;
	}

	if (SetFilePointer(FileHandle, 0xA, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (ReadFile(FileHandle, &PixelDataOffset, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (SetFilePointer(FileHandle, 0xE, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (ReadFile(FileHandle, &GameBitmap->BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		goto Exit;
	}

	if ((GameBitmap->Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GameBitmap->BitmapInfo.bmiHeader.biSizeImage)) == NULL)
	{
		Error = ERROR_NOT_ENOUGH_MEMORY;
		goto Exit;
	}

	if (SetFilePointer(FileHandle, PixelDataOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		goto Exit;
	}

	if (ReadFile(FileHandle, GameBitmap->Memory, GameBitmap->BitmapInfo.bmiHeader.biSizeImage, &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		goto Exit;
	}

Exit:

	if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(FileHandle);
	}

	if (Error == ERROR_SUCCESS)
	{
		LogMessageA(LL_INFO, "[%s] Loading successful: %s", __FUNCTION__, Filename);
	}
	else
	{
		LogMessageA(LL_ERROR, "[%s] Loading failed: %s!  Error 0x%081x!", __FUNCTION__, Filename, Error);
	}

	return Error;
}

DWORD InitializeHero(void) {
	DWORD Error = ERROR_SUCCESS;
	gPlayer.ScreenPos.X = 192;
	gPlayer.ScreenPos.Y = 64;
	gPlayer.CurrentArmor = SUIT_0;
	gPlayer.Direction = DOWN;

	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Down_Standing.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Down_Walk1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Down_Walk2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Left_Standing.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Left_Walk1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Left_Walk2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Right_Standing.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Right_Walk1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Right_Walk2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Up_Standing.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UPWARD_0])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Up_Walk1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UPWARD_1])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	if ((Error = Load32BbpBitmapFromFile(".\\Assets\\Hero_Suit0_Up_Walk2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UPWARD_2])) != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Load32BbpBitmapFromFile failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

Exit:
	return Error;
}

void BlitStringToBuffer(_In_ char* String, _In_ GAMEBITMAP* FontSheet, _In_ PIXEL32* Color, _In_ uint16_t x, _In_ uint16_t y)
{
	uint16_t CharWidth = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biWidth / FONT_SHEET_CHARACTERS_PER_ROW;
	uint16_t CharHeight = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biHeight;
	uint16_t BytesPerCharacter = (CharWidth * CharHeight * (FontSheet->BitmapInfo.bmiHeader.biBitCount / 8));
	uint16_t StringLength = (uint16_t)strlen(String);
	GAMEBITMAP StringBitmap = { 0 };
	StringBitmap.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	StringBitmap.BitmapInfo.bmiHeader.biHeight = CharHeight;
	StringBitmap.BitmapInfo.bmiHeader.biWidth = CharWidth * StringLength;
	StringBitmap.BitmapInfo.bmiHeader.biPlanes = 1;
	StringBitmap.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	StringBitmap.Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (size_t)BytesPerCharacter * (size_t)StringLength);

	for (int Character = 0; Character < StringLength; Character++)
	{
		int StartingFontSheetPixel = 0;
		int FontSheetOffset = 0;
		int StringBitmapOffst = 0;
		PIXEL32 FontSheetPixel = { 0 };
		StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) -
			FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * gFontCharacterPixelOffset[(uint8_t)String[Character]]);

		for (int yPixel = 0; yPixel <= CharHeight - 1; yPixel++)
		{
			for (int xPixel = 0; xPixel < CharWidth - 1; xPixel++)
			{
				FontSheetOffset = StartingFontSheetPixel + xPixel - (FontSheet->BitmapInfo.bmiHeader.biWidth * yPixel);
				StringBitmapOffst = (Character * CharWidth) + ((StringBitmap.BitmapInfo.bmiHeader.biWidth * StringBitmap.BitmapInfo.bmiHeader.biHeight) -
					StringBitmap.BitmapInfo.bmiHeader.biWidth) + xPixel - (StringBitmap.BitmapInfo.bmiHeader.biWidth * yPixel);

				memcpy_s(&FontSheetPixel, sizeof(PIXEL32), (PIXEL32*)FontSheet->Memory + FontSheetOffset, sizeof(PIXEL32));

				FontSheetPixel.Red = Color->Red;
				FontSheetPixel.Green = Color->Green;
				FontSheetPixel.Blue = Color->Blue;

				memcpy_s((PIXEL32*)StringBitmap.Memory + StringBitmapOffst, sizeof(PIXEL32), &FontSheetPixel, sizeof(PIXEL32));
			}
		}
	}

	Blit32BppBitmapToBuffer(&StringBitmap, x, y);

	if (StringBitmap.Memory)
	{
		HeapFree(GetProcessHeap(), 0, StringBitmap.Memory);
	}
}

void RenderFrameGraphics(void)
{

	switch (gCurrentGameState)
	{

		case GS_OPENINGSPLASHSCREEN:
		{
			DrawOpeningSplashScreen();
			break;
		}

		case GS_TITLESCREEN:
		{
			DrawTitleScreen();
			break;
		}

		case GS_CHARACTERNAMING:
		{
			DrawCharacterNamingScreen();
			break;
		}

		case GS_GAMEPADUNPLUGGED:
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

		case GS_OPTIONSSCREEN:
		{
			DrawOptionsScreen();
			break;
		}

		case GS_EXITYESNOSCREEN:
		{
			DrawExitYesNoScreen();
			break;
		}

		default:
		{
			ASSERT(FALSE, "GameState not implemented");
		}
	}

	if (gPerformanceData.DisplayDebugInfo == TRUE)
	{
		DrawDebugInfo();
	}

	HDC DeviceContext = GetDC(gGameWindow);

	StretchDIBits(DeviceContext,
		((gPerformanceData.MonitorInfo.rcMonitor.right - gPerformanceData.MonitorInfo.rcMonitor.left) / 2) - ((GAME_RES_WIDTH * gPerformanceData.CurrentScaleFactor) / 2),
		((gPerformanceData.MonitorInfo.rcMonitor.bottom - gPerformanceData.MonitorInfo.rcMonitor.top) / 2) - ((GAME_RES_HEIGHT * gPerformanceData.CurrentScaleFactor) / 2),
		(GAME_RES_WIDTH * gPerformanceData.CurrentScaleFactor),
		(GAME_RES_HEIGHT * gPerformanceData.CurrentScaleFactor),
		0,
		0,
		GAME_RES_WIDTH,
		GAME_RES_HEIGHT,
		gBackBuffer.Memory,
		&gBackBuffer.BitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	ReleaseDC(gGameWindow, DeviceContext);
}

#ifdef AVX
_forceinline void ClearScreen(_In_ __m256i* Color)
{
	for (int index = 0; index < (GAME_RES_WIDTH * GAME_RES_HEIGHT) / 8; index++)
	{
		_mm256_store_si256((__m256i*)gBackBuffer.Memory + index, *Color);
	}
}
#elif defined SSE2
_forceinline void ClearScreen(_In_ __m128i* Color)
{

	for (int index = 0; index < (GAME_RES_WIDTH * GAME_RES_HEIGHT) / 4; index++)
	{
		_mm_store_si128((__m128i*)gBackBuffer.Memory + index, *Color);
	}

}
#else
__forceinline void ClearScreen(_In_ PIXEL32* Pixel)
{
	for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x++)
	{
		memcpy((PIXEL32*)gBackBuffer.Memory + x, Pixel, sizeof(PIXEL32));
	}
}
#endif

void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ uint16_t x, _In_ uint16_t y)
{

	int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;

	int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight)
		- GameBitmap->BitmapInfo.bmiHeader.biWidth);

	int32_t MemoryOffset = 0;
	int32_t BitmapOffset = 0;
	PIXEL32 BitmapPixel = { 0 };

	for (int16_t yPixel = 0; yPixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; yPixel++)
	{
		for (int16_t xPixel = 0; xPixel < GameBitmap->BitmapInfo.bmiHeader.biWidth; xPixel++)
		{
			MemoryOffset = StartingScreenPixel + xPixel - (GAME_RES_WIDTH * yPixel);
			BitmapOffset = StartingBitmapPixel + xPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * yPixel);
			memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));
			if (BitmapPixel.Alpha == 255)
			{
				memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));
			}
		}
	}

}

void BlitTilemapToBuffer(_In_ GAMEBITMAP* GameBitmap)
{
	int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH);

	int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight)
		- GameBitmap->BitmapInfo.bmiHeader.biWidth) + gCamera.X - (GameBitmap->BitmapInfo.bmiHeader.biWidth * gCamera.Y);

	int32_t MemoryOffset = 0;
	int32_t BitmapOffset = 0;
	
#ifdef AVX
	__m256i BitmapOctoPixel;
	for (int16_t yPixel = 0; yPixel < GAME_RES_HEIGHT; yPixel++)
	{
		for (int16_t xPixel = 0; xPixel < GAME_RES_WIDTH; xPixel += 8)
		{
			MemoryOffset = StartingScreenPixel + xPixel - (GAME_RES_WIDTH * yPixel);
			BitmapOffset = StartingBitmapPixel + xPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * yPixel);
			BitmapOctoPixel = _mm256_loadu_si256((const __m256i*)((PIXEL32*)gOverworld01.Memory + BitmapOffset));
			_mm256_store_si256((__m256i*)((PIXEL32*)gBackBuffer.Memory + MemoryOffset), BitmapOctoPixel);
		}
	}
#elif defined SSE2
	__m128i BitmapQuadPixel;
	for (int16_t yPixel = 0; yPixel < GAME_RES_HEIGHT; yPixel++)
	{
		for (int16_t xPixel = 0; xPixel < GAME_RES_WIDTH; xPixel += 4)
		{
			MemoryOffset = StartingScreenPixel + xPixel - (GAME_RES_WIDTH * yPixel);
			BitmapOffset = StartingBitmapPixel + xPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * yPixel);
			BitmapQuadPixel = _mm_load_si128((const __m128i*)((PIXEL32*)gOverworld01.Memory + BitmapOffset));
			_mm_store_si128((__m128i*)((PIXEL32*)gBackBuffer.Memory + MemoryOffset), BitmapQuadPixel);
		}
	}
#else
	PIXEL32 BitmapPixel = { 0 };
	for (int16_t yPixel = 0; yPixel < GAME_RES_HEIGHT; yPixel++)
	{
		for (int16_t xPixel = 0; xPixel < GAME_RES_WIDTH; xPixel++)
		{
			MemoryOffset = StartingScreenPixel + xPixel - (GAME_RES_WIDTH * yPixel);
			BitmapOffset = StartingBitmapPixel + xPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * yPixel);
			memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));
			memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));
		}
	}
#endif
}

DWORD LoadRegistryParameters(void)
{
	DWORD Result = ERROR_SUCCESS;
	HKEY RegKey = NULL;
	DWORD RegDisposition = 0;
	DWORD RegBytesRead = sizeof(DWORD);
	Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);

	if (Result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	if (RegDisposition == REG_CREATED_NEW_KEY)
	{
		LogMessageA(LL_INFO, "[%s] Registry key did not exist; created new key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
	}
	else
	{
		LogMessageA(LL_INFO, "[%s] Opened existing registry key HKCU\\SOFTWARE\\%s", __FUNCTION__, GAME_NAME);
	}

	Result = RegGetValueA(RegKey, NULL, "LogLevel", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.LogLevel, &RegBytesRead);
	if (Result != ERROR_SUCCESS)
	{
		if (Result == ERROR_FILE_NOT_FOUND)
		{
			Result = ERROR_SUCCESS;
			LogMessageA(LL_INFO, "[%s] Registry value 'LogLevel' not found. Using default of 0. (LOG_LEVEL_NONE)", __FUNCTION__);
			gRegistryParams.LogLevel = LL_NONE;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'LogLevel' registry value. Error 0x%08lx!", __FUNCTION__, Result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] LogLevel is %d.", __FUNCTION__, gRegistryParams.LogLevel);

	Result = RegGetValueA(RegKey, NULL, "ScaleFactor", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.ScaleFactor, &RegBytesRead);
	if (Result != ERROR_SUCCESS)
	{
		if (Result == ERROR_FILE_NOT_FOUND)
		{
			Result = ERROR_SUCCESS;
			LogMessageA(LL_INFO, "[%s] Registry value 'ScaleFactor' not found. Using default of 0.", __FUNCTION__);
			gRegistryParams.ScaleFactor = 0;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'ScaleFactor' registry value. Error 0x%08lx!", __FUNCTION__, Result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] ScaleFactor is %d.", __FUNCTION__, gRegistryParams.ScaleFactor);

	Result = RegGetValueA(RegKey, NULL, "SFXVolume", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.SFXVolume, &RegBytesRead);
	if (Result != ERROR_SUCCESS)
	{
		if (Result == ERROR_FILE_NOT_FOUND)
		{
			Result = ERROR_SUCCESS;
			LogMessageA(LL_INFO, "[%s] Registry value 'SFXVolume' not found. Using default of 0.5", __FUNCTION__);
			gRegistryParams.SFXVolume = 50;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'SFXVolume' registry value. Error 0x%08lx!", __FUNCTION__, Result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] SFXVolume is %.1f.", __FUNCTION__, (float)(gRegistryParams.SFXVolume / 100.0f));
	gSFXVolume = (float)(gRegistryParams.SFXVolume / 100.0f);

	Result = RegGetValueA(RegKey, NULL, "MusicVolume", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.MusicVolume, &RegBytesRead);
	if (Result != ERROR_SUCCESS)
	{
		if (Result == ERROR_FILE_NOT_FOUND)
		{
			Result = ERROR_SUCCESS;
			LogMessageA(LL_INFO, "[%s] Registry value 'MusicVolume' not found. Using default of 0.5", __FUNCTION__);
			gRegistryParams.MusicVolume = 50;
		}
		else
		{
			LogMessageA(LL_ERROR, "[%s] Failed to read the 'MusicVolume' registry value. Error 0x%08lx!", __FUNCTION__, Result);
			goto Exit;
		}
	}
	LogMessageA(LL_INFO, "[%s] MusicVolume is %.1f.", __FUNCTION__, (float)(gRegistryParams.MusicVolume / 100.0f));
	gMusicVolume = (float)(gRegistryParams.MusicVolume / 100.0f);

Exit:

	if (RegKey)
	{
		RegCloseKey(RegKey);
	}
	return Result;
}

DWORD SaveRegistryParameters(void)
{
	DWORD Result = ERROR_SUCCESS;
	HKEY RegKey = NULL;
	DWORD RegDisposition = 0;
	DWORD SFXVolume = (DWORD)(gSFXVolume * 100.0f);
	DWORD MusicVolume = (DWORD)(gMusicVolume * 100.0f);
	Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);

	if (Result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] Reg key open for save. ", __FUNCTION__);

	Result = RegSetValueExA(RegKey, "SFXVolume", 0, REG_DWORD, (const BYTE*)&SFXVolume, sizeof(DWORD));
	if (Result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'SFXVolume' in registry. Error code 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] SFXVolume saved %d. ", __FUNCTION__, SFXVolume);

	Result = RegSetValueExA(RegKey, "MusicVolume", 0, REG_DWORD, (const BYTE*)&MusicVolume, sizeof(DWORD));
	if (Result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'MusicVolume' in registry. Error code 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] MusicVolume saved %d. ", __FUNCTION__, MusicVolume);

	Result = RegSetValueExA(RegKey, "ScaleFactor", 0, REG_DWORD, &gPerformanceData.CurrentScaleFactor, sizeof(DWORD));
	if (Result != ERROR_SUCCESS)
	{
		LogMessageA(LL_ERROR, "[%s] Failed to set 'ScaleFactor' in registry. Error code 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	LogMessageA(LL_INFO, "[%s] ScaleFactor saved %d. ", __FUNCTION__, gPerformanceData.CurrentScaleFactor);



Exit:
	if (RegKey)
	{
		RegCloseKey(RegKey);
	}
	return Result;
}

void LogMessageA(_In_ LOGLEVEL LogLevel, _In_ char* Message, _In_ ...)
{
	size_t MessageLength = strlen(Message);
	SYSTEMTIME Time = { 0 };
	HANDLE LogFileHandle = INVALID_HANDLE_VALUE;
	DWORD EndOfFile = 0;
	DWORD NumberOfBytesWritten = 0;
	char DateTimeString[96] = { 0 };
	char SeverityString[8] = { 0 };
	char FormattedString[4096] = { 0 };
	if ((LOGLEVEL)gRegistryParams.LogLevel < LogLevel)
	{
		return;
	}

	if (MessageLength < 1 || MessageLength > 4096)
	{
		ASSERT(FALSE, "Message was either too short or too long!");
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
			strcpy_s(SeverityString, sizeof(SeverityString), "[INFO] ");
			break;
		}
		case LL_WARNING:
		{
			strcpy_s(SeverityString, sizeof(SeverityString), "[WARN] ");
			break;
		}
		case LL_ERROR:
		{
			strcpy_s(SeverityString, sizeof(SeverityString), "[ERROR]");
			break;
		}
		case LL_DEBUG:
		{
			strcpy_s(SeverityString, sizeof(SeverityString), "[DEBUG]");
			break;
		}
		default:
		{
			ASSERT(FALSE, "LogLevel was unrecognized.");
		}
	}

	GetLocalTime(&Time);

	va_list ArgPointer = NULL;
	va_start(ArgPointer, Message);
	_vsnprintf_s(FormattedString, sizeof(FormattedString), _TRUNCATE, Message, ArgPointer);
	va_end(ArgPointer);

	_snprintf_s(DateTimeString, sizeof(DateTimeString), _TRUNCATE, "\r\n[%02u/%02u/%u %02u:%02u:%02u.%03u]", Time.wMonth, Time.wDay, Time.wYear, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);

	if ((LogFileHandle = CreateFileA(LOG_FILE_NAME, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE, "Failed to access log file!");
		return;
	}

	EndOfFile = SetFilePointer(LogFileHandle, 0, NULL, FILE_END);

	WriteFile(LogFileHandle, DateTimeString, (DWORD)strlen(DateTimeString), &NumberOfBytesWritten, NULL);
	WriteFile(LogFileHandle, SeverityString, (DWORD)strlen(SeverityString), &NumberOfBytesWritten, NULL);
	WriteFile(LogFileHandle, FormattedString, (DWORD)strlen(FormattedString), &NumberOfBytesWritten, NULL);
	if (LogFileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(LogFileHandle);
	}
}

void DrawDebugInfo(void)
{
	char DebugTextBuffer[64] = { 0 };
	PIXEL32 White = { 0xFF,0xFF, 0xFF, 0xFF };
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPSRaw:  %.01f", gPerformanceData.RawFPSAverage);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 0);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPSCookd:%.01f", gPerformanceData.CookedFPSAverage);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 8);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "MinTimer:%.02f", gPerformanceData.MinimumTimerResolution / 10000.0f);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 16);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "MaxTimer:%.02f", gPerformanceData.MaximumTimerResolution / 10000.0f);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 24);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CurTimer:%.02f", gPerformanceData.CurrentTimerResolution / 10000.0f);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 32);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Handles: %lu", gPerformanceData.HandleCount);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 40);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Memory:  %i KB", (int)(gPerformanceData.MemInfo.PrivateUsage / 1024));
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 48);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU:     %.02f%%", gPerformanceData.CPUPercent);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 56);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FramesT: %llu", gPerformanceData.TotalFramesRendered);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 64);
	sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "ScreenXY:%d,%d", (short)gPlayer.ScreenPos.X, (short)gPlayer.ScreenPos.Y);
	BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 72);
}

void FindFirstConnectedGamepad(void)
{
	gGamepadID = -1;
	for (int8_t GamepadIndex = 0; GamepadIndex < XUSER_MAX_COUNT && gGamepadID == -1; GamepadIndex++)
	{
		XINPUT_STATE State = { 0 };
		if (XInputGetState(GamepadIndex, &State) == ERROR_SUCCESS)
		{
			gGamepadID = GamepadIndex;
		}
	}
}



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

	//memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);

	BlitTilemapToBuffer(&gOverworld01);

	//Blit32BppBitmapToBuffer(&gOverworld01, 0, 0);
	
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

	if (PRESSED_ESCAPE)
	{
		gPreviousGameState = gCurrentGameState;
		gCurrentGameState = GS_TITLESCREEN;
	}

}

HRESULT InitializeSoundEngine(void)
{
	HRESULT Result = S_OK;
	WAVEFORMATEX SfxWaveFormat = { 0 };
	WAVEFORMATEX MusicWaveFormat = { 0 };

	Result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (Result != S_OK)
	{
		LogMessageA(LL_ERROR, "[%s] CoInitializeEx failed with 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	Result = XAudio2Create(&gXAudio, 0, XAUDIO2_ANY_PROCESSOR);

	if (FAILED(Result))
	{
		LogMessageA(LL_ERROR, "[%s] XAudio2Create failed with 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	Result = gXAudio->lpVtbl->CreateMasteringVoice(gXAudio, &gXAudioMasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0);
	if (FAILED(Result))
	{
		LogMessageA(LL_ERROR, "[%s] CreateMasteringVoice failed with 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}

	SfxWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	SfxWaveFormat.nChannels = 1;
	SfxWaveFormat.nSamplesPerSec = 44100;
	SfxWaveFormat.nAvgBytesPerSec = SfxWaveFormat.nSamplesPerSec * SfxWaveFormat.nChannels * 2;
	SfxWaveFormat.nBlockAlign = SfxWaveFormat.nChannels * 2;
	SfxWaveFormat.wBitsPerSample = 16;
	SfxWaveFormat.cbSize = 0x6164;

	for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
	{
		Result = gXAudio->lpVtbl->CreateSourceVoice(gXAudio, &gXAudioSFXSourceVoice[Counter], &SfxWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
		if (Result != S_OK)
		{
			LogMessageA(LL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, Result);
			goto Exit;
		}
		gXAudioSFXSourceVoice[Counter]->lpVtbl->SetVolume(gXAudioSFXSourceVoice[Counter], gSFXVolume, XAUDIO2_COMMIT_NOW);
	}

	MusicWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	MusicWaveFormat.nChannels = 2;
	MusicWaveFormat.nSamplesPerSec = 44100;
	MusicWaveFormat.nAvgBytesPerSec = MusicWaveFormat.nSamplesPerSec * MusicWaveFormat.nChannels * 2;
	MusicWaveFormat.nBlockAlign = MusicWaveFormat.nChannels * 2;
	MusicWaveFormat.wBitsPerSample = 16;
	MusicWaveFormat.cbSize = 0;

	Result = gXAudio->lpVtbl->CreateSourceVoice(gXAudio, &gXAudioMusicSourceVoice, &MusicWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
	if (Result != S_OK)
	{
		LogMessageA(LL_ERROR, "[%s] CreateSourceVoice failed with 0x%08lx!", __FUNCTION__, Result);
		goto Exit;
	}
	gXAudioMusicSourceVoice->lpVtbl->SetVolume(gXAudioMusicSourceVoice, gMusicVolume, XAUDIO2_COMMIT_NOW);

Exit:
	return Result;
}

DWORD LoadWavFromFile(_In_ char* Filename, _Inout_ GAMESOUND* GameSound)
{
	DWORD Error = ERROR_SUCCESS;
	DWORD NumberOfBytesRead = 0;
	DWORD RIFF = 0;
	uint16_t DataChunkOffset = 0;
	DWORD DataChunkSearcher = 0;
	BOOL DataChunkFound = FALSE;
	DWORD DataChunkSize = 0;
	HANDLE FileHandle = INVALID_HANDLE_VALUE;
	void* AudioData = NULL;

	if ((FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (ReadFile(FileHandle, &RIFF, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (RIFF != 0x46464952) // "RIFF" Backwards
	{
		Error = ERROR_FILE_INVALID;
		LogMessageA(LL_ERROR, "[%s] First four bytes of this file are not 'RIFF'", __FUNCTION__);
		goto Exit;
	}

	if (SetFilePointer(FileHandle, 20, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (ReadFile(FileHandle, &GameSound->WaveFormat, sizeof(WAVEFORMATEX), &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (GameSound->WaveFormat.nBlockAlign != ((GameSound->WaveFormat.nChannels * GameSound->WaveFormat.wBitsPerSample) / 8) ||
		(GameSound->WaveFormat.wFormatTag != WAVE_FORMAT_PCM) ||
		(GameSound->WaveFormat.wBitsPerSample != 16))
	{
		Error = ERROR_DATATYPE_MISMATCH;
		LogMessageA(LL_ERROR, "[%s] This wave file did not meet the format requirements! Only PCM format, 44.1Khz, 16 bits per sample wav files are supported. 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	while (DataChunkFound == FALSE)
	{
		if (SetFilePointer(FileHandle, DataChunkOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			Error = GetLastError();
			LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
			goto Exit;
		}

		if (ReadFile(FileHandle, &DataChunkSearcher, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
		{
			Error = GetLastError();
			LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
			goto Exit;
		}

		if (DataChunkSearcher == 0x61746164)	// 'data' backwards
		{
			DataChunkFound = TRUE;
			break;
		}
		else
		{
			DataChunkOffset += 4;
		}

		if (DataChunkOffset > 256)
		{
			Error = ERROR_DATATYPE_MISMATCH;
			LogMessageA(LL_ERROR, "[%s] Data chunk not found withing first 256 bytes of this file! 0x%08lx!", __FUNCTION__, Error);
			goto Exit;
		}
	}

	if (SetFilePointer(FileHandle, DataChunkOffset + 4, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (ReadFile(FileHandle, &DataChunkSize, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	GameSound->Buffer.pAudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, DataChunkSize);
	if (GameSound->Buffer.pAudioData == NULL)
	{
		Error = ERROR_NOT_ENOUGH_MEMORY;
		LogMessageA(LL_ERROR, "[%s] HeapAlloc failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	AudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, DataChunkSize);
	if (AudioData == NULL)
	{
		Error = ERROR_NOT_ENOUGH_MEMORY;
		LogMessageA(LL_ERROR, "[%s] HeapAlloc failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
	GameSound->Buffer.AudioBytes = DataChunkSize;

	if (SetFilePointer(FileHandle, DataChunkOffset + 8, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	if (ReadFile(FileHandle, AudioData, DataChunkSize, &NumberOfBytesRead, NULL) == 0)
	{
		Error = GetLastError();
		LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
		goto Exit;
	}

	GameSound->Buffer.pAudioData = AudioData;

Exit:

	if (Error == ERROR_SUCCESS)
	{
		LogMessageA(LL_INFO, "[%s] Successfully loaded %s!", __FUNCTION__, Filename);
	}
	else
	{
		LogMessageA(LL_ERROR, "[%s] Failed to load %s! Error: 0x%08lx!", __FUNCTION__, Filename, Error);
	}

	if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(FileHandle);
	}
	return Error;
}

void PlayGameSound(_In_ GAMESOUND* GameSound)
{
	gXAudioSFXSourceVoice[gSFXSourceVoiceSelector]->lpVtbl->SubmitSourceBuffer(gXAudioSFXSourceVoice[gSFXSourceVoiceSelector], &GameSound->Buffer, NULL);
	gXAudioSFXSourceVoice[gSFXSourceVoiceSelector]->lpVtbl->Start(gXAudioSFXSourceVoice[gSFXSourceVoiceSelector], 0, XAUDIO2_COMMIT_NOW);
	gSFXSourceVoiceSelector++;
	if (gSFXSourceVoiceSelector > (NUMBER_OF_SFX_SOURCE_VOICES - 1))
	{
		gSFXSourceVoiceSelector = 0;
	}
}
