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

    Level();
};
#endif