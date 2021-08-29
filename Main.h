#pragma once

#define _CRT_RAND_S  // NOLINT(clang-diagnostic-reserved-id-macro, bugprone-reserved-identifier)

#pragma warning(disable: 28251)
#pragma warning(disable: 4668 4711 4710)

#pragma warning(push, 3)

#include <Windows.h>
#include <xaudio2.h>
#pragma comment(lib, "XAudio2.lib")
// ReSharper disable once CppUnusedIncludeDirective
#include <stdio.h>
#include <Psapi.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#include <stdint.h>
#pragma comment(lib, "Winmm.lib")
#define AVX					// AVX, SSE2 or nothing
#ifdef AVX
// ReSharper disable once CppUnusedIncludeDirective
#include <immintrin.h>
#elif defined SSE2
#include <emmintrin.h>
#endif

#pragma warning(pop)
// ReSharper disable once CppUnusedIncludeDirective
#include "Tiles.h"

#ifdef _DEBUG
#define ASSERT(Expression, Message) if (!(Expression)) { *(int*)0 = 0; }
#else
#define ASSERT(Expression, Message) ((void)0);
#endif

#define LOAD_ASSET(Filename, AssetType, Destination) if ((error = LoadAssetFromArchive(ASSET_FILE, Filename, AssetType, Destination)) != ERROR_SUCCESS) { LogMessageA(LL_ERROR, "[%s] Loading %s failed with 0x%08lx!", __FUNCTION__, Filename, error); goto Exit; }

#define GAME_NAME								"Game_B"
#define GAME_VER								"0.9a"
#define GAME_CODE_MODULE						"GameCode.dll"
#define GAME_CODE_MODULE_TMP					"GameCode.tmp"
#define LOG_FILE_NAME GAME_NAME					".log"
#define FONT_SHEET_CHARACTERS_PER_ROW			98
#define ASSET_FILE								"Assets.dat"

#define GAME_RES_WIDTH							384
#define GAME_RES_HEIGHT							240
#define GAME_BPP								32
#define GAME_DRAWING_AREA_MEMORY_SIZE			(GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))
#define CALCULATE_AVERAGE_FPS_EVERY_X_FRAMES	120
#define TARGET_MICROSECONDS_PER_FRAME			16667ULL
#define NUMBER_OF_SFX_SOURCE_VOICES				4
#define SUIT_0									0
#define SUIT_1									1
#define SUIT_2									2
#define FACING_DOWN_0							0
#define FACING_DOWN_1							1
#define FACING_DOWN_2							2
#define FACING_LEFT_0							3
#define FACING_LEFT_1							4
#define FACING_LEFT_2							5
#define FACING_RIGHT_0							6
#define FACING_RIGHT_1							7
#define FACING_RIGHT_2							8
#define FACING_UPWARD_0							9
#define FACING_UPWARD_1							10
#define FACING_UPWARD_2							11
#define RANDOM_MONSTER_GRACE_PERIOD_STEPS		3

#define PRESSED_UP g_game_input.UpKeyIsDown && !g_game_input.UpKeyWasDown
#define PRESSED_DOWN g_game_input.DownKeyIsDown && !g_game_input.DownKeyWasDown
#define PRESSED_LEFT g_game_input.LeftKeyIsDown && !g_game_input.LeftKeyWasDown
#define PRESSED_RIGHT g_game_input.RightKeyIsDown && !g_game_input.RightKeyWasDown
#define PRESSED_ESCAPE g_game_input.EscapeKeyIsDown && !g_game_input.EscapeKeyWasDown
#define PRESSED_CHOOSE g_game_input.ChooseKeyIsDown && !g_game_input.ChooseKeyWasDown
#define PRESSED_DEBUG g_game_input.DebugKeyIsDown && !g_game_input.DebugKeyWasDown

typedef enum DIRECTION
{ 
	DOWN = 0,
	LEFT = 3,
	RIGHT = 6,
	UP = 9
} DIRECTION;

