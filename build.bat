mkdir target
g++ src/epi.cpp -o target/epi2 -static -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32