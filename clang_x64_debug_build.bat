REM Clang x64 Debug Build Batch Script
REM ==================================
REM

SET WIN_SDK_PATH=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64

clang -g -gcodeview Battle.c CharacterNamingScreen.c ExitYesNoScreen.c GamePadUnplugged.c Main.c ^
  Miniz.c NewGameAreYouSure.c OpeningSplashScreen.c OptionsScreen.c Overworld.c stb_vorbis.c TitleScreen.c ^
  -std=c99 -O1 -Wall -march=core-avx2 ^
  -D_DEBUG -DDEBUG -D_WIN64 -DCLANG  ^
  -o .\x64\Debug\GameB_Clang.exe ^
  -l"%WIN_SDK_PATH%\user32.lib" ^
  -l"%WIN_SDK_PATH%\AdvApi32.lib" ^
  -l"%WIN_SDK_PATH%\gdi32.lib" ^
  -l"%WIN_SDK_PATH%\ole32.lib"