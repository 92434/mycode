@echo off

setlocal enabledelayedexpansion

:::::::::::����s1��s12��Ϊÿ���µ�����
for %%b in (31,28,31,30,31,30,31,31,30,31,30,31) do (
set /a y+=1
set s!y!=%%b
) 

:::::::::::�ж���������Ƿ�Ϊ���꣬����2�µ�����
set /p year=�鼸��?

set /a year_chk1=year%%400
set /a year_chk2=year%%4
set /a year_chk3=year%%100
if %year_chk1% equ 0 set s2=29
if %year_chk2% equ 0 if %year_chk3% neq 0 set s2=29

::::::::::�����·ݣ������㵱��ĵ�һ�쵽�����µ׵�����
set /p m=�鼸��?
set n=%m%
set /a n-=1
for /l %%c in (1,1,%n%) do (
set /a day+=!s%%c!)


::::::::::�ж���һ��ĵ�һ�������ڼ������ж���Ҫ��ѯ�·ݵĵ�һ�������ڼ���
set /a yearflag=(%year%+(%year%-1)/4-(%year%-1)/100+(%year%-1)/400)%%7
if %m%==1 (set  monthflag=%yearflag% ) else (set /a monthflag=%yearflag%+!day!%%7 )

if %monthflag% geq 7 (set /a monthflag-=7)

echo    ��   һ   ��   ��   ��   ��   ��   

:::::::::�����·�1��ǰ�Ŀ�����
for /l %%d in (1,1,%monthflag%) do (
set /p=     <nul)
:::::::::�ҳ�Ҫ���е����ڣ���һ������ÿ7��һ��������ʾ�������ڲ�����
set /a cl=7-%monthflag%

for /l %%e in (1 1 !s%m%!) do (
if %%e lss 10 (set /p=    %%e<nul) else (set /p=   %%e<nul)
if %%e==!cl! (echo. &set /a cl+=7)
)
echo.
echo.
echo.
set /p abc = �˳