#ifndef _BREAK_SKETCH_OPTIMIZATION_1_H
#define _BREAK_SKETCH_OPTIMIZATION_1_H

#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include <bits/stdc++.h>
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"

using namespace std;

class Break_Sketch_Optimization_1 : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket[3];
    int size;
    BOBHash *hash[3];
    BloomFilter *BF;

public:
    Break_Sketch_Optimization_1(int memory, int BFmemory, int hash_seed1 = 1000, int hash_seed2 = 1010, int hash_seed3 = 1020)
        : Break_Sketch(memory)
    {
        size = (memory - BFmemory) / 48;

        for (int i = 0; i < 3; i++)
        {
            bucket[i] = new SIMD_Bucket_4_32[size];
        }

        hash[0] = new BOBHash(hash_seed1);
        hash[1] = new BOBHash(hash_seed2);
        hash[2] = new BOBHash(hash_seed3);

        BF = new BloomFilter(BFmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        int bucket_res = 0;
        for (int i = 0; i <= 2; i++)
        {
            int index = hash[i]->run((char *)&packet.id, sizeof(packet.id)) % size;

#ifdef JUDGEMENT
            bucket_res += (bucket[i][index].Insert(packet.seq) != 0);
#else
            bucket_res += (bucket[i][index].Insert(packet.seq) == 1);
#endif
        }

        if (!BF->Query((char *)&packet.id))
        {
            BF->Insert((char *)&packet.id);
            return false;
        }

#ifdef JUDGEMENT
        return bucket_res == 3;
#else
        return bucket_res > 1;
#endif
    }

    string Name()
    {
        return string("Break_Sketch_Optimization_1");
    }
    ~Break_Sketch_Optimization_1()
    {
        delete BF;
        for (int i = 0; i <= 2; i++)
        {
            delete hash[i];
            delete bucket[i];
        }
    }
};

#endif