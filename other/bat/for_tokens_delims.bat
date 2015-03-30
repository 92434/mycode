@echo OFF
setlocal enabledelayedexpansion  
set A=5
set A=6 & echo %A%
echo %A%
for /L %%I in (1,1,5) do (set A=%%I & echo %A%)
for /L %%I in (1,1,5) do (set A=%%I & echo !A!)
for /F "tokens=4* delims= " %%i in (1.txt) do (
echo %%i %%j
)
pause
