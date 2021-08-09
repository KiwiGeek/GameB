#pragma once

#pragma warning(disable: 28251)
#pragma warning(disable: 4668 4711)

#pragma warning(push, 3)

#include <Windows.h>
#include <xaudio2.h>
#pragma comment(lib, "XAudio2.lib")
#include <stdio.h>
#include <Psapi.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#include <stdint.h>
#pragma comment(lib, "Winmm.lib")
#define AVX					// AVX, SSE2 or nothing
#ifdef AVX
#include <immintrin.h>
#elif defined SSE2
#include <emmintrin.h>
#endif

#pragma warning(pop)
#include "Tiles.h"

#ifdef _DEBUG
#define ASSERT(Expression, Message) if (!(Expression)) { *(int*)0 = 0; }
#else
#define ASSERT(Expression, Message) ((void)0);
#endif

#define GAME_NAME	"Game_B"
#define GAME_VER	"0.9a"
#define GAME_RES_WIDTH  384
#define GAME_RES_HEIGHT	240
#define GAME_BPP		32
#define GAME_DRAWING_AREA_MEMORY_SIZE	(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))
#define CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES	120
#define TARGET_MICROSECONDS_PER_FRAME			16667ULL
#define NUMBER_OF_SFX_SOURCE_VOICES				4
#define SUIT_0	0
#define SUIT_1	1
#define SUIT_2	2
#define FACING_DOWN_0	0
#define FACING_DOWN_1	1
#define FACING_DOWN_2	2
#define FACING_LEFT_0	3
#define FACING_LEFT_1	4
#define FACING_LEFT_2	5
#define FACING_RIGHT_0	6
#define FACING_RIGHT_1	7
#define FACING_RIGHT_2	8
#define FACING_UPWARD_0	9
#define FACING_UPWARD_1	10
#define FACING_UPWARD_2	11

#define PRESSED_UP gGameInput.UpKeyIsDown			&& !gGameInput.UpKeyWasDown
#define PRESSED_DOWN gGameInput.DownKeyIsDown		&& !gGameInput.DownKeyWasDown
#define PRESSED_LEFT gGameInput.LeftKeyIsDown		&& !gGameInput.LeftKeyWasDown
#define PRESSED_RIGHT gGameInput.RightKeyIsDown		&& !gGameInput.RightKeyWasDown
#define PRESSED_ESCAPE gGameInput.EscapeKeyIsDown	&& !gGameInput.EscapeKeyWasDown
#define PRESSED_CHOOSE gGameInput.ChooseKeyIsDown	&& !gGameInput.ChooseKeyWasDown
#define PRESSED_DEBUG gGameInput.DebugKeyIsDown		&& !gGameInput.DebugKeyWasDown

#define DEBUG_DISPLAY_NONE		0
#define DEBUG_DISPLAY_VARS		1
#define DEBUG_DISPLAY_TILES		2

typedef enum DIRECTION
{ 
	DOWN = 0,
	LEFT = 3,
	RIGHT = 6,
	UP = 9
} DIRECTION;

typedef enum LOGLEVEL
{
	LL_NONE = 0,
	LL_ERROR = 1,
	LL_WARNING = 2,
	LL_INFO = 3,
	LL_DEBUG = 4
} LOGLEVEL;

typedef enum GAMESTATE
{
	GS_OPENINGSPLASHSCREEN,
	GS_TITLESCREEN,
	GS_CHARACTERNAMING,
	GS_OVERWORLD,
	GS_BATTLE,
	GS_OPTIONSSCREEN,
	GS_EXITYESNOSCREEN,
	GS_GAMEPADUNPLUGGED
} GAMESTATE;

typedef struct UPOINT
{
	uint16_t	X;
	uint16_t	Y;
} UPOINT;

typedef struct GAMEINPUT 
{
	int16_t EscapeKeyIsDown;
	int16_t DebugKeyIsDown;
	int16_t LeftKeyIsDown;
	int16_t RightKeyIsDown;
	int16_t UpKeyIsDown;
	int16_t DownKeyIsDown;
	int16_t ChooseKeyIsDown;
	int16_t EscapeKeyWasDown;
	int16_t DebugKeyWasDown;
	int16_t LeftKeyWasDown;
	int16_t RightKeyWasDown;
	int16_t UpKeyWasDown;
	int16_t DownKeyWasDown;
	int16_t ChooseKeyWasDown;
} GAMEINPUT;

#define LOG_FILE_NAME GAME_NAME ".log"
#define FONT_SHEET_CHARACTERS_PER_ROW	98

#pragma warning(disable: 4820)	// disable warning about structure padding
#pragma warning(disable: 5045)	// disable warning about Spectre/Meltdown CPU vulnerability

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);

_NtQueryTimerResolution NtQueryTimerResolution;

typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;
	void* Memory;
} GAMEBITMAP;

typedef struct GAMESOUND 
{
	WAVEFORMATEX WaveFormat;
	XAUDIO2_BUFFER Buffer;
} GAMESOUND;

typedef struct PIXEL32
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;
} PIXEL32;

typedef struct GAMEPERFDATA
{
	uint64_t TotalFramesRendered;
	float RawFPSAverage;
	float CookedFPSAverage;
	int64_t PerfFrequency;
	MONITORINFO MonitorInfo;
	uint8_t DisplayDebugInfo;			// 0 = none, 1 = vars, 2 = tilemap
	ULONG MinimumTimerResolution;
	ULONG MaximumTimerResolution;
	ULONG CurrentTimerResolution;
	DWORD HandleCount;
	PROCESS_MEMORY_COUNTERS_EX MemInfo;
	SYSTEM_INFO SystemInfo;
	int64_t CurrentSystemTime;
	int64_t PreviousSystemTime;
	double CPUPercent;
	uint8_t MaxScaleFactor;
	uint8_t CurrentScaleFactor;
} GAMEPERFDATA;

typedef struct TILEMAP
{
	uint16_t Width;
	uint16_t Height;
	uint8_t** Map;
} TILEMAP;

typedef struct GAMEMAP
{
	TILEMAP TileMap;
	GAMEBITMAP GameBitmap;
} GAMEMAP;

typedef struct HERO
{
	char Name[9];
	GAMEBITMAP Sprite[3][12];
	BOOL Active;
	UPOINT ScreenPos;
	UPOINT WorldPos;
	uint8_t MovementRemaining;
	DIRECTION Direction;
	uint8_t CurrentArmor;
	uint8_t SpriteIndex;
	int32_t HP;
	int32_t Strength;
	int32_t MP;
} HERO;

typedef struct REGISTRYPARAMS
{
	DWORD LogLevel;
	DWORD SFXVolume;
	DWORD MusicVolume;
	DWORD ScaleFactor;
} REGISTRYPARAMS;

typedef struct MENUITEM
{
	char* Name;
	int16_t X;
	int16_t Y;
	BOOL Enabled;
	void (*Action)(void);
} MENUITEM;

typedef struct MENU
{
	char* Name;
	uint8_t SelectedItem;
	uint8_t ItemCount;
	MENUITEM** Items;
} MENU;

GAMEPERFDATA gPerformanceData;
GAMEBITMAP gBackBuffer;
GAMEBITMAP g6x7Font;
GAMEMAP gOverworld01;
GAMESTATE gCurrentGameState;
GAMESTATE gPreviousGameState;
GAMEINPUT gGameInput;
GAMESOUND gSoundMenuNavigate;
GAMESOUND gSoundMenuChoose;
GAMESOUND gSoundSplashScreen;
HERO gPlayer;
float gSFXVolume;
float gMusicVolume;
int8_t gGamepadID;
HWND gGameWindow;
IXAudio2SourceVoice* gXAudioSFXSourceVoice[NUMBER_OF_SFX_SOURCE_VOICES];
IXAudio2SourceVoice* gXAudioMusicSourceVoice;
uint8_t gPassableTiles[1];
UPOINT gCamera;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);
DWORD CreateMainGameWindow(void);
BOOL GameIsAlreadyRunning(void);
void ProcessPlayerInput(void);
DWORD Load32BbpBitmapFromFile(_In_ char* Filename, _Inout_ GAMEBITMAP* GameBitmap);
DWORD InitializeHero(void);
void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ uint16_t x, _In_ uint16_t y);
void BlitBackgroundToBuffer(_In_ GAMEBITMAP* GameBitmap);
void BlitStringToBuffer(_In_ char* String, _In_ GAMEBITMAP* FontSheet, _In_ PIXEL32* Color, _In_ uint16_t x, _In_ uint16_t y);
void RenderFrameGraphics(void);
DWORD LoadRegistryParameters(void);
DWORD SaveRegistryParameters(void);
void LogMessageA(_In_ LOGLEVEL LogLevel, _In_ char* Message, _In_ ...);
void DrawDebugInfo(void);
void FindFirstConnectedGamepad(void);
HRESULT InitializeSoundEngine(void);
DWORD LoadWavFromFile(_In_ char* Filename, _Inout_ GAMESOUND* GameSound);
void PlayGameSound(_In_ GAMESOUND* GameSound);
#ifdef AVX
void ClearScreen(_In_ __m256i* Color);
#elif defined SSE2
void ClearScreen(_In_ __m128i* Color);
#else
void ClearScreen(_In_ PIXEL32* Pixel);
#endif
DWORD LoadTilemapFromFile(_In_ char* FileName, _Inout_ TILEMAP* TileMap);