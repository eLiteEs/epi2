# Install required libraries
apt install g++
apt install curl
apt install ncurses

# Create output folder
mkdir linux-target

# Comile program using g++
g++ src/epi2.cpp -o linux-target/epi2 -lnucurses -lcurl