# Build Instructions

The batch file named `CopyAssets.bat` creates a compressed archive of assets named `Assets.dat` using the assets
stored in the `.\Assets` directory. The `Assets.dat` file needs to reside in the same directory as the game executable.

If you run the game from the Visual Studio debugger, the working directory of the game will be the same directory
as the Visual Studio solution. But if you run the game normally, outside of the debugger, the working directory
will be the same directory as the game's executable.

The `CopyAssets.bat` batch file contains an environment variable that you need to create on your machine. The environment variable is called GAMEBDIR and it should point to the fully-qualified path to the project on your machine. You can use the setx utility to create this environment variable. For example:
```
C:\>setx GAMEBDIR W:\GameB
```

You probably have to restart Visual Studio after creating this environment variable for the first time.