#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "Level.h"
#include "Map.h"

class PageTable{
    public:
    int levelCount;
    class Level* root;
    PageTable();
};
#endif