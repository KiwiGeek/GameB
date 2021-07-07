#pragma warning(disable: 28251)
#pragma warning(disable: 4668 4711)

#pragma warning(push, 3)
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
#include <emmintrin.h>
#pragma warning(pop)

#include <stdint.h>
#include "Main.h"

#pragma comment(lib, "Winmm.lib")

HWND gGameWindow;
BOOL gGameIsRunning;
GAMEBITMAP gBackBuffer;
GAMEPERFDATA gPerformanceData;
PLAYER gPlayer;
BOOL gWindowHasFocus;

int _stdcall WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, INT CmdShow)
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

	if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
	{
		MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
	{
		MessageBoxA(NULL, "Couldn't find the NtQueryTimerResolution function in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	NtQueryTimerResolution(&gPerformanceData.MinimumTimerResolution, &gPerformanceData.MaximumTimerResolution, &gPerformanceData.CurrentTimerResolution);
	GetSystemInfo(&gPerformanceData.SystemInfo);
	GetSystemTimeAsFileTime((LPFILETIME)&gPerformanceData.PreviousSystemTime);

	if (GameIsAlreadyRunning())
	{
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if(SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS) == 0)
	{
		MessageBoxA(NULL, "Failed to set process priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	
	if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
	{
		MessageBoxA(NULL, "Failed to set thread priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	
	if (timeBeginPeriod(1) == TIMERR_NOCANDO)
	{
		MessageBoxA(NULL, "Failed to set global timer resolution!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if (CreateMainGameWindow() != ERROR_SUCCESS)
	{
		goto Exit;
	}

	QueryPerformanceFrequency((LARGE_INTEGER*)&gPerformanceData.PerfFrequency);

	gPerformanceData.DisplayDebugInfo = TRUE;

	gBackBuffer.BitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.BitmapInfo.bmiHeader);
	gBackBuffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
	gBackBuffer.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
	gBackBuffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
	gBackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	gBackBuffer.BitmapInfo.bmiHeader.biPlanes = 1;
	gBackBuffer.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (gBackBuffer.Memory == NULL)
	{
		MessageBox(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	memset(gBackBuffer.Memory, 0x7F, GAME_DRAWING_AREA_MEMORY_SIZE);

	gPlayer.ScreenPosX = 25;
	gPlayer.ScreenPosY = 25;

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
	windowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

	//SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (RegisterClassExA(&windowClass) == 0)
	{
		result = GetLastError();
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	gGameWindow = CreateWindowExA(0, "GAME_B_WINDOWCLASS", "Window Title",
								  WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL,
								  GetModuleHandleA(NULL), NULL);

	if (gGameWindow == NULL)
	{
		result = GetLastError();
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	gPerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gPerformanceData.MonitorInfo) == 0)
	{
		result = ERROR_MONITOR_NO_DESCRIPTOR;
		goto Exit;
	}

	gPerformanceData.MonitorWidth = gPerformanceData.MonitorInfo.rcMonitor.right - gPerformanceData.MonitorInfo.rcMonitor.left;
	gPerformanceData.MonitorHeight = gPerformanceData.MonitorInfo.rcMonitor.bottom - gPerformanceData.MonitorInfo.rcMonitor.top;

	if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, WS_VISIBLE) == 0)
	{
		result = GetLastError();
		goto Exit;
	}
	if (SetWindowPos(gGameWindow, HWND_TOP,
					 gPerformanceData.MonitorInfo.rcMonitor.left, gPerformanceData.MonitorInfo.rcMonitor.top,
					 gPerformanceData.MonitorWidth, gPerformanceData.MonitorHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
	{
		result = GetLastError();
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
	
	int16_t EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
	int16_t DebugKeyIsDown = GetAsyncKeyState(VK_F1);
	int16_t LeftKeyIsDown = GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A');
	int16_t RightKeyIsDown = GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D');
	int16_t UpKeyIsDown = GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W');
	int16_t DownKeyIsDown = GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S');

	static int16_t DebugKeyWasDown;

	static int16_t LeftKeyWasDown;
	static int16_t RightKeyWasDown;
	static int16_t UpKeyWasDown;
	static int16_t DownKeyWasDown;

	if (EscapeKeyIsDown)
	{
		SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
	}

	if (DebugKeyIsDown && !DebugKeyWasDown)
	{
		gPerformanceData.DisplayDebugInfo = !gPerformanceData.DisplayDebugInfo;
	}

	if (LeftKeyIsDown)
	{
		if (gPlayer.ScreenPosX > 0)
		{
			gPlayer.ScreenPosX--;
		}
	}

	if (RightKeyIsDown)
	{
		if (gPlayer.ScreenPosX < GAME_RES_WIDTH - 16)
		{
			gPlayer.ScreenPosX++;
		}
	}

	if (DownKeyIsDown)
	{
		if (gPlayer.ScreenPosY < GAME_RES_HEIGHT - 16)
		{
			gPlayer.ScreenPosY++;
		}
	}

	if (UpKeyIsDown)
	{
		if (gPlayer.ScreenPosY > 0)
		{
			gPlayer.ScreenPosY--;
		}
	}

	DebugKeyWasDown = DebugKeyIsDown;
	LeftKeyWasDown = LeftKeyIsDown;
	RightKeyWasDown = RightKeyIsDown;
	UpKeyWasDown = UpKeyIsDown;
	DownKeyWasDown = DownKeyIsDown;
}

void RenderFrameGraphics(void)
{

#ifdef SIMD
	__m128i QuadPixel = { 0x7f, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0xff };
	ClearScreen(&QuadPixel);
#else
	PIXEL32 Pixel = { 0x7f, 0x00, 0x00, 0xff };
	ClearScreen(&Pixel);
#endif


	int32_t ScreenX = gPlayer.ScreenPosX;
	int32_t ScreenY = gPlayer.ScreenPosY;
	int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * ScreenY) + ScreenX;

	for (int32_t y = 0; y < 16; y++)
	{
		for (int32_t x = 0; x < 16; x++)
		{
			memset((PIXEL32*)gBackBuffer.Memory + (uintptr_t)StartingScreenPixel + x - ((uintptr_t)GAME_RES_WIDTH * y), 0xFF, sizeof(PIXEL32));
		}
	}

	HDC DeviceContext = GetDC(gGameWindow);

	StretchDIBits(DeviceContext, 0, 0, gPerformanceData.MonitorWidth, gPerformanceData.MonitorHeight,
				  0, 0, GAME_RES_WIDTH, GAME_RES_HEIGHT,
				  gBackBuffer.Memory, &gBackBuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	if (gPerformanceData.DisplayDebugInfo == TRUE)
	{
		SelectObject(DeviceContext, (HFONT)GetStockObject(ANSI_FIXED_FONT));

		char DebugTextBuffer[64] = { 0 };

		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Raw:    %.01f", gPerformanceData.RawFPSAverage);
		TextOutA(DeviceContext, 0, 0, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Cooked: %.01f", gPerformanceData.CookedFPSAverage);
		TextOutA(DeviceContext, 0, 13, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Min Timer Res: %.02f", gPerformanceData.MinimumTimerResolution / 10000.0f);
		TextOutA(DeviceContext, 0, 26, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Max Timer Res: %.02f", gPerformanceData.MaximumTimerResolution / 10000.0f);
		TextOutA(DeviceContext, 0, 39, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Cur Timer Res: %.02f", gPerformanceData.CurrentTimerResolution / 10000.0f);
		TextOutA(DeviceContext, 0, 52, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Handles: %lu", gPerformanceData.HandleCount);
		TextOutA(DeviceContext, 0, 65, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Memory:  %i KB", (int)(gPerformanceData.MemInfo.PrivateUsage / 1024));
		TextOutA(DeviceContext, 0, 78, DebugTextBuffer, (int)strlen(DebugTextBuffer));
		sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU:     %.02f%%", gPerformanceData.CPUPercent);
		TextOutA(DeviceContext, 0, 91, DebugTextBuffer, (int)strlen(DebugTextBuffer));
	}

	ReleaseDC(gGameWindow, DeviceContext);
}

#ifdef SIMD
_forceinline void ClearScreen(_In_ __m128i* Color)
{
	for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x += 4)
	{
		_mm_store_si128((PIXEL32*)gBackBuffer.Memory + x, *Color);
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
