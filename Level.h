#ifndef LEVEL_H
#define LEVEL_H

#include "PageTable.h"
#include "Map.h"
#include <vector>

class Level{
    public:
    bool isLeaf;
    int depth;
    std::vector<class Level*> nextLevel;
    std::vector<class Map*> map;

    class PageTable* table;

    //CONSTRUCTOR
    Level();
    Level(PageTable* table, int depth);
};

void pageLevelInsert(Level* level, unsigned int address, int frame);
Map* pageLevelLookup(Level* level, unsigned int address);

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
#endif