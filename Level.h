#ifndef LEVEL_H
#define LEVEL_H

#include "PageTable.h"
#include "Map.h"

class Level{
    public:
    int currDepth;
    class PageTable* table;

    Level();
};
#endif