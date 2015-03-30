@echo off&color f1
regsvr32/s QvodInsert.dll
set lnk="%APPDATA%\Microsoft\Internet Explorer\Quick Launch\网络\QvodPlayer.lnk"
set fn="%~dp0QvodPlayer.exe"
set ico="%~dp0QvodPlayer.exe"
set des="Qvod"
echo Dim WshShell,Shortcut>temp.vbs
echo Dim path>>temp.vbs
echo path=%fn%>>temp.vbs
echo Set WshShell=WScript.CreateObject("WScript.Shell")>>temp.vbs
echo Set Shortcut=WshShell.CreateShortCut(%lnk%) '快捷方式名称>>temp.vbs
echo Shortcut.TargetPath=path '目标>>temp.vbs
echo Shortcut.WindowStyle=1 '参数1默认窗口激活，参数3最大化激活，参数7最小化>>temp.vbs
echo Shortcut.Hotkey="" '快捷键>>temp.vbs
echo Shortcut.IconLocation=%ico% '图标>>temp.vbs
echo Shortcut.Description= %des% '备注>>temp.vbs
echo Shortcut.WorkingDirectory= "%~dp0" '起始位置>>temp.vbs
echo Shortcut.Save>>temp.vbs
temp.vbs
del temp.vbs
