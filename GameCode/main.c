// Platform-independent game code binary

#pragma warning(disable: 4668)
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

_declspec(dllexport) int TestFunc01(void)  // NOLINT(clang-diagnostic-language-extension-token)
{
	MessageBoxA(NULL, "Hello from TestFunc01", "Test", MB_OK | MB_ICONASTERISK);
	return 0;
}

// DllMain intentionally does nothing.
BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{

	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(Reason);
	UNREFERENCED_PARAMETER(Reserved);
	
	return TRUE;
}