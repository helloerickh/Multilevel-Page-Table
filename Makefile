CXX = g++ 
CXXFLAGS = -std=c++11 -g

pagingwithtlb: main.o
	$(CXX) $(CXXFLAGS) -o paginwithtlb $^

main.o: main.cpp

output_mode_helpers.o: output_mode_helpers.h output_mode_helpers.cpp

tracereader.o: tracereader.h tracereader.cpp