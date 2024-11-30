# Install required libraries
apt-get install g++
apt-get install curl
apt-get install ncurses

# Create output folder
mkdir linux-target

# Comile program using g++
g++ src/epi2.cpp -o linux-target/epi2 -lnucurses -lcurl