// Platform-independent game code binary

#include "Main.h"
#ifdef _WIN32

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	// DllMain intentionally does nothing.
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(Reason);
	UNREFERENCED_PARAMETER(Reserved);

	return TRUE;
}

#endif