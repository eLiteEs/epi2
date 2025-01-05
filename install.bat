@echo off

:: Build the program for obtaining the executables
.\build.bat

:: Make a folder in which the program is going to be installed
cd %USERPROFILE%
mkdir epi2

:: Navigate back to the directory where the script was run 
cd -

:: Move build files to the installation folder
move target\epi2.exe %USERPROFILE%\epi2
move LICENSE %USERPROFILE%\epi2
move uninstall.bat %USERPROFILE%\epi2

:: Add a directory to the PATH environment variable 
setx PATH "%PATH%;C:\Users\%USERPROFILE%\epi2\epi2.exe"