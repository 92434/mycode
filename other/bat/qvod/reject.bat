@echo off 
echo [.ShellClassInfo]>desktop.ini 
echo CLSID={0CD7A5C0-9F37-11CE-AE65-08002B2E1262}>>desktop.ini 
REM 改进后有些地方如有空格的地方也可以用所以加上"号 
::attrib +s +h "%cd%" 
rem 其实只要将上面加上隐藏就可以了不用将下面这行隐藏 
attrib +s +h desktop.ini 
::if not exist "F:\temp\control\" md "F:\temp\control\"
::set str=%cd%
::for /f "delims=" %%a in ("%str%") do echo cacls "%cd%" /t /c /e /g %username%:f>F:\temp\control\%%~na_recover.bat
::for /f "delims=" %%a in ("%str%") do echo attrib -s -h "%cd%">>F:\temp\control\%%~na_recover.bat
::for /f "delims=" %%a in ("%str%") do echo attrib -s -h "%cd%\desktop.ini" >>F:\temp\control\%%~na_recover.bat
::for /f "delims=" %%a in ("%str%") do echo del "%cd%\desktop.ini" >>F:\temp\control\%%~na_recover.bat
::for /f "delims=" %%a in ("%str%") do echo del "F:\temp\control\%%~na_recover.bat" >>F:\temp\control\%%~na_recover.bat
cacls "%cd%" /t /c /e /p %username%:n