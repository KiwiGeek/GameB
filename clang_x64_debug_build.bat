@echo off
ECHO.
ECHO ==================================
ECHO Clang x64 Debug Build Batch Script
ECHO ==================================
ECHO.

SET WIN_SDK_PATH=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64
SET OUTPUT_PATH=.\x64\Debug\GameB_Clang.exe
@echo on
clang -g -gcodeview Battle.c CharacterNamingScreen.c ExitYesNoScreen.c GamePadUnplugged.c Main.c ^
  Miniz.c NewGameAreYouSure.c OpeningSplashScreen.c OptionsScreen.c Overworld.c stb_vorbis.c TitleScreen.c ^
  -std=c99 -O1 -Wall -march=core-avx2 ^
  -D_DEBUG -DDEBUG -D_WIN64 -D_MBCS -DCLANG  ^
  -o %OUTPUT_PATH% ^
  -l"%WIN_SDK_PATH%\user32.lib" ^
  -l"%WIN_SDK_PATH%\AdvApi32.lib" ^
  -l"%WIN_SDK_PATH%\gdi32.lib" ^
  -l"%WIN_SDK_PATH%\ole32.lib"
@echo off
if %ERRORLEVEL% NEQ 0 GOTO END
CHOICE /M "Launch executable under debugger?"
IF %ERRORLEVEL% EQU 1 (start windbgx %OUTPUT_PATH%)
:END