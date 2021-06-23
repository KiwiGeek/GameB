#include <stdio.h>

#pragma warning(disable: 28251)
#pragma warning(push, 3)
#pragma warning(disable: 4668 4711)
#include <Windows.h>
#pragma warning(pop)

#include "Main.h"

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, INT CmdShow)
{
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

	if (GameIsAlreadyRunning())
	{
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if (CreateMainGameWindow() != ERROR_SUCCESS)
	{
		goto Exit;
	}

	MSG message = { 0 };

	while (GetMessageA(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessageA(&message);
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
				PostQuitMessage(0);
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
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

	if (RegisterClassExA(&windowClass) == 0)
	{
		result = GetLastError();
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	HWND windowHandle = CreateWindowExA(WS_EX_CLIENTEDGE, "GAME_B_WINDOWCLASS", "Window Title",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL,
		GetModuleHandleA(NULL), NULL);

	if (windowHandle == NULL)
	{
		result = GetLastError();
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
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