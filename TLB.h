#ifndef TLB_H
#define TLB_H

#include "helpers.h"
#include <utility>          //pair
#include <vector>           //vector
#include <map>    //unordered map

#define LRU_SIZE 10

/*
TODO:
- Extract VPN from virtual address
- lrureplacement policy
- tlb lookup
- tlb insert*/

class TLB{
    public:
    unsigned int cacheCap;
    unsigned int currCacheCap;
    unsigned int currLRUCap;


    unsigned int offsetSize;
    unsigned int vpnSize;
    unsigned int vpnMask;
    unsigned int tlbHit;
    unsigned int tlbMiss;

    std::map<unsigned int, unsigned int> cache;
    std::vector<std::pair<unsigned int, unsigned int>> LRU;

    unsigned int tlbLookup(unsigned int virtualAddress, unsigned long currTime);

    void tlbInsert(unsigned int virtualAddress, unsigned int frame ,unsigned long currTime);

    unsigned int lruReplacementPolicy(unsigned int VPN, unsigned long currTime);


    TLB();

};



#endif