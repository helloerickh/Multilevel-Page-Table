#include "Level.h"

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794235
*/

/*Level Constructor
- initialize class members
- if level is a leaf level, initialize vector of Map pointers
- if level is an inner level, initialize vector of Level pointers
- level pointers are set to NULL*/
Level::Level(PageTable* table, int depth){
    this->table = table;
    this->depth = depth;
    this->entryCount = 0;
    //check if leaf node
    if((depth + 1) >= table->levelCount){
        this->isLeaf = true;
    }
    else{
        this->isLeaf = false;
    }
    //if leaf push Map pointers to vector
    if(this->isLeaf){
        // for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
        //     this->map.push_back(new Map());
        // } 
        this->map.resize(table->numEntriesPerLevel[depth]);
        for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
            this->map[i] = new Map();
        }
    }
    //if non leaf, push Level pointers to vector and set them to null
    else{
        // for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
        //     this->nextLevel.push_back(new Level());
        //     this->nextLevel[i] = nullptr;
        // }
        this->nextLevel.resize(table->numEntriesPerLevel[depth]);
        for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
            this->nextLevel[i] = nullptr;
        }
    }
}

/*Assign virtual address to physical frame
INPUT: Level pointer, unsigned int virtual address, int physical frame
- get page index from virtualAddressToPageNum
- if leaf node, initialize corresponding map members
- if inner level, check if corresponding level exists, else create level
- traverse to the next level*/
void pageLevelInsert(Level* level, unsigned int address, int frame){
    //get page index for current level
    unsigned int pageIndex = virtualAddressToPageNum(address, level->table->bitmasks[level->depth], level->table->shiftInfo[level->depth]);
    //check if current level has leaf nodes
    if(level->isLeaf){
        //set valid mapping, assign frame number, then increment frame
        level->map[pageIndex]->isValid = true;
        level->map[pageIndex]->frame = frame;
        level->table->frameCount++;
        level->entryCount++;
    }
    //inner level
    else{
        //check if page does not exist, nullptr
        if(!(level->nextLevel[pageIndex])){
            //create new page, with appropriate arguments
            level->nextLevel[pageIndex] = new Level(level->table, (level->depth) + 1);
            level->entryCount++;
        }
        //move to next page
        pageLevelInsert(level->nextLevel[pageIndex], address, frame);
    }
}

/*Find physical frame for virtual address
INPUT: Level pointer, unsigned int virtual address
RETURN: true if frame found, else false
- if at any point a page is not found, return false*/
Map* pageLevelLookup(Level* level, unsigned int address){
    //get page index for current level
    unsigned int pageIndex = virtualAddressToPageNum(address, level->table->bitmasks[level->depth], level->table->shiftInfo[level->depth]);
    //check if leaf node
    if(level->isLeaf){
        //check if valid frame
        if(level->map[pageIndex]->isValid){
            return level->map[pageIndex];
        }
        else{
            return nullptr;
        }
    }
    //check if page exists
    else{
        if(!(level->nextLevel[pageIndex])){
            return nullptr;
        }
        pageLevelLookup(level->nextLevel[pageIndex], address);
    }
}