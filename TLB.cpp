#include "TLB.h"

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794235
*/

TLB::TLB(std::vector<unsigned int> levelSizes, unsigned int cacheCap, unsigned int lruCap){
    this->cacheCap = cacheCap;
    this->currCacheCap = 0;
    this->lruCap = lruCap;
    this->currLRUCap = 0;
    
    //track size of VPN
    unsigned int currVpnSize = 0;
    //to avoid "magic number" when generating VPN bitshift
    unsigned int vpnMSBPos = 0;
    for(int i = 0; i < levelSizes.size(); i++){
        currVpnSize += levelSizes[i];
    }
    this->vpnSize = currVpnSize;
    this->vpnShift = generateBitShift(ADDRESS_SIZE, this->vpnSize, vpnMSBPos);
    this->vpnMask = generateBitMask(this->vpnSize, ADDRESS_SIZE);

    this->tlbHit = 0;
    this->tlbMiss = 0;

}
unsigned int TLB::tlbLookup(unsigned int virtualAddress, unsigned long currTime){
    unsigned int PFN;
    //TODO Extract VPN
    unsigned VPN = 0;
    std::map<unsigned int, unsigned int>::iterator it = this->cache.find(virtualAddress);
    //check if VPN is in cache
    if(it == this->cache.end()){
        //VPN not in cache
        this->tlbMiss++;
        return NULL;
    }
    //get PFN
    PFN = it->second;
    this->tlbHit++;

    //flag if in LRU
    bool lruFlag = false;
    //look for VPN in LRU
    for(int i = 0; i < this->LRU.size(); i++){
        //VPN found in LRU
        if(VPN == this->LRU[i].first){
            //update access time for VPN
            this->LRU[i].second = currTime;
            lruFlag = true;
            break;
        }       
    }
    //VPN not in LRU
    if(!lruFlag){
        //check if LRU not full
        if(this->currLRUCap < LRU_SIZE){
            //add VPN and access time to LRU
            this->LRU.push_back(std::make_pair(VPN, currTime));
            //increase LRU capacity tracker
            this->currLRUCap++;

        }
        //LRU is full
        else{
            //get least recently used VPN and replace it with new VPN
            unsigned int vpnToRemove = lruReplacementPolicy(VPN, currTime);
            //remove LRU VPN from cache
            this->cache.erase(vpnToRemove);
            //decrement current cache capacity
            this->currCacheCap--;
        }
    }
    return PFN;
}

unsigned int TLB::lruReplacementPolicy(unsigned int VPN, unsigned int long currTime){
    
}

void TLB::tlbInsert(unsigned int virtualAddress, unsigned int frame, unsigned long currTime){
    //TODO Extract VPN
    unsigned int VPN = 0;


}