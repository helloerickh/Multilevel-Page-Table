#ifndef MAP_H
#define MAP_H

/*MAP CLASS
- represents a mapping between a vpn and a pfn
- isValid true if frame assigned, else false
- frame is the assigned physical frame number*/
class Map{
    public:
    bool isValid;
    unsigned int frame;
    Map();
};

#endif