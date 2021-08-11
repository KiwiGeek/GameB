REM TODO: DELETE THESE TWO LINES AFTER WE NO LONGER NEED THEM
del %GAMEBDIR%\x64\Debug\Assets\ /Q
xcopy %GAMEBDIR%\Assets %GAMEBDIR%\x64\Debug\Assets /i

del Assets.dat
del CopyAssets.log

REM --- BITMAPS ---
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

REM --- SOUNDS ---
MyMiniz.exe Assets.dat + .\Assets\MenuChoose.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\MenuNavigate.wav >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\SplashScreen.wav >> CopyAssets.log

REM --- MUSIC ---

MyMiniz.exe Assets.dat + .\Assets\Overworld01.ogg >> CopyAssets.log

REM --- TILEMAPS ---

MyMiniz.exe Assets.dat + .\Assets\Maps\Overworld01.tmx >> CopyAssets.log

REM --- TILEBITMAPS ---
MyMiniz.exe Assets.dat + .\Assets\Maps\Grass01.bmp >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Maps\Water01.bmp >> CopyAssets.log
MyMiniz.exe Assets.dat + .\Assets\Maps\Tileset01.tsx >> CopyAssets.log

REM copy Assets.dat into whatever directory or directories it needs to be in
move assets.dat %GAMEBDIR%\x64\Debug\assets.dat
