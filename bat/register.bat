@echo off&color f1
regsvr32/s QvodInsert.dll
set lnk="%APPDATA%\Microsoft\Internet Explorer\Quick Launch\����\QvodPlayer.lnk"
set fn="%~dp0QvodPlayer.exe"
set ico="%~dp0QvodPlayer.exe"
set des="Qvod"
echo Dim WshShell,Shortcut>temp.vbs
echo Dim path>>temp.vbs
echo path=%fn%>>temp.vbs
echo Set WshShell=WScript.CreateObject("WScript.Shell")>>temp.vbs
echo Set Shortcut=WshShell.CreateShortCut(%lnk%) '��ݷ�ʽ����>>temp.vbs
echo Shortcut.TargetPath=path 'Ŀ��>>temp.vbs
echo Shortcut.WindowStyle=1 '����1Ĭ�ϴ��ڼ������3��󻯼������7��С��>>temp.vbs
echo Shortcut.Hotkey="" '��ݼ�>>temp.vbs
echo Shortcut.IconLocation=%ico% 'ͼ��>>temp.vbs
echo Shortcut.Description= %des% '��ע>>temp.vbs
echo Shortcut.WorkingDirectory= "%~dp0" '��ʼλ��>>temp.vbs
echo Shortcut.Save>>temp.vbs
temp.vbs
del temp.vbs
