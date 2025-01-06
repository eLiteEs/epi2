@echo off
:: Script for building epi2
mkdir target
g++ -ffunction-sections -fdata-sections -O3 src/epi.cpp -o target/epi2 -static icon/icon.o -lwininet