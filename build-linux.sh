# Install required libraries
sudo apt install g++
sudo apt install curl
sudo apt install ncurses

# Create output folder
mkdir linux-target

# Comile program using g++
g++ src/epi2.cpp -o linux-target/epi2 -lnucurses -lcurl