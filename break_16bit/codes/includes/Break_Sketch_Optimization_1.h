#ifndef _BREAK_SKETCH_OPTIMIZATION_1_H
#define _BREAK_SKETCH_OPTIMIZATION_1_H

#include "BOBHash.h"
#include "params.h"
#include <bits/stdc++.h>
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

using namespace std;

class Break_Sketch_Optimization_1 : public Break_Sketch
{
public:
    SIMD_Bucket_4_16 *bucket[3];
    int size;
    BOBHash *hash[3];
    Tower_Sketch_CU *TS;

public:
    Break_Sketch_Optimization_1(int memory, int TSmemory, int hash_seed1 = 1000, int hash_seed2 = 1010, int hash_seed3 = 1020)
        : Break_Sketch(memory)
    {
        size = (memory - TSmemory) / 24;

        for (int i = 0; i < 3; i++)
        {
            bucket[i] = new SIMD_Bucket_4_16[size];
        }

        hash[0] = new BOBHash(hash_seed1);
        hash[1] = new BOBHash(hash_seed2);
        hash[2] = new BOBHash(hash_seed3);

        TS = new Tower_Sketch_CU(TSmemory);
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        unsigned int num = TS->InsertAndQuery((char *)&packet.id);
        
        if(num <= 14)
            return false;
        else
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
        
#ifdef JUDGEMENT
            return bucket_res == 3;
#else
            //return bucket_res > 1;
            return bucket_res > 0;
#endif
        }
    }

    string Name()
    {
        return string("Break_Sketch_Optimization_1");
    }
    ~Break_Sketch_Optimization_1()
    {
        delete TS;
        for (int i = 0; i <= 2; i++)
        {
            delete hash[i];
            delete[] bucket[i];
        }
    }
};

#endif