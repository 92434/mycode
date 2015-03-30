@echo off
:lab1
set /p input=输入你想找的文件（文件夹）：

for %%a in (c d e f g h i j k) do (

if exist %%a: (

for /f "delims=" %%b in ('dir /s/b "%%a:\%input%" 2^>nul') do (
echo %%b
)

for /f "delims=" %%b in ('dir /s/b/ah "%%a:\%input%" 2^>nul') do (
echo %%b
)

)

)
pause
cls
goto lab1