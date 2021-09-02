@ECHO OFF

REM -- Clean up old asset archive --
ECHO Cleaning up asset build artifacts
DEL Assets.dat
DEL CopyAssets.log

REM --- BITMAPS ---
ECHO Bundling bitmaps into asset archive
MyMiniz.exe Assets.dat + .\Assets\6x7font.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Down_Standing.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Down_Walk1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Down_Walk2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Left_Standing.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Left_Walk1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Left_Walk2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Right_Standing.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Right_Walk1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Right_Walk2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Up_Standing.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Up_Walk1.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Hero_Suit0_Up_Walk2.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Maps\Overworld01.bmpx >> CopyAssets.log

REM --- BATTLE SCENES ---
MyMiniz.exe Assets.dat + .\Assets\BattleScenes\Grasslands01.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\BattleScenes\Dungeon01.bmpx >> CopyAssets.log

REM --- SOUNDS ---
ECHO Bundling sounds into asset archive
MyMiniz.exe Assets.dat + .\Assets\MenuChoose.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\MenuNavigate.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\SplashScreen.wav >> CopyAssets.log

REM --- MUSIC ---
ECHO Bundling music into asset archive
MyMiniz.exe Assets.dat + .\Assets\Overworld01.ogg >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Dungeon01.ogg >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Dungeon01.ogg >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Battle01.ogg >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\BattleIntro01.ogg >> CopyAssets.log

REM --- MONSTERS ---
ECHO Bundling monsters into asset archive
MyMiniz.exe Assets.dat + .\Assets\Monsters\Slime001.bmpx >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Monsters\Rat001.bmpx >> CopyAssets.log

REM --- TILEMAPS ---
ECHO Bundling tilemap into asset archive
MyMiniz.exe Assets.dat + .\Assets\Maps\Overworld01.tmx >> CopyAssets.log

REM --- TILEBITMAPS ---
REM ECHO Bundling unnecessary tilebitmaps into asset archive
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Grass01.bmp >> CopyAssets.log
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Water01.bmp >> CopyAssets.log
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Bricks01.bmp >> CopyAssets.log
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Debug01.bmp >> CopyAssets.log
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Portal01.bmp >> CopyAssets.log

REM --- TILESETS ---
REM ECHO Bundling unnecessary tileset into asset archive
REM MyMiniz.exe Assets.dat + .\Assets\Maps\Tileset01.tsx >> CopyAssets.log

REM -- Copy asset bundle into debug and release folders --
ECHO Copying asset bundle to Debug and Release folders
COPY assets.dat %GAMEBDIR%\x64\Debug\assets.dat
COPY assets.dat %GAMEBDIR%\x64\Release\assets.dat
COPY assets.dat %GAMEBDIR%\build\assets.dat