#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "Level.h"
#include "Map.h"    //return type of pagelookup
#include <vector>

#define ADDRESS_SIZE 32

class PageTable{
    public:
    int levelCount;
    int frameCount;
    int pageTableHit;
    int pageTableMiss;
    unsigned int offsetMask;

    std::vector<unsigned int> bitmasks;
    std::vector<unsigned int> shiftInfo;
    std::vector<unsigned int> numEntriesPerLevel;

    class Level* root;
    //CONSTRUCTOR
    PageTable(std::vector<unsigned int> pageSizes);
    //METHODS
    void pageTableInsert(unsigned int address);
    class Map* pageTableLookup(unsigned int address);
    unsigned int getOffset(unsigned int address);
    void printPageTable();
};

unsigned int generateBitShift(unsigned int addressSize, unsigned int pageSize, int currBit);
unsigned int generateBitMask(unsigned int length, unsigned int start);

#endif

