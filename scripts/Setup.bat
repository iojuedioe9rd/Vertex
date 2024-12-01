@echo off
python Setup.py %*
if "%1"=="cla" (
    echo First argument is "cla"
) else (
    PAUSE
)
