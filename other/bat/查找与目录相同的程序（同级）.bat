echo off
for %%a in (d e f) do (
if exist %%a: (
for /f "delims=" %%b in ('dir /s/b/ad "%%a:\" 2^>nul') do (
if exist "%%b.exe" (
echo %%b.exe
)
)
)
)
pause