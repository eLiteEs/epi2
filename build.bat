echo "Analyzing epi.cpp"
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/epi.cpp -o temp_exec 2>&1
echo "Analyzing downloadbar-utils.cpp"
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/downloadbar-utils.cpp -o temp_exec 2>&1
echo "Analyzing noerrorfile.cpp"
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/noerrorfile.cpp -o temp_exec 2>&1
echo "Analyzing license.cpp"
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/license.cpp -o temp_exec 2>&1
echo "Compiling epi.cpp into epi2.exe"
mkdir target
g++ src/epi.cpp -o target/epi2 -static -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32