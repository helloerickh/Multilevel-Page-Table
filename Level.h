#ifndef LEVEL_H
#define LEVEL_H

#include "PageTable.h"
#include "Map.h"
#include "helpers.h"
#include <vector>

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794235
*/

/*LEVEL CLASS
- level implementation for page table
- isLeaf = true if leaf level, else false
- depth = level depth
- entryCount = # of non null pointers in nextLevel
- nextLevel = vector of Level pointers to the next level, only for inner level
- map = vector of Map pointers, only for leaf level
- table = PageTable pointer, to access members*/
class Level{
    public:
    bool isLeaf;
    int depth;
    unsigned int entryCount;
    std::vector<class Level*> nextLevel;
    std::vector<class Map*> map;

    class PageTable* table;

    //CONSTRUCTOR
    Level(PageTable* table, int depth);
};

//ADDITIONAL METHODS
void pageLevelInsert(Level* level, unsigned int address, int frame);
Map* pageLevelLookup(Level* level, unsigned int address);

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
#endif