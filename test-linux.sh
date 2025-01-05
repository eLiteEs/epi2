# Install required libraries
apt-get install g++
apt-get install curl
apt-get install ncurses

# Test files
g++ -Wall -Wextra -pedantic src/epi.cpp
g++ -Wall -Wextra -pedantic src/downloadbar-utils.cpp
g++ -Wall -Wextra -pedantic src/license.cpp
g++ -Wall -Wextra -pedantic src/noerrorfile.cpp