#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "Level.h"
#include "Map.h"    //return type of pagelookup
#include "helpers.h"
#include <vector>

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274
*/

/*PAGE TABLE CLASS
- single or multilevel page mapping implementation
- levelCount = number of levels
- frameCount = number of assigned physical frames
- pageTableHit = number of found physical frames upon lookup
- pageTableMiss = number of times physical frames were not found upon lookup
- offsetMask = bit mask for getting the offset bits of a virtual address
- offsetSize = number of offset bits
- bitmasks = vector of bit masks to get level[i] page number bits
- shiftInfo = vector number of bits to shift page number bits towards MSB
- numEntriesPerLevel = vector of sizes for each level
- root = pointer to root level*/
class PageTable{
    public:
    int levelCount;
    int frameCount;
    int pageTableHit;
    int pageTableMiss;
    unsigned int offsetMask;
    unsigned int offsetSize;

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

//ADDITIONAL METHODS
unsigned int bytesUsed(PageTable* ptr);
void bytesUsedHelper(Level* ptr, unsigned int &bytes);

#endif

