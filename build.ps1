# Script for building epi2
mkdir target
g++ src/epi.cpp -o target/epi2 -static -O3 icon/icon.o -lwininet