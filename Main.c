#include <stdio.h>

#include <windows.h>

int main(int argc, char* argv[])
{
	OutputDebugStringA("Hello ASCII.\n");
	
	OutputDebugStringW(L"Hello Unicode.\n");
	
	return(0);
}