#ifndef HELPERS_H
#define HELPERS_H

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274
*/

#define ADDRESS_SIZE 32

unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int bitshift);

unsigned int getOffset(unsigned int virtualAddress, unsigned int offsetMask);

unsigned int getPhysicalAddress(unsigned int pfn, unsigned int offset, unsigned int offsetSize);

unsigned int generateBitShift(unsigned int virtualAddressSize, unsigned int bitLength, int msbBitPos);

unsigned int generateBitMask(unsigned int bitLength, unsigned int start);
#endif