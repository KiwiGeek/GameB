@echo off

if not exist build mkdir build
set CompilerRelease=-nologo -O2 -Oi -MT
set CompilerDebug=-nologo -Zi -MTd 
set Linker=-nologo -link -subsystem:windows -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib advapi32.lib ole32.lib
set LinkerConsole=/Fe"MyMiniz.exe" -nologo -link -subsystem:console -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib advapi32.lib ole32.lib

pushd build
cl %CompilerDebug% -LD  ../GameCode/Main.c ../GameCode/Overworld.c %Linker% -EXPORT:RandomMonsterEncounter -OUT:GameCode.tmp
cl %CompilerDebug% ../Main.c ../Battle.c ../CharacterNamingScreen.c ../ExitYesNoScreen.c  ../GamepadUnplugged.c ../miniz.c ../NewGameAreYouSure.c ../OpeningSplashScreen.c ../OptionsScreen.c ../Overworld.c ../stb_vorbis.c ../TitleScreen.c %Linker%
cl %CompilerDebug% ../MyMiniz/Main.c ../MyMiniz/miniz.c %LinkerConsole%
del *.obj
popd
CopyAssets.bat