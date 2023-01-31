#ifndef _BREAK_SKETCH_H
#define _BREAK_SKETCH_H

#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "Packet.h"
#include <bits/stdc++.h>
using namespace std;

class Break_Sketch
{
public:
    int mem;
    Break_Sketch(int memory) : mem(memory) {}
    virtual bool Solution(const Packet &packet) { return 1; };
    virtual string Name() { return (string) ""; }
    int getmemory(){return mem;};
    ~Break_Sketch() {}
};
#endif