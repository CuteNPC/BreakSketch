#ifndef _BREAK_SKETCH_OPTIMIZATION_2_H
#define _BREAK_SKETCH_OPTIMIZATION_2_H

#include "BOBHash.h"
#include "params.h"
#include "simd.h"
#include "Packet.h"
#include <bits/stdc++.h>
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"

using namespace std;

class Break_Sketch_Optimization_2 : public Break_Sketch
{
public:
    SIMD_Bucket_2_16 *bucket2;
    SIMD_Bucket_4_16 *bucket4;
    SIMD_Bucket_8_16 *bucket8;
    int size2, size4, size8;
    BOBHash *hash2, *hash4, *hash8;
    Tower_Sketch_CU *TS;

public:
    Break_Sketch_Optimization_2(
        int memory, int TSmemory,
        int hash_seed2 = 1000,
        int hash_seed4 = 1010,
        int hash_seed8 = 1020) // prime最多1229！
        : Break_Sketch(memory)
    {

        hash2 = new BOBHash(hash_seed2),
        hash4 = new BOBHash(hash_seed4),
        hash8 = new BOBHash(hash_seed8);

        size2 = (memory - TSmemory) / 12;
        size4 = (memory - TSmemory) / 24;
        size8 = (memory - TSmemory) / 48;

        bucket2 = new SIMD_Bucket_2_16[size2];
        bucket4 = new SIMD_Bucket_4_16[size4];
        bucket8 = new SIMD_Bucket_8_16[size8];

        TS = new Tower_Sketch_CU(TSmemory);
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
        return string("Break_Sketch_Optimization_2");
    }
    ~Break_Sketch_Optimization_2()
    {
        delete TS;
        delete[] bucket2;
        delete[] bucket4;
        delete[] bucket8;
        delete hash2;
        delete hash4;
        delete hash8;
    }
};

#endif