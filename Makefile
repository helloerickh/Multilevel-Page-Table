CXX = g++ 
CXXFLAGS = -std=c++11 -g

pagingwithtlb: main.o
	$(CXX) $(CXXFLAGS) -o paginwithtlb $^

main.o: main.cpp