#include <stdio.h>  //FILE
#include <stdlib.h> //exit
#include <unistd.h> //getopt
#include <vector>   //vector
#include <locale>   //isdigit
#include <string.h> //strcmp

#include "PageTable.h"
#include "helpers.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

/*
CS 480 Operating Systems Assignment #3

NAME: ERICK HERNANDEZ
RED ID: 821321274

SYNOPSIS:
This assignment intends to simulate virtual address to physical address access of a program. 
A Page Table structure will serve to store virtual page number to physical frame mapping. When a virtual address is looked up, the corresponding vpn to pfn mapping will be returned. Other wise a frame number will be assigned to this virtual page number.

In addtion, a Translation Lookaside Buffer will store a specified number of VPN to PFN mappings that will circumvent the relatively slow lookup time of the PageTable structure.

To run this program provide a .tr file followed by a series of positive integers to represent the bits used per level.

REFERENCES
Generate Bit Mask
https://stackoverflow.com/questions/1392059/algorithm-to-generate-bit-mask
*/

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

//DRIVERS FOR MODES
void summaryDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag);
void bitmasksDriver(PageTable* ptr);
void virtual2physicalDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag);
void v2p_tlb_ptDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag);
void vpn2pfnDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag);
void offsetDriver(PageTable* ptr, FILE* file, int numAddr, bool nFlag);

//GET ARGUMENTS
void getArguments(int argc, char* argv[], int& numAddr, bool& nFlag, int& cacheCap, int& mode, std::vector<unsigned int>* levelBits, int& pathIdx);
//PROCESS ADDRESS
void memoryAccess(PageTable* ptrPT, unsigned int virtualAddress, unsigned int& frame, bool& cacheHit, bool& pageTableHit ,int cacheCap);

int main(int argc, char **argv)
{
  if(argc < 2){
    printf("USAGE: %s <file>.tr", argv[0]);
    exit(EXIT_FAILURE);
  }
  //argument variables
  int numAddr;
  int cacheCap;
  int mode;
  int pathIdx;
  bool nFlag;
  std::vector<unsigned int>* levelBits = new std::vector<unsigned int>;
  //get arguments
  getArguments(argc, argv, numAddr, nFlag, cacheCap, mode, levelBits, pathIdx);
  //initialize PageTable
  PageTable* bruh = new PageTable(*levelBits);
  //cleanup
  delete(levelBits);

  FILE *ifp;
  ifp = fopen(argv[pathIdx], "rb");
  //OUTPUT CONTROL
  if(mode == SUMMARY){
    summaryDriver(bruh, ifp, numAddr, cacheCap, nFlag);
  }
  else if(mode == BITMASKS){
    bitmasksDriver(bruh);
  }
  else if(mode == VIRTUAL2PHYSICAL){
    virtual2physicalDriver(bruh, ifp, numAddr, cacheCap, nFlag);
  }
  else if(mode == V2P_TLB_PT){
    v2p_tlb_ptDriver(bruh, ifp, numAddr, cacheCap, nFlag);
  }
  else if(mode == VPN2PFN){
    vpn2pfnDriver(bruh, ifp, numAddr, cacheCap, nFlag);
  }
  else{
    offsetDriver(bruh, ifp, numAddr, nFlag);
  }

  //CLEANUP
  fclose(ifp);
  bruh->bitmasks.clear();
  bruh->shiftInfo.clear();
  bruh->numEntriesPerLevel.clear();
  delete(bruh->root);
  delete(bruh);
  return (0);
}

//DRIVER FOR SUMMARY MODE
void summaryDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag){
  unsigned long i = 0;
  p2AddrTr trace;
  while(!feof(file)){
    //enforce address processing max
    if(nFlag){
      if(i >= numAddr){
        break;
      }
    }
    //get address from trace file
    if(NextAddress(file, &trace)){
      unsigned int frame;
      //flag if VPN in TLB
      bool cacheHit = false;
      //flag if VPN in PageTable
      bool pageTableHit = false;
      //access virtual address using TLB and PageTable
      memoryAccess(ptr, trace.addr, frame, cacheHit, pageTableHit, cacheCap);
      i++;
    }
  }
  //TODO REPLACE ZERO WITH TLB HIT
  report_summary(1 << ptr->offsetSize, 0, ptr->pageTableHit, i, ptr->frameCount, bytesUsed(ptr));
}

