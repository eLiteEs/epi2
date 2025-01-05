@echo off

:: Navigate to the user's profile directory
cd %USERPROFILE%

:: Remove the epi2 directory
rmdir /S /Q epi2

:: Remove the path from the PATH environment variable
setlocal enabledelayedexpansion
set "Path=%PATH%"
set "Path=!Path:%USERPROFILE%\epi2=!"
endlocal & setx PATH "%Path:;;=;%"