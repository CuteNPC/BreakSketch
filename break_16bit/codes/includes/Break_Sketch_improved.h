#ifndef _BREAK_SKETCH_IMPROVED_H
#define _BREAK_SKETCH_IMPROVED_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

using namespace std;
class Break_Sketch_improved : public Break_Sketch
{
public:
    SIMD_Bucket_4_16 *bucket;
    int size;
    BOBHash *hash;
    Tower_Sketch_CU *TS;

public:
    Break_Sketch_improved(int memory, int TSmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory - TSmemory) / 8;
        bucket = new SIMD_Bucket_4_16[size];
        hash = new BOBHash(hash_seed);
        TS = new Tower_Sketch_CU(TSmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        unsigned int num = TS->InsertAndQuery((char *)&packet.id);

        //dbg_printf("TS查询结果: %d\n", num);
        if (num <= 14)
            return false;
        else
        {
#ifdef DEBUG
            s.insert(packet.id);
#endif
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