//DRIVER FOR BITMASKS MODE, prints out bit masks 
void bitmasksDriver(PageTable* ptr){
  report_bitmasks(ptr->levelCount, &(ptr->bitmasks[0]));
}

//DRIVER FOR VIRTUAL2PHYSICAL, prints out virtual address -> physical address
void virtual2physicalDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag){
  unsigned long  i = 0;
  p2AddrTr trace;
  while(!feof(file)){
    //enforce address processing max
    if(nFlag){
      if(i >= numAddr){
        return;
      }
    }
    //get address from trace file
    if(NextAddress(file, &trace)){
      unsigned int physicalAddress;
      unsigned int frame;
      //flag if VPN in TLB
      bool cacheHit = false;
      //flag if VPN in PageTable
      bool pageTableHit = false;

      //access virtual address using TLB and PageTable
      memoryAccess(ptr, trace.addr, frame, cacheHit, pageTableHit, cacheCap);
      
      //get physical address
      physicalAddress = getPhysicalAddress(getOffset(trace.addr, ptr->offsetMask), ptr->offsetSize, frame);

      report_virtual2physical(trace.addr, physicalAddress);
      i++;
    }
  }
}

//DRIVER FOR V2P_TLB_PT MODE, prints virtual address -> physical address 
//includes hit or miss information for TLB and PageTable 
void v2p_tlb_ptDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag){
  //TODO REMOVE WHEN FINISHED TLB
  return;
  unsigned long i = 0;
  p2AddrTr trace;
  while(!feof(file)){
    //enforce address processing max
    if(nFlag){
      if(i >= numAddr){
        return;
      }
    }
    //get address from trace file
    if(NextAddress(file, &trace)){
      unsigned int physicalAddress;
      unsigned int frame;
      //flag if VPN in TLB
      bool cacheHit = false;
      //flag if VPN in PageTable
      bool pageTableHit = false;

      //access virtual address using TLB and PageTable
      memoryAccess(ptr, trace.addr, frame, cacheHit, pageTableHit, cacheCap);
      i++;
    }
  }
}

//DRIVER FOR VPN2PFN MODE, prints vpn -> pfn
void vpn2pfnDriver(PageTable* ptr, FILE* file, int numAddr, int cacheCap, bool nFlag){
  unsigned long  i = 0;
  p2AddrTr trace;
  while(!feof(file)){
    //enforce address processing max
    if(nFlag){
      if(i >= numAddr){
        return;
      }
    }
    //get address from trace file
    if(NextAddress(file, &trace)){
      unsigned int frame;
      //flag if VPN in TLB
      bool cacheHit = false;
      //flag if VPN in PageTable
      bool pageTableHit = false;

      //access virtual address using TLB and PageTable
      memoryAccess(ptr, trace.addr, frame, cacheHit, pageTableHit, cacheCap);

      //vector to hold pages
      std::vector<unsigned int> pageHolder;
      for(int i = 0; i < ptr->levelCount; i++){
        pageHolder.push_back(virtualAddressToPageNum(trace.addr, ptr->bitmasks[i], ptr->shiftInfo[i]));
      }
      report_pagemap(ptr->levelCount, &pageHolder[0], frame);
      i++;
    }
  }
}

//DRIVER FOR OFFSET MODE, prints out offset for virtual address
void offsetDriver(PageTable* ptr, FILE* file, int numAddr, bool nFlag){
  unsigned long i = 0;
  p2AddrTr trace;
  while(!feof(file)){
    if(nFlag){
      if(i >= numAddr){
        return;
      }
    }
    if(NextAddress(file, &trace)){
      hexnum(getOffset(trace.addr, ptr->offsetMask));
    }
    i++;
  }
}

