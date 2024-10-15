Step by Step:

1. Get arguments and verify trace.tr
    - trace.tr: filled with memory references of arbitrary program

    - page table size: number of bits assigned to page tables for each level
        - sum of bits must not exceed 28

    - o: output mode
        - summary
            - page sizes + pagetable hits and misses
        - bitmasks
            - bitmasks used to isolate page numbers
        - virtual2physical
            - virtual address -> physical address pairs
        - v2p_tlb_pt
            - virtual address -> physical frame number pairs + TLB hits and misses
        - vpn2pfn
            - virtual address -> physical frame number pairs
        - offset
            - offset portion of virtual address
    - n: number of addresses to read from trace file
        - default: inf
    - c: cache buffer size
        - default: no cache

    Example: pagingwithtlb ./trace.tr 1 2 3 4 -o summary -n 100 -c 24

2. Generate BitShifts and Bitmasks
    - Bitshifts are used to shift masked page addresses to the LSB so that may be used for indexing to traverse page
    table
    - Bitmasks used to isolate sections of virtual address such as page addresses and offset

3. Initialize PageTable and Levels
    - upon reading a virtual address from the trace file we traverse the PageTable tree structure using page numbers as indices
    - if a page number has not been seen yet we create new levels
    - Leaf Node levels point to Map structures that hold Physical Frame Numbers

4. Finding the Physical Address given a Physical Frame Number
    - Physical Address is represented by (MSB -> LSB) Physical Frame Number followed by the offset of the Virtual Address
    - Example: PFN = 0x918d Offset = 0x204 Physical Address = 0x918d204

5. TLB enabled memory lookup with Least Recently Used policy
    - if not in TLB  add to LRU and execute pagetable lookup and then insert to TLB followed by replacement policy if needed
    - if in TLB retrieve PFN in constant time and update LRU accordingly
    - when a virtual address to PFN mapping is added to TLB a corresponding "time" value is added to the LRU
    - when space is needed in cache the virtual address with the smallest or least used "time" value is removed from the lru and tlb