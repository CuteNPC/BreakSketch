#ifndef _BREAK_SKETCH_IMPROVED_H
#define _BREAK_SKETCH_IMPROVED_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"

using namespace std;
class Break_Sketch_improved : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket;
    int size;
    BOBHash *hash;
    BloomFilter *BF;

public:
    Break_Sketch_improved(int memory, int BFmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory - BFmemory) / 16;
        bucket = new SIMD_Bucket_4_32[size];
        hash = new BOBHash(hash_seed);
        BF = new BloomFilter(BFmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
        int bucket_res = bucket[index].Insert(packet.seq);

        if (!BF->Query((char *)&packet.id))
        {
            BF->Insert((char *)&packet.id);
            return false;
        }
#ifdef JUDGEMENT
        return bucket_res != 0;
#else
        return bucket_res == 1;
#endif
    }

    string Name()
    {
        return string("Break_Sketch_improved");
    }
    ~Break_Sketch_improved()
    {
        delete hash;
        delete BF;
        delete[] bucket;
    }
};

#endif