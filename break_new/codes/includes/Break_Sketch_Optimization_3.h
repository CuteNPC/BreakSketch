#ifndef _BREAK_SKETCH_OPTIMIZATION_3_H
#define _BREAK_SKETCH_OPTIMIZATION_3_H

#include "BOBHash.h"
#include "params.h"
#include <bits/stdc++.h>
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

using namespace std;
/* 就是把 Op1 的三张表改成两张了 */
class Break_Sketch_Optimization_3 : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket[2];
    int size;
    BOBHash *hash[2];
    Tower_Sketch *TS;

public:
    Break_Sketch_Optimization_3(int memory, int TSmemory, int hash_seed1 = 1000, int hash_seed2 = 1010, int hash_seed3 = 1020)
        : Break_Sketch(memory)
    {
        size = (memory - TSmemory) / 32;

        for (int i = 0; i < 2; i++)
        {
            bucket[i] = new SIMD_Bucket_4_32[size];
        }

        hash[0] = new BOBHash(hash_seed1);
        hash[1] = new BOBHash(hash_seed2);

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
            int bucket_res = 0;
            for (int i = 0; i < 2; i++)
            {
                int index = hash[i]->run((char *)&packet.id, sizeof(packet.id)) % size;
#ifdef JUDGEMENT
                bucket_res += (bucket[i][index].Insert(packet.seq) != 0);
#else
                bucket_res += (bucket[i][index].Insert(packet.seq) == 1);
#endif
            }
        
#ifdef JUDGEMENT
            return bucket_res == 2;
#else
            return bucket_res > 0;
#endif
        }
    }

    string Name()
    {
        return string("Break_Sketch_Optimization_3");
    }

    ~Break_Sketch_Optimization_3()
    {
        delete TS;
        for (int i = 0; i < 2; i++)
        {
            delete hash[i];
            delete[] bucket[i];
        }
    }
};

#endif