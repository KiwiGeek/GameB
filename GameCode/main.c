// Platform-independent game code binary

#ifdef _WIN32
#pragma warning(disable: 4668)
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	// DllMain intentionally does nothing.
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(Reason);
	UNREFERENCED_PARAMETER(Reserved);

	return TRUE;
}

#endif

_declspec(dllexport) int TestFunc01(void)  // NOLINT(clang-diagnostic-language-extension-token)
{
	MessageBoxA(NULL, "Hello from TestFunc02", "Test", MB_OK | MB_ICONASTERISK);
	return 0;
}
