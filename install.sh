#!/bin/bash

# Make uninstall and build scripts executable
chmod +x uninstall.sh
chmod +x build.sh

# Build the program for obtaining the executables
./build.sh

# Make a folder in which the program is going to be installed
cd $HOME
mkdir -p epi2

# Navigate back to the directory where the script was run
cd -

# Move build files to the installation folder
mv target/epi2 $HOME/epi2
mv LICENSE $HOME/epi2

# Add a directory to the PATH environment variable
echo 'export PATH=$PATH:$HOME/epi2' >> ~/.bashrc
source ~/.bashrc