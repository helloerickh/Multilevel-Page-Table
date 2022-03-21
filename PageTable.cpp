#include "PageTable.h"
#include <stdio.h>

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274
*/

/*Page Table Constructor
INPUT: vector of unsigned integers representing page sizes
- Initialize class members and pointer to root level*/
PageTable::PageTable(std::vector<unsigned int> pageSizes){
    this->levelCount = pageSizes.size();
    this->frameCount = 0;
    this->pageTableHit = 0;
    this-> pageTableMiss = 0;
    //keep track of first untouched bit in ADDRESS_SIZE-bit address
    int currBit = 0;
    for(int i = 0; i < this->levelCount; i++){
        //push BitShift to back of vector
        this->shiftInfo.push_back(generateBitShift(ADDRESS_SIZE, pageSizes[i], currBit));
        //push BitMask to back of vector
        this->bitmasks.push_back(generateBitMask(pageSizes[i], ADDRESS_SIZE - currBit));
        //push 2^pageSizes[i] to back of vector
        this->numEntriesPerLevel.push_back((1 << pageSizes[i]));
        currBit += pageSizes[i];
    }
    //currbit should now be at the first LSB of offset
    this->offsetSize = ADDRESS_SIZE - currBit;
    this->offsetMask = generateBitMask(ADDRESS_SIZE - currBit, ADDRESS_SIZE - currBit);
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

/*Calculate the total number of bytes used by PageTable
INPUT: PageTable pointer
RETURN: unsigned int bytes used*/
unsigned int bytesUsed(PageTable* ptr){
    unsigned int bytes = 0;
    bytesUsedHelper(ptr->root, bytes);
    return bytes;
}

/*Calculate the total number of bytes used for all levels in PageTable
INPUT: Level pointer, unsigned int reference to bytesUsed
- total bytes used by inner level = # non null entries + size of level + size of nextLevel vector
- total bytes used by leaf level = # non null entries + size of level + size of map vector*/
void bytesUsedHelper(Level* ptr, unsigned int &bytes){
    //bytes used by entries
    bytes += ptr->entryCount * sizeof(*ptr);
    //size of curr level
    bytes += sizeof(*ptr);
    if(!ptr->isLeaf){
        //bytes used by nextlevel
        bytes += sizeof(ptr->nextLevel) * ptr->nextLevel.size();
        for(int i = 0; i < ptr->table->numEntriesPerLevel[ptr->depth]; i++){
            if(ptr->nextLevel[i]){
                bytesUsedHelper(ptr->nextLevel[i], bytes);
            }
        }   
    }
    else{
        //bytes used by map
        bytes+= sizeof(ptr->map) * ptr->map.size();
    }
    return;
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