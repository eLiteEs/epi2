# Build the program for obtaining the executables
.\build.ps1

# Make a folder in which the program is going to be installed
$UserProfile = [System.Environment]::GetFolderPath('UserProfile')
$InstallPath = Join-Path $UserProfile "epi2"
if (-not (Test-Path $InstallPath)) {
    New-Item -ItemType Directory -Path $InstallPath
}

# Move build files to the installation folder
Move-Item -Path .\target\epi2.exe -Destination $InstallPath
Move-Item -Path .\LICENSE -Destination $InstallPath
Move-Item -Path .\uninstall.ps1 -Destination $InstallPath

# Add a directory to the PATH environment variable
$Path = [System.Environment]::GetEnvironmentVariable("Path", [System.EnvironmentVariableTarget]::User)
$NewPath = "$Path;$InstallPath"
[System.Environment]::SetEnvironmentVariable("Path", $NewPath, [System.EnvironmentVariableTarget]::User)