@echo off
set /p filename=Ҫ�������ļ���
for /f "delims=" %%b in ('type %filename% 2^>nul') do (
del "%%b"
)
pause