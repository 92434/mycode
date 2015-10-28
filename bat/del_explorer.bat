echo off
for %%a in (d e f) do (
if exist %%a: (
for /f "delims=" %%b in ('dir /s/b/a-d "%%a:\explorer.exe" 2^>nul') do (
echo %%b
attrib -a -h -s "%%b"
del "%%b"
)
)
)
pause