#!/bin/bash

# Remove the epi2 directory from the user's home directory
rm -rf $HOME/epi2

# Remove the path from .bashrc
sed -i '/$HOME\/epi2/d' ~/.bashrc

# Source the .bashrc to apply changes
source ~/.bashrc