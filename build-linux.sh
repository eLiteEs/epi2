# Install required libraries
apt-get g++
apt-get curl
apt-get ncurses

# Create output folder
mkdir linux-target

# Comile program using g++
g++ src/epi2.cpp -o linux-target/epi2 -lnucurses -lcurl