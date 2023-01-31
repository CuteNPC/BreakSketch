#ifndef _BREAK_SKETCH_OPTIMIZATION_2_H
#define _BREAK_SKETCH_OPTIMIZATION_2_H

#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "simd.h"
#include "Packet.h"
#include <bits/stdc++.h>
#include "Break_Sketch.h"

using namespace std;

class Break_Sketch_Optimization_2 : public Break_Sketch
{
public:
    SIMD_Bucket_2_32 *bucket2;
    SIMD_Bucket_4_32 *bucket4;
    SIMD_Bucket_8_32 *bucket8;
    int size2, size4, size8;
    BOBHash *hash2, *hash4, *hash8;
    BloomFilter *BF;

public:
    Break_Sketch_Optimization_2(
        int memory, int BFmemory,
        int hash_seed2 = 1000,
        int hash_seed4 = 1010,
        int hash_seed8 = 1020) // prime最多1229！
        : Break_Sketch(memory)
    {

        hash2 = new BOBHash(hash_seed2),
        hash4 = new BOBHash(hash_seed4),
        hash8 = new BOBHash(hash_seed8);

        size2 = (memory - BFmemory) / 24;
        size4 = (memory - BFmemory) / 48;
        size8 = (memory - BFmemory) / 96;

        bucket2 = new SIMD_Bucket_2_32[size2];
        bucket4 = new SIMD_Bucket_4_32[size4];
        bucket8 = new SIMD_Bucket_8_32[size8];

        BF = new BloomFilter(BFmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        int bucket_res = 0;
        int index;
#ifdef JUDGEMENT
        index = hash2->run((char *)&packet.id, sizeof(packet.id)) % size2;
        bucket_res += (bucket2[index].Insert(packet.seq) != 0);
        index = hash4->run((char *)&packet.id, sizeof(packet.id)) % size4;
        bucket_res += (bucket4[index].Insert(packet.seq) != 0);
        index = hash8->run((char *)&packet.id, sizeof(packet.id)) % size8;
        bucket_res += (bucket8[index].Insert(packet.seq) != 0);
#else
        index = hash2->run((char *)&packet.id, sizeof(packet.id)) % size2;
        bucket_res += (bucket2[index].Insert(packet.seq) == 1);
        index = hash4->run((char *)&packet.id, sizeof(packet.id)) % size4;
        bucket_res += (bucket4[index].Insert(packet.seq) == 1);
        index = hash8->run((char *)&packet.id, sizeof(packet.id)) % size8;
        bucket_res += (bucket8[index].Insert(packet.seq) == 1);
#endif

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
        return string("Break_Sketch_Optimization_2");
    }
    ~Break_Sketch_Optimization_2()
    {
        delete BF;
        delete[] bucket2;
        delete[] bucket4;
        delete[] bucket8;
        delete hash2;
        delete hash4;
        delete hash8;
    }
};

#endif