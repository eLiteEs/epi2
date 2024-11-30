# Install required libraries
sudo apt install g++
sudo apt install curl
sudo apt isntall ncurses

# Test files
g++ -Wall -Wextra -pedantic src/epi2.cpp
g++ -Wall -Wextra -pedantic src/downloadbar-utils.cpp
g++ -Wall -Wextra -pedantic src/license.cpp
g++ -Wall -Wextra -pedantic src/noerrorfile.cpp