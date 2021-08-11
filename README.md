# Build Instructions

This project contains a post-build step. The post-build step runs the batch file CopyAssets.bat. The CopyAssets.bat batch file contains an environment variable that you need to create on your machine. The environment variable is called GAMEBDIR and it should point to the fully-qualified path to the project on your machine. You can use the setx utility to create this environment variable. For example:
```
C:\>setx GAMEBDIR W:\GameB

```

You probably have to restart Visual Studio after creating this environment variable for the first time.