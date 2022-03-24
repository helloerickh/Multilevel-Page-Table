#include "helpers.h"

/*
NAME: ERICK HERNANDEZ
RED ID: 821321274

NAME: KAVON CACHO
RED ID: 822794135
*/

/*Calculate Page Number
INPUT: unsigned int virtual address, bit mask, number of bits to shift
RETURN: unsigned int isolated page number
- applies bit wise & to virtual address to isolate page number
- shift this page number to be flush with LSB side
- to get full VPN mask should cover all vpn bits and shift = offsetSize*/
unsigned int virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int bitshift){
    unsigned int page = virtualAddress;
    page &= mask;
    page >>= bitshift;
    return page;
}

/*Get offset of a virtual address
INPUT: unsigned int virtual address
RETURN: offset bits*/
unsigned int getOffset(unsigned int virtualAddress, unsigned int offsetMask){
    unsigned int offset = virtualAddress;
    offset &= offsetMask;
    return offset;
}

/*Get physical address
INPUT: unsigned int offset bits, size of offset, physical frame number
RETURN: unsigned int physical address*/
unsigned int getPhysicalAddress(unsigned int offset, unsigned int offsetSize, unsigned int pfn){
    unsigned int physicalAddress = offset;
    physicalAddress += (pfn << offsetSize);
    return physicalAddress;
}

/*Generate number of bits to shift a set of bits to be flush with LSB
INPUT: unsigned int address size and page size, int bit position from MSB
RETURN: unsigned int bit shift
- Bit shift represents number of bits to shift right to be flush with LSB
- Bit shifted page number is used to traverse page table*/
unsigned int generateBitShift(unsigned int virtualAddressSize, unsigned int bitLength, int msbBitPos){
    //calculate how many bits to shift over depending on how many bits there are and where they begin
    unsigned int bitShift = virtualAddressSize - bitLength - msbBitPos;
    return bitShift;
}

/*Generate a bitmask given the length of the mask and where the MSB bit of the mask should be
INPUT: unsigned int length of bitmask, unsigned int start of MSB side of bit mask
RETURN: unsigned int bit mask
- Bit mask is used to isolate page numbers for level
- start = length, results in zero shift, good for offset mask*/
unsigned int generateBitMask(unsigned int length, unsigned int start){
    //intitialize length number of 1s from MSB side
    unsigned int bitMask = (1 << length) - 1;
    //shift 1s to start bit
    bitMask <<= (start - length);
    return bitMask;
}