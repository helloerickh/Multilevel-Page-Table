#include "PageTable.h"
#include <stdio.h>

PageTable::PageTable(std::vector<unsigned int> pageSizes){
    levelCount = pageSizes.size();
    //keep track of first untouched bit in ADDRESS_SIZE bit address
    int currBit = 0;
    for(int i = 0; i < levelCount; i++){
        //push BitShift to back of vector
        shiftInfo.push_back(generateBitShift(ADDRESS_SIZE, pageSizes[i], currBit));
        //push BitMask to back of vector
        bitmasks.push_back(generateBitMask(pageSizes[i], ADDRESS_SIZE - currBit));
        //push 2^pageSizes[i] to back of vector
        numEntriesPerLevel.push_back((1 << pageSizes[i]));
        currBit += pageSizes[i];
    }
    //TODO Level Constructor

}

void PageTable::printBitMasks(){
    printf("BITMASKS \n");
    for(auto i : bitmasks){
        printf("%X ", i);
    }
    printf("\n");
}

void PageTable::printBitShifts(){
    printf("SHIFT INFO \n");
    for(auto i : shiftInfo){
        printf("%d ", i);
    }
    printf("\n");
}

void PageTable::printEntriesPerLevel(){
    printf("ENTRIES PER LEVEL \n");
    for(auto i : numEntriesPerLevel){
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