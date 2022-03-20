#include "PageTable.h"
#include <stdio.h>

/*Page Table Constructor
INPUT: vector of unsigned integers representing page sizes
- Initialize class members and pointer to root level*/
PageTable::PageTable(std::vector<unsigned int> pageSizes){
    this->levelCount = pageSizes.size();
    this->frameCount = 0;
    this->pageTableHit = 0;
    this-> pageTableMiss = 0;
    unsigned int offsetLength = 0;
    //keep track of first untouched bit in ADDRESS_SIZE bit address
    int currBit = 0;
    for(int i = 0; i < levelCount; i++){
        //increment offsetLength
        offsetLength += pageSizes[i];
        //push BitShift to back of vector
        this->shiftInfo.push_back(generateBitShift(ADDRESS_SIZE, pageSizes[i], currBit));
        //push BitMask to back of vector
        this->bitmasks.push_back(generateBitMask(pageSizes[i], ADDRESS_SIZE - currBit));
        //push 2^pageSizes[i] to back of vector
        this->numEntriesPerLevel.push_back((1 << pageSizes[i]));
        currBit += pageSizes[i];
    }
    this->offsetMask = generateBitMask(offsetLength, offsetLength);
    //construct root level
    this->root = new Level(this, 0);

}

/*Assign virtual address to physical frame
INPUT: unsigned int virtual address
- Starts insertion from root*/
void PageTable::pageTableInsert(unsigned int address){
    pageLevelInsert(this->root, address, this->frameCount);
}

/*Find physical frame for a virtual address
INPUT: unsigned int virtual address
RETURN: Map pointer for physical frame
- Starts lookup from root*/
Map* PageTable::pageTableLookup(unsigned int address){
    pageLevelLookup(this->root, address);
}

/*Get offset of a virtual address
INPUT: unsigned int virtual address
RETURN: offset bits*/
unsigned int PageTable::getOffset(unsigned int address){
    unsigned int offset = address;
    offset &= this->offsetMask;
    return offset;
}

/*Print out Page Table class members*/
void PageTable::printPageTable(){
    printf("LEVELCOUNT: %d FRAMECOUNT: %d TABLEHIT: %d TABLEMISS: %d \n", this->levelCount, this->frameCount, this->pageTableHit, this->pageTableMiss);
    printf("BITMASKS \n");
    for(auto i : this->bitmasks){
        printf("%X ", i);
    }
    printf("\n");

    printf("SHIFT INFO \n");
    for(auto i : this->shiftInfo){
        printf("%d ", i);
    }
    printf("\n");

    printf("ENTRIES PER LEVEL \n");
    for(auto i : this->numEntriesPerLevel){
        printf("%d ", i);
    }
    printf("\n");
}

/*Generate number of bits to shift given address size, page size, and start
INPUT: unsigned int address size and page size, int current bit
RETURN: unsigned int bit shift
- Bit shift represents number of bits to shift page number to the right
- Bit shifted page number is used to traverse page table*/
unsigned int generateBitShift(unsigned int addressSize, unsigned int pageSize, int currBit){
    //calculate how many bits to shift over depending on how many bits there are and where they begin
    unsigned int bitShift = addressSize - currBit - pageSize;
    return bitShift;
}

/*Generate a bitmask given the length of the mask and where the mask begins
INPUT: unsigned int length of bitmask, unsigned int start of MSB side of bit mask
RETURN: unsigned int bit mask
- Bit mask is used to isolate page numbers for level
- start = length, results in zero shift, good for offset mask*/
unsigned int generateBitMask(unsigned int length, unsigned int start){
    //intitialize length number of 1s from MSB side
    unsigned int bitMask = (1 << length) - 1;
    //shift 1s to start bit
    bitMask <<= (start - length);
    return bitMask;
}