#ifndef HELPERS_H
#define HELPERS_H

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794235
*/

//address size
#define ADDRESS_SIZE 32

//number of addresses to track in TLB
#define MAX_LRU_SIZE 10

//DEFAULT VALUES
#define DEFAULT_CACHE_CAP 0
#define DEFAULT_MODE 0
/*DEFAULT_NUM_ADDR is ALL ADDRESSES
  N addresses processed if nFlag TRUE, otherwise ALL ADDRESSES*/

//MODE MACROS
#define SUMMARY 0
#define BITMASKS 1
#define VIRTUAL2PHYSICAL 2
#define V2P_TLB_PT 3
#define VPN2PFN 4
#define OFFSET 5

//MAX VPN BITS
#define MAXTOTALNUMBITS 28

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int bitshift);

unsigned int getOffset(unsigned int virtualAddress, unsigned int offsetMask);

unsigned int getPhysicalAddress(unsigned int pfn, unsigned int offset, unsigned int offsetSize);

unsigned int generateBitShift(unsigned int virtualAddressSize, unsigned int bitLength, int msbBitPos);

unsigned int generateBitMask(unsigned int bitLength, unsigned int start);
#endif