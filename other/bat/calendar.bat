@echo off

setlocal enabledelayedexpansion

:::::::::::定义s1～s12，为每个月的天数
for %%b in (31,28,31,30,31,30,31,31,30,31,30,31) do (
set /a y+=1
set s!y!=%%b
) 

:::::::::::判断输入年份是否为闰年，更改2月的天数
set /p year=查几年?

set /a year_chk1=year%%400
set /a year_chk2=year%%4
set /a year_chk3=year%%100
if %year_chk1% equ 0 set s2=29
if %year_chk2% equ 0 if %year_chk3% neq 0 set s2=29

::::::::::输入月份，并计算当年的第一天到上月月底的天数
set /p m=查几月?
set n=%m%
set /a n-=1
for /l %%c in (1,1,%n%) do (
set /a day+=!s%%c!)


::::::::::判断这一年的第一天是星期几？再判断所要查询月份的第一天是星期几？
set /a yearflag=(%year%+(%year%-1)/4-(%year%-1)/100+(%year%-1)/400)%%7
if %m%==1 (set  monthflag=%yearflag% ) else (set /a monthflag=%yearflag%+!day!%%7 )

if %monthflag% geq 7 (set /a monthflag-=7)

echo    日   一   二   三   四   五   六   

:::::::::输入月份1号前的空日期
for /l %%d in (1,1,%monthflag%) do (
set /p=     <nul)
:::::::::找出要换行的日期（第一个，后每7个一换），显示当月日期并换行
set /a cl=7-%monthflag%

for /l %%e in (1 1 !s%m%!) do (
if %%e lss 10 (set /p=    %%e<nul) else (set /p=   %%e<nul)
if %%e==!cl! (echo. &set /a cl+=7)
)
echo.
echo.
echo.
set /p abc = 退出