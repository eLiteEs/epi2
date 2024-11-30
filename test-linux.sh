# Install required libraries
apt-get g++
apt-get curl
apt-get ncurses

# Test files
g++ -Wall -Wextra -pedantic src/epi2.cpp
g++ -Wall -Wextra -pedantic src/downloadbar-utils.cpp
g++ -Wall -Wextra -pedantic src/license.cpp
g++ -Wall -Wextra -pedantic src/noerrorfile.cpp