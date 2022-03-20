#include "Level.h"

Level::Level(PageTable* table, int depth){
    this->table = table;
    this->depth = depth;
    //check if leaf node
    if((depth + 1) >= table->levelCount){
        this->isLeaf = true;
    }
    else{
        this->isLeaf = false;
    }
    //if leaf push Map pointers to vector, else push NULL to nextLevel vector
    if(this->isLeaf){
        for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
            Map* temp = new Map();
            this->map.push_back(temp);
        } 
    }
    else{
        for(int i = 0; i < table->numEntriesPerLevel[depth]; i++){
            this->nextLevel.push_back(NULL);
        }
    }
}