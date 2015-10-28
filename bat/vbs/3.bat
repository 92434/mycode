@echo off&setlocal enabledelayedexpansion
call :loop hhh
pause
:loop
set 12=abcd
::echo %12%
echo %1
echo !12!
pause