void getArguments(int argc, char* argv[], int& numAddr, bool& nFlag, int& cacheCap, int& mode, std::vector<unsigned int>* levelBits, int& pathIdx){
  /*flags to check presence of option and throw error for more than 1
    of same option*/
  bool oFlag = false;
  bool cFlag = false;
  nFlag = false;  //to determine ALL or n # of addresses

  /*Retrieve optional arguments:
    First if statement of each case checks for multiple of same option.
    -n and -c options expect integers.*/
  int Option;
  while((Option = getopt(argc, argv, "o:n:c:")) != -1){
    switch(Option){
      case 'o':
        if(oFlag){
          printf("ERROR: Multiple -o options\n");
          exit(EXIT_FAILURE);
        }
        //select mode
        else if(strcmp("summary", optarg) == 0){
          mode = SUMMARY;
        }
        else if(strcmp("bitmasks", optarg) == 0){
          mode = BITMASKS;
        }
        else if(strcmp("virtual2physical", optarg) == 0){
          mode = VIRTUAL2PHYSICAL;
        }
        else if(strcmp("v2p_tlb_pt", optarg) == 0){
          mode = V2P_TLB_PT;
        }
        else if(strcmp("vpn2pfn", optarg) == 0){
          mode = VPN2PFN;
        }
        else if(strcmp("offset", optarg) == 0){
          mode = OFFSET;
        }
        else{
          printf("ERROR: Unknown Mode Choice\n");
          exit(EXIT_FAILURE);
        }
        oFlag = true;
        break;
      case 'n':
        if(nFlag){
          printf("ERROR: Multiple -p options\n");
          exit(EXIT_FAILURE);
        }
        else if(!isdigit(*optarg)){
          printf("ERROR: -p expects integer\n");
          exit(EXIT_FAILURE);
        }
        else if(atoi(optarg) < 0){
          printf("ERROR: -o argument must be >= 0");
          exit(EXIT_FAILURE);
        }
        else{
          numAddr = atoi(optarg);
        }
        nFlag = true;
        break;
      case 'c':
        if(cFlag){
          printf("ERROR: Multiple -c options\n");
          exit(EXIT_FAILURE);
        }
        else if(atoi(optarg) < 0){
          printf("Cache capacity must be a number, greater than or equal to 0");
          exit(EXIT_FAILURE);
        }
        else{
          cacheCap = atoi(optarg);
        }
        cFlag = true;
        break;
      case '?':
        printf("ERROR: Unknown argument");
        exit(EXIT_FAILURE);
      default:
        printf("ERROR: Default");
        exit(EXIT_FAILURE);
    }
  }

  //set defaults if necessary
  if(!oFlag){
    mode = DEFAULT_MODE;
  }
  if(!cFlag){
    cacheCap = DEFAULT_CACHE_CAP;
  }

  //Retrieve mandatory arguments
  int idx = optind;
  //need at least two mandatory arguments, file + bits for level 0
  if((idx + 1 >= argc) || (idx >= argc)){
    printf("Usage: %s <file>.tr int...\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else{
    FILE *ifp;
    //check if valid file
    if((ifp = fopen(argv[idx], "rb")) == NULL){
      printf("Unable to open <<%s>>\n", argv[idx]);
      exit(EXIT_FAILURE);
    }
    fclose(ifp);
    //index for trace file in argv
    pathIdx = idx;

    //push back level bits into vector
    int levelBitsSum = 0;
    int currLevel = 0;
    for(int i = idx + 1; i < argc; i++){
      if(atoi(argv[i]) < 1){
        printf("Level %d page table must be at least 1 bit\n", currLevel);
        exit(EXIT_FAILURE);
      }
      levelBitsSum += atoi(argv[i]);
      levelBits->push_back(atoi(argv[i]));
      currLevel++;
    }
    //check if sum of bits does not exceed specified value
    if(levelBitsSum > MAXTOTALNUMBITS){
      printf("Too many bits used in page tables");
      exit(EXIT_FAILURE);
    }
  }

}

void memoryAccess(PageTable* ptrPT, unsigned int virtualAddress, unsigned int& frame, bool& cacheHit, bool& pageTableHit, int cacheCap){
  //check if TLB is used
  if(cacheCap){
    //TODO TLB LOOKUP
    //TODO SET CACHEHIT TO TRUE IF VPN IN TLB
  }
  //if VPN not in TLB or TLB not used
  if(!cacheHit){
    Map* map;
    //check if VPN in PageTable
    if(!(map = ptrPT->pageTableLookup(virtualAddress))){
      ptrPT->pageTableMiss++;
      //insert VPN into PageTable
      ptrPT->pageTableInsert(virtualAddress);
      //get PFN of newly inserted VPN
      frame = (ptrPT->pageTableLookup(virtualAddress))->frame;
    }
    else{
      //VPN found
      ptrPT->pageTableHit++;
      //get PFN of VPN
      frame = map->frame;
      //TODO INSERT VPN TO TLB
    }
  }
}