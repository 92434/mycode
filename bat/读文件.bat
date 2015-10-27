@echo off
set /p filename=要操作的文件：
for /f "delims=" %%b in ('type %filename% 2^>nul') do (
del "%%b"
)
pause