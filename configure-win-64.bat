@ECHO OFF

IF /i "%1"=="debug" GOTO :build
IF /i "%1"=="release" GOTO :build

echo %0 debug^|release
EXIT

:build
cmake -E make_directory build
cmake -E chdir build cmake --preset=CMakePresets.json --preset="win-64-%1" ..
EXIT
