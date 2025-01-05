# Get the UserProfile path
$UserProfile = [System.Environment]::GetFolderPath('UserProfile')
$InstallPath = Join-Path $UserProfile "epi2"

# Remove the epi2 directory
if (Test-Path $InstallPath) {
    Remove-Item -Recurse -Force $InstallPath
}

# Remove the path from the PATH environment variable
$Path = [System.Environment]::GetEnvironmentVariable("Path", [System.EnvironmentVariableTarget]::User)
$NewPath = ($Path -split ';' | Where-Object { $_ -ne $InstallPath }) -join ';'
[System.Environment]::SetEnvironmentVariable("Path", $NewPath, [System.EnvironmentVariableTarget]::User)