g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/epi.cpp -o temp_exec 2>&1
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/downloadbar-utils.cpp -o temp_exec 2>&1
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/noerrorfile.cpp -o temp_exec 2>&1
g++ -Wall -Wextra -pedantic -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 src/license.cpp -o temp_exec 2>&1
mkdir target
g++ -lwinmm -lole32 -lgdi32 -lwinspool -lwlanapi -lWs2_32 -lwininet -lwinhttp -liphlpapi -lurlmon -lcomctl32 -static src/epi.cpp -o epi2