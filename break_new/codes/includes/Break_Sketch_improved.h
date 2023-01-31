#ifndef _BREAK_SKETCH_IMPROVED_H
#define _BREAK_SKETCH_IMPROVED_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

using namespace std;
class Break_Sketch_improved : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket;
    int size;
    BOBHash *hash;
    Tower_Sketch *TS;

public:
    Break_Sketch_improved(int memory, int TSmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory - TSmemory) / 16;
        bucket = new SIMD_Bucket_4_32[size];
        hash = new BOBHash(hash_seed);
        TS = new Tower_Sketch(TSmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        TS->Insert((char *)&packet.id);
        unsigned int num = TS->Query((char *)&packet.id);
        
        if(num <= 14)
            return false;
        else
        {
            int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
            int bucket_res = bucket[index].Insert(packet.seq);
#ifdef JUDGEMENT
            return bucket_res != 0;
#else
            return bucket_res == 1;
#endif
        }
    }

    string Name()
    {
        return string("Break_Sketch_improved");
    }
    ~Break_Sketch_improved()
    {
        delete hash;
        delete TS;
        delete[] bucket;
    }
};

#endif