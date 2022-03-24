CXX=g++ 
CXXFLAGS=-std=c++11 -g

pagingwithtlb: main.o tracereader.o output_mode_helpers.o PageTable.o Level.o Map.o helpers.o TLB.o
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o: main.cpp tracereader.h output_mode_helpers.h PageTable.h helpers.h

helpers.o: helpers.h helpers.cpp

PageTable.o: PageTable.h PageTable.cpp

Level.o: Level.h Level.cpp 

Map.o: Map.h Map.cpp

TLB.o: TLB.h TLB.cpp

tracereader.o: tracereader.h tracereader.cpp

output_mode_helpers.o: output_mode_helpers.h output_mode_helpers.cpp

clean: 
	rm *.o