#ifndef _BREAK_SKETCH_IMPROVED_BIGTS15_H
#define _BREAK_SKETCH_IMPROVED_BIGTS15_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU8bit.h"

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
set<uint32_t> s;
#else
#define dbg_printf(...)
#endif

using namespace std;
class Break_Sketch_improved_bigTS15 : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket;
    int size;
    BOBHash *hash;
    Tower_Sketch_CU8 *TS;

public:
    Break_Sketch_improved_bigTS15(int memory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = memory / 16;
        bucket = new SIMD_Bucket_4_32[size];
        hash = new BOBHash(hash_seed);
        TS = new Tower_Sketch_CU8(64 * 1024 * 1024);
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
        return string("Break_Sketch_improved_bigTS15");
    }
    ~Break_Sketch_improved_bigTS15()
    {
        delete hash;
        delete TS;
        delete[] bucket;
    }
};

#endif