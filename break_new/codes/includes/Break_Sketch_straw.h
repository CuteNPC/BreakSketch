/*
单哈希，只有一张表table；
cell里存<flowID + seq>，对新插入的包，当ID相同时比较seq并更新；
当ID不同时直接踢走原记录；
*/
#ifndef _BREAK_SKETCH_STRAW_H
#define _BREAK_SKETCH_STRAW_H

#include "BOBHash.h"
#include "params.h"
#include "Packet.h"
#include <bits/stdc++.h>
#include "Break_Sketch.h"
using namespace std;

class Break_Sketch_straw : public Break_Sketch
{
public:
    struct cell
    {
        uint32_t flowID;
        uint32_t seq;
    };
    int w;
    cell *table;
    BOBHash *hash;

public:
    Break_Sketch_straw(int memory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        w = memory / 8;
        table = new cell[w];
        memset(table, 0, sizeof(cell) * w);
        hash = new BOBHash(hash_seed);
    }

    bool Solution(const Packet &packet)
    {
        bool res = false; 
        int index = (hash->run((const char *)&packet.id, KEY_LEN)) % w;
        if (table[index].flowID == packet.id)
        {
            uint32_t delta = packet.seq - table[index].seq;
            if (delta > 3)
                res = true; 
        }
        
        table[index].flowID = packet.id;
        table[index].seq = packet.seq;
        return res;
    }

    string Name()
    {
        return string("Break_Sketch_straw");
    }

    ~Break_Sketch_straw()
    {
        delete[] table;
        delete hash;
    }
};

#endif