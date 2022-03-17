CXX=g++ 
CXXFLAGS=-std=c++11 -g

pagingwithtlb: main.o tracereader.o output_mode_helpers.o
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o: main.cpp tracereader.h output_mode_helpers.h

tracereader.o: tracereader.h tracereader.cpp

output_mode_helpers.o: output_mode_helpers.h output_mode_helpers.cpp

Level.o: Level.h Level.cpp

Map.o: Map.h Map.cpp

PageTable.o: Map.h Level.h PageTable.h PageTable.cpp

clean: 
	rm *.o