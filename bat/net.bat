@echo off
::for /l %%i in (0,1,255) do (for /l %%j in (0,1,255) do (for /l %%k in (0,1,255) do @echo %%i,%%j,%%k)) >>net.txt
setlocal enabledelayedexpansion

    set VAR=before
    if "%VAR%" == "before" ( set VAR=after
        if "!VAR!" == "after" @echo If you see this, it worked )