typedef enum LOG_LEVEL
{
	LL_NONE = 0,
	LL_ERROR = 1,
	LL_WARNING = 2,
	LL_INFO = 3,
	LL_DEBUG = 4
} LOG_LEVEL;

typedef enum GAME_STATE
{
	GS_OPENING_SPLASH_SCREEN,
	GS_TITLE_SCREEN,
	GS_CHARACTER_NAMING,
	GS_OVERWORLD,
	GS_BATTLE,
	GS_OPTIONS_SCREEN,
	GS_EXIT_YES_NO_SCREEN,
	GS_GAMEPAD_UNPLUGGED,
	GS_NEW_GAME_ARE_YOU_SURE
} GAME_STATE;

typedef enum RESOURCE_TYPE
{
	RT_WAV,
	RT_OGG,
	RT_TILEMAP,
	RT_BMPX
} RESOURCE_TYPE;

typedef enum WINDOW_FLAGS
{
	WF_BORDERED = 1,
	WF_HORIZONTALLY_CENTERED = 2,
	WF_VERTICALLY_CENTERED = 4,
	WF_SHADOWED = 8,
	WF_SHAKE = 16
} WINDOW_FLAGS;

typedef struct UPOINT
{
	uint16_t	X;
	uint16_t	Y;
} UPOINT;

typedef struct GAME_INPUT 
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
} GAME_INPUT;

#pragma warning(disable: 5045)	// disable warning about structure padding
#pragma warning(disable: 4820)	// disable warning about Spectre/Meltdown CPU vulnerability

typedef LONG (NTAPI* NtQueryTimerResolution)(OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);
NtQueryTimerResolution nt_query_timer_resolution;

typedef struct GAME_BITMAP
{
	BITMAPINFO BitmapInfo;
	void* Memory;
} GAME_BITMAP;

typedef struct GAME_SOUND 
{
	WAVEFORMATEX WaveFormat;
	XAUDIO2_BUFFER Buffer;
} GAME_SOUND;

typedef struct GAME_AREA
{
	char* Name;
	RECT Area;
	GAME_SOUND* Music;
} GAME_AREA;

typedef union PIXEL32
{
	struct COLORS
	{
		uint8_t Blue;
		uint8_t Green;
		uint8_t Red;
		uint8_t Alpha;
	} colors;
	DWORD bytes;
} PIXEL32;

typedef struct GAME_PERF_DATA
{
	uint64_t TotalFramesRendered;
	float RawFPSAverage;
	float CookedFPSAverage;
	int64_t PerfFrequency;
	MONITORINFO MonitorInfo;
	BOOL DisplayDebugInfo;
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
} GAME_PERF_DATA;

typedef struct TILEMAP
{
	uint16_t Width;
	uint16_t Height;
	uint8_t** Map;
} TILEMAP;

typedef struct GAMEMAP
{
	TILEMAP TileMap;
	GAME_BITMAP GameBitmap;
} GAMEMAP;

typedef struct HERO
{
	char Name[9];
	GAME_BITMAP Sprite[3][12];
	BOOL Active;
	UPOINT ScreenPos;
	UPOINT WorldPos;
	uint8_t MovementRemaining;
	DIRECTION Direction;
	BOOL HasPlayerMovedSincePortal;
	uint8_t CurrentArmor;
	uint8_t SpriteIndex;
	uint64_t StepsTaken;
	uint8_t RandomEncounterPercentage;
	uint64_t StepsSinceLastRandomMonsterEncounter;
	int16_t HP;
	int32_t XP;
	int16_t Money;
	int16_t Strength;
	int16_t Luck;
	int16_t MP;
} HERO;

typedef struct REGISTRY_PARAMS
{
	DWORD LogLevel;
	DWORD SFXVolume;
	DWORD MusicVolume;
	DWORD ScaleFactor;
} REGISTRY_PARAMS;

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

