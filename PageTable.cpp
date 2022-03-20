#include "PageTable.h"
#include <stdio.h>

PageTable::PageTable(std::vector<unsigned int> pageSizes){
    this->levelCount = pageSizes.size();
    this->frameCount = 0;
    this->pageTableHit = 0;
    this-> pageTableMiss = 0;
    //keep track of first untouched bit in ADDRESS_SIZE bit address
    int currBit = 0;
    for(int i = 0; i < levelCount; i++){
        //push BitShift to back of vector
        this->shiftInfo.push_back(generateBitShift(ADDRESS_SIZE, pageSizes[i], currBit));
        //push BitMask to back of vector
        this->bitmasks.push_back(generateBitMask(pageSizes[i], ADDRESS_SIZE - currBit));
        //push 2^pageSizes[i] to back of vector
        this->numEntriesPerLevel.push_back((1 << pageSizes[i]));
        currBit += pageSizes[i];
    }
    //TODO Level Constructor
    this->root = new Level(this, 0);

}

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

//Generate number of bits to shift given address size, page size, and start
unsigned int generateBitShift(unsigned int addressSize, unsigned int pageSize, int currBit){
    //calculate how many bits to shift over depending on how many bits there are and where they begin
    unsigned int bitShift = addressSize - currBit - pageSize;
    return bitShift;
}

//Generate a bitmask given the length of the mask and where the mask begins
unsigned int generateBitMask(unsigned int length, unsigned int start){
    //intitialize length number of 1s from MSB side
    unsigned int bitMask = (1 << length) - 1;
    //shift 1s to start bit
    bitMask <<= (start - length);
    return bitMask;
}