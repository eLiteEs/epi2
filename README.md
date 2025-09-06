[![Build C++](https://github.com/eLiteEs/epi2/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/eLiteEs/epi2/actions/workflows/build.yml)[![Analyze C++](https://github.com/eLiteEs/epi2/actions/workflows/analyze.yml/badge.svg?branch=main)](https://github.com/eLiteEs/epi2/actions/workflows/analyze.yml)[![Build Installers](https://github.com/eLiteEs/epi2/actions/workflows/build-installers.yml/badge.svg?branch=main)](https://github.com/eLiteEs/epi2/actions/workflows/build-installers.yml)![Estado](https://img.shields.io/badge/State-On%20Development-yellow) 
![Languages](https://img.shields.io/github/languages/top/eLiteEs/epi2)![Licencia](https://img.shields.io/github/license/eLiteEs/epi2)![Version](https://img.shields.io/badge/Version-0.202-blue)
![Descargas](https://img.shields.io/github/downloads/eLiteEs/epi2/total)![Issues](https://img.shields.io/github/issues/eLiteEs/epi2)![Último Commit](https://img.shields.io/github/last-commit/eLiteEs/epi2)![Tamaño](https://img.shields.io/github/repo-size/eLiteEs/epi2)![Plataforms](https://img.shields.io/badge/Platforms-Windows%20|%20Linux-blue)![Pull Requests](https://img.shields.io/github/issues-pr/eLiteEs/epi2)![Contribuir](https://img.shields.io/badge/Contributions-Welcome-brightgreen)

---

# epi2
In this repo you can find the code of epi2 programming language.
## Table of contents
- [Pre-Requisites for installing epi2](#pre-requisites-for-installing-epi2)
- [Installation for Windows using cmd](#installation-for-windows-using-cmd)
- [Installation for Windows using PowerShell](#installation-for-windows-using-powershell)
- [Installation for Linux using Shell](#installation-for-linux-using-shell)
- [Colaborate to this project](#colaborating-to-this-project)
- [License](#license)

## Pre-Requisites for installing epi2
- Have installed MSYS mingw64 for using `g++` on the console or install `gcc` package on Linux.
- Optionally have Git, GitHub Desktop or GitHub CLI installed for commits directly from console
- For making the icon be an object file I used WiX Toolset

## Installation for Windows using Command Prompt
1. Clone or download this repository to a folder. Git: `git clone https://github.com/eLiteEs/epi2.git` GitHub CLI: `gh repo clone eLiteEs/epi2` or download the code from GitHub.
2. Open the folder and run the script `install.bat`. That script will build the project using `g++`, create a folder on your user folder, move all epi2 files into that folder and add epi2 to `PATH` enviroment variable.
3. Then you can use `epi2` on Terminal.
> **Note**: You should have `g++` installed for installing epi2. Until I don't sign the program, this will be required, if not Windows Defender will mark this software as malware. 

## Installation for Windows using PowerShell
1. Clone or download this repository to a folder. Git: `git clone https://github.com/eLiteEs/epi2.git` GitHub CLI: `gh repo clone eLiteEs/epi2` or download the code from GitHub.
2. Open the folder and run the script `install.ps1`. That script will build the project using `g++`, create a folder on your user folder, move all epi2 files into that folder and add epi2 to `PATH` enviroment variable.
3. Then you can use `epi2` on Terminal.
> **Note**: You should have `g++` installed for installing epi2. Until I don't sign the program, this will be required, if not Windows Defender will mark this software as malware. 

## Installation for Linux using Shell
1. Clone or download this repository to a folder. Git: `git clone https://github.com/eLiteEs/epi2.git` GitHub CLI: `gh repo clone eLiteEs/epi2` or download the code from GitHub.
2. Open the folder on the Terminal and run the script `./install.sh`. That script will build the project using `g++`, create a folder on your user folder, move all epi2 files into that folder and add epi2 to `PATH` enviroment variable.
> **Note**: You should have `g++` installed for installing epi2.

## Colaborating to this project
### Table of contents
1. [Creating and Issue](#creating-an-issue)
2. [Adding things to the documentation](#adding-things-to-documentation)
3. [Cloning this repository](#cloning-this-repository)
4. [Writing the Code](#writing-the-code)
5. [Building the Code](#building-the-code)
6. [Creating Pull Requests](#creating-pull-requests)
7. [Forking this repository](#forking-this-repository)

### Creating an Issue
You can write an Issue on GitHub if you're finding a bug on the program or if you want to add some documentation for the program by doing this:
- **If it's a bug**:
    1. Be sure to add a description about how to recreate it.
    2. You can add the platform in which you're finding the bug.
    3. Also add the expected output into that problem
    4. If you want add a possible way of fixing it.

- **If you want to add documentation**:
You can create issues if you think that documentation is missing or if you think that it misght be wrong. You can also edit files of the documentaion and create pull requests.

## Adding things to the documentation
If you find some documentation weak or if it contains errors, you can edit them and create a pull request adding information about what was wrong and what changes you made.


## Cloning this repository
You can clone this repository using Git like this:
```
git clone https://YourUsename@github.com/eLiteEs/epi2.git
```

## Writing the code
Try to comment all the code that you write.
Don't change the tab indent on the files, normally epi2 is written with 4 spaces indent. 

## Building the project
On Windows you can run `build.bat` or `build.ps1` scripts for building the executables of epi2. You can also run the `test.exe` file for checking if the code has some performance issues.
On Linux you can use the `./build.sh` script for building the project and `./test-linux.sh` script for checking if the code has performance issues.

## Creating Pull Requests
If you create a Pull Request add a description about what did you changed and how it was before. If it's a new feature be sure that it works correctly.

## Forking this repository
You can use epi2 for creating you own programming language but you may add the `²` symbol to the name.

## License
This project is licensed by the GNU General Public License v3 available at [LICENSE](LICENSE) file.