#include "TLB.h"

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794135
*/

/*TLB Constructor
- initialize map for cache
- map has KEY: VPN, VALUE: PFN
- initiailize vector of pairs for LRU
- pair structure holds <VPN, access time> information*/
TLB::TLB(std::vector<unsigned int> levelSizes, unsigned int cacheCap, unsigned int lruCap){
    this->cacheCap = cacheCap;
    this->currCacheCap = 0;
    //check if cache is smaller than max lru size
    if(cacheCap < MAX_LRU_SIZE){
        //lru size = cache size
        this->lruCap = cacheCap;
    }
    else{
        //cache bigger than lru, lru size set to max lru size
        this->lruCap = MAX_LRU_SIZE;
    }
    this->currLRUCap = 0;
    
    //track size of VPN
    unsigned int currVpnSize = 0;
    //to avoid "magic number" when generating VPN bitshift
    unsigned int vpnMSBPos = 0;
    for(int i = 0; i < levelSizes.size(); i++){
        currVpnSize += levelSizes[i];
    }
    //generate variables to be used to process virtual addresses, to get VPN
    this->vpnSize = currVpnSize;
    this->vpnShift = generateBitShift(ADDRESS_SIZE, this->vpnSize, vpnMSBPos);
    this->vpnMask = generateBitMask(this->vpnSize, ADDRESS_SIZE);

    //hit and miss counters
    this->tlbHit = 0;
    this->tlbMiss = 0;

}
unsigned int TLB::tlbLookup(unsigned int virtualAddress, unsigned long currTime, bool& vpnFoundTLB){
    //hold PFN
    unsigned int PFN;
    //Extract VPN
    unsigned VPN = virtualAddressToPageNum(virtualAddress, this->vpnMask, this->vpnShift);

    //iterate through map to find VPN
    std::map<unsigned int, unsigned int>::iterator it = this->cache.find(VPN);

    //check if VPN is in cache
    if(it == this->cache.end()){
        //VPN not in cache
        this->tlbMiss++;
        return NULL;
    }
    //VPN in cache, get PFN
    PFN = it->second;
    this->tlbHit++;
    vpnFoundTLB = true;

    //flag true if VPN in LRU
    bool lruFlag = false;
    //iterate through LRU to find VPN
    for(int i = 0; i < this->LRU.size(); i++){
        //check if first element of pair is the VPN we want
        if(VPN == this->LRU[i].first){
            //VPN match, update access time for VPN
            this->LRU[i].second = currTime;
            lruFlag = true;
            //stop looking through LRU
            break;
        }       
    }
    //VPN not in LRU
    if(!lruFlag){
        //check if LRU not full
        if(this->currLRUCap < this->lruCap){
            //LRU not full, add VPN and access time to LRU
            this->LRU.push_back(std::make_pair(VPN, currTime));
            //increase LRU capacity tracker
            this->currLRUCap++;

        }
        //LRU is full
        else{
            //get least recently used VPN and replace it with new VPN
            // unsigned int vpnToRemove = lruReplacementPolicy(VPN, currTime);
            // //remove LRU VPN from cache
            // this->cache.erase(vpnToRemove);
            // //decrement current cache capacity
            // this->currCacheCap--;

            //run LRU replacement, does not use vpnToRemove because we are not inserting anything to the cache
            unsigned int vpnToRemove = lruReplacementPolicy(VPN, currTime);
        }
    }
    return PFN;
}


unsigned int TLB::lruReplacementPolicy(unsigned int VPN, unsigned int long currTime){
    //sort LRU in decreasing order of access time
    sort(this->LRU.begin(), this->LRU.end(), sortBySecond);
    //get frame to be removed
    unsigned int pfnToRemove = this->LRU.back().first; 
    //remove VPN with lowest access time
    this->LRU.pop_back();
    //insert new VPN, access time pair
    this->LRU.push_back(std::make_pair(VPN, currTime));
    return pfnToRemove;
}

bool sortBySecond(const std::pair<unsigned int, unsigned int>& x, const std::pair<unsigned int, unsigned int>& y){
    return (x.second > y.second);
}


void TLB::tlbInsert(unsigned int virtualAddress, unsigned int frame, unsigned long currTime){

    //Extract VPN
    unsigned int VPN = virtualAddressToPageNum(virtualAddress, this->vpnMask, this->vpnShift);

    //Is cache full?
    if(this->currCacheCap >= this->cacheCap){
        //yes, full cache
        unsigned int vpnToRemove = lruReplacementPolicy(VPN, currTime);
        //remove return of replacement from cache
        this->cache.erase(vpnToRemove);
        //insert new mapping to cache
        this->cache.insert({VPN, frame});
    }
    else{
        //no, cache not full
        this->cache.insert({VPN, frame});
        this->currCacheCap++;
        //check if lru full
        if(this->currLRUCap >= this->lruCap){
            //yes, LRU full
            //don't do anything with vpnToRemove because cache is not full
            unsigned int vpnToRemove = lruReplacementPolicy(VPN, currTime);
        }
        else{
            //no, LRU not full
            this->LRU.push_back(std::make_pair(VPN, currTime));
            this->currLRUCap++;
        }
    }
}