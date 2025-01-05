#!/bin/bash

# Install required libraries
apt-get install g++
apt-get install curl
apt-get install ncurses

# Create output folder
mkdir target

# Comile program using g++
g++ src/epi.cpp -o target/epi2 -lnucurses -lcurl -O3 icon/icon.o