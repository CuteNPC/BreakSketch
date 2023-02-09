#ifndef _BREAK_SKETCH_NEW_H
#define _BREAK_SKETCH_NEW_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

using namespace std;
class Break_Sketch_new : public Break_Sketch
{
public:
    SIMD_Bucket_4_16 *bucket_16;
    uint8_t *seq2_bucket;
    int size;
    BOBHash *hash;

public:
    Break_Sketch_new(int memory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory+512) / 16;
        bucket_16 = new SIMD_Bucket_4_16[size];
        seq2_bucket = new uint8_t[size * 8];
        hash = new BOBHash(hash_seed);
        memset(bucket_16, 0, memory / 2);
        memset(seq2_bucket, 0, memory / 2);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        uint16_t kickednum = 0;
        int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
        int bucket_16_res = bucket_16[index].MatchAndInsert(packet.seq, kickednum);

        if (bucket_16_res < 2)
        { // 在第一层match，0为正常，1为断流
            return bucket_16_res == 1;
        }
        else
        {
            return 0;
            uint32_t index2 = (index / 32) * 256 + (uint8_t)(packet.seq >> 8);
            if(index2 >= size * 8)
            {
                printf("%d,%d,%d,%d", index, packet.seq, index2, size);
                exit(1);
            }
            uint8_t seq2 = (uint8_t)(packet.seq);
            uint8_t diff = seq2 - seq2_bucket[index2];
            if (kickednum != (uint16_t)0)
                seq2_bucket[index2] = kickednum;
            return diff >= 4 && diff < 50;
        }
    }

    string Name()
    {
        return string("Break_Sketch_new");
    }

    ~Break_Sketch_new()
    {
        delete hash;
        delete[] bucket_16;
        delete[] seq2_bucket;
    }
};

#endif