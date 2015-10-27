@echo off
setlocal EnableDelayedExpansion
for /l !i! in (1,1,10) do (
set a=!i!
echo !a!
)