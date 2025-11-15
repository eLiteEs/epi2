## On Windows
### Downlaoding the installer from the web
You can download a installer for Windows on the epi² web. You can decide between .msi or .exe installer. Those installers are only for Windows machines with 64 bit processors.

You download the installer and it automatically setups epi² on your computer. It also setups the `PATH Variable` and file asociations.
### Downloading the epi² portable version
You can download the epi² excutable in only one file on the portable version, that version can be for 64 bit and 32 bit computers. It has the extension of a .zip file.

You have to manually setup the `PATH Variable` for running it on the terminal and optionally file asociations.

## On Linux
### Downloading the portable version from the web
You can download the .zip file containing the epi² executables for Linux from the epi² web.

You have to manually setup the `PATH Variable` for running it on the terminal and optionally file asociations.

### Building from source

1. Clone the git repo of epi2 running `git clone https://github.com/eLiteEs/epi2.git`
2. From the repo, run `make install`. That command builds epi2 and moves all the files to a new directory. It also prepared the `.bashrc` with the new epi2 executable.
3. Run `source ~/.bashrc` for configurating properly the PATH variable and running epi2 from anywhere.

## On MacOS
We tried our best trying to compile epi² for MacOS using GitHub Actions but it didn't worked because it doesn't work at all.

So you have to download the source code and compile it manually. You have to install some dependencies like `curl` and `ncurses`.
