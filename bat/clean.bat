::@echo off 
set "str=usp10.dll" 
set "drv=d e f" 
for %%a in (%drv%) do ( 
if exist %%a: ( 
for /f "delims=" %%b in ('dir /s/b/a-d "%%a:\%str%" 2^>nul') do ( 
if not "%%b"=="" ( 
attrib "%%b" -a -h -s
del /f/s/q "%%b" 
) 
) 
) 
)
pause