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
    SIMD_Bucket_8_8 *bucket_8;
    int size;
    BOBHash *hash;

public:
    Break_Sketch_new(int memory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = memory / 16;
        bucket_16 = new SIMD_Bucket_4_16[size];
        bucket_8 = new SIMD_Bucket_8_8[size];
        hash = new BOBHash(hash_seed);
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
            // 在第一层unmatch
            // 先到第二层寻找
            int bucket_8_res = bucket_8[index].MatchAndEmpty((uint8_t)packet.seq);
            // 把第一层踢出的插入第二层
            if (kickednum != (uint16_t)0)
                bucket_8[index].Insert((uint8_t)kickednum);
            return bucket_8_res == 1;
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
        delete[] bucket_8;
    }
};

#endif