GAME_PERF_DATA g_performance_data;
GAME_BITMAP g_back_buffer;
GAME_BITMAP g_6x7_font;
GAME_BITMAP g_battle_scene_grasslands01;
GAME_BITMAP g_battle_scene_dungeon01;
GAMEMAP g_overworld01;
GAME_STATE g_current_game_state;
GAME_STATE g_previous_game_state;
GAME_INPUT g_game_input;
GAME_SOUND g_sound_menu_navigate;
GAME_SOUND g_sound_menu_choose;
GAME_SOUND g_sound_splash_screen;
GAME_SOUND g_music_overworld01;
GAME_SOUND g_music_dungeon01;
GAME_SOUND g_music_battle_intro01;
GAME_SOUND g_music_battle01;
HERO g_player;
float g_sfx_volume;
float g_music_volume;
BOOL g_music_is_paused;
int8_t g_gamepad_id;
HWND g_game_window;
IXAudio2SourceVoice* g_xaudio_sfx_source_voice[NUMBER_OF_SFX_SOURCE_VOICES];
IXAudio2SourceVoice* g_xaudio_music_source_voice;
uint8_t g_passable_tiles[3];
UPOINT g_camera;
HANDLE g_asset_loading_thread_handle;
HANDLE g_essential_assets_loaded_event;
BOOL g_input_enabled;
BOOL g_game_is_running;
HMODULE g_game_code_module;
FILETIME g_game_code_last_write_time;

// imports from GAMECODE.DLL
// ReSharper disable once CppInconsistentNaming
typedef int(__cdecl* _RandomMonsterEncounter)(_In_ GAME_STATE* PreviousGameState, _Inout_ GAME_STATE* CurrentGameState);  // NOLINT(bugprone-reserved-identifier)
_RandomMonsterEncounter RandomMonsterEncounter;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);
DWORD LoadGameCode(_In_ const char* ModuleFileName);
DWORD CreateMainGameWindow(void);
BOOL GameIsAlreadyRunning(void);
void ProcessPlayerInput(void);
DWORD InitializeHero(void);
void Blit32BppBitmapToBuffer(_In_ const GAME_BITMAP* GameBitmap, _In_ int16_t X, _In_ int16_t Y, _In_ int16_t BrightnessAdjustment);
void BlitBackgroundToBuffer(_In_ const GAME_BITMAP* GameBitmap, _In_ int16_t BrightnessAdjustment);
void BlitStringToBuffer(_In_ const char* String, _In_ const GAME_BITMAP* FontSheet, _In_ const PIXEL32* Color, _In_ int16_t X, _In_ int16_t Y);
void RenderFrameGraphics(void);
DWORD LoadRegistryParameters(void);
DWORD SaveRegistryParameters(void);
void LogMessageA(_In_ LOG_LEVEL LogLevel, _In_ char* Message, _In_ ...);
void DrawDebugInfo(void);
void FindFirstConnectedGamepad(void);
HRESULT InitializeSoundEngine(void);
DWORD LoadAssetFromArchive(_In_ char* ArchiveName, _In_ char* AssetFileName, _In_ RESOURCE_TYPE ResourceType, _Inout_ void* Resource);
DWORD LoadWavFromMemory(_In_ void* Buffer, _Inout_ GAME_SOUND* GameSound);
DWORD LoadOggFromMemory(_In_ const void* Buffer, _In_ uint64_t BufferSize, _Inout_ GAME_SOUND* GameSound);
DWORD LoadTileMapFromMemory(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap);
DWORD Load32BppBitmapFromMemory(_In_ void* Buffer, _Inout_ GAME_BITMAP* GameBitmap);
void PlayGameSound(_In_ const GAME_SOUND* GameSound);
void PlayGameMusic(_In_ GAME_SOUND* GameSound, _In_ BOOL Loop, _In_ BOOL Immediate);
void PauseMusic(void);
void StopMusic(void);
BOOL MusicIsPlaying(void);
DWORD AssetLoadingThreadProc(_In_ LPVOID Param);
void InitializeGlobals(void);
void DrawWindow(_In_ int16_t X, _In_ int16_t Y, _In_ int16_t Width, _In_ int16_t Height, _In_ PIXEL32 BackgroundColor, _In_ DWORD Flags);