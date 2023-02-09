/*
4哈希，4张哈希表；
cell里存<seq + LRUtag>，按LRU踢走旧记录
*/
#ifndef _BREAK_SKETCH_BASIC_1_H
#define _BREAK_SKETCH_BASIC_1_H

#include "BOBHash.h"
#include "params.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"
#include <string.h>
#include <iostream>
using namespace std;

class Break_Sketch_basic_1 : public Break_Sketch
{
public:
    struct cell
    {
        uint16_t seq;
        int t;
        cell() { seq = t = 0; }
    };
    int w;           // bucket的个数
    cell *tables[4]; // 4张table，每张w个cell
    BOBHash *hash[4];
    Tower_Sketch_CU *TS;

public:
    Break_Sketch_basic_1(int memory, int TSmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        TS = new Tower_Sketch_CU(TSmemory);
        w = (memory - TSmemory) / 24;

        for (int i = 0; i < 4; i++)
        {
            tables[i] = new cell[w];
        }

        for (int i = 0; i < 4; i++)
        {
            hash[i] = new BOBHash(i + hash_seed);
        }
    }

    void Insert(Packet packet)
    {
        int index[4];
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;

        int minplace = 0;
        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].seq == (uint16_t)0) /*找空的，找到了直接插入返回*/
            {
                tables[i][index[i]].seq = packet.seq;
                tables[i][index[i]].t = packet.time;
                return;
            }
            if (tables[i][index[i]].t < tables[minplace][index[minplace]].t) /*找最小的time*/
                minplace = i;
        }

        tables[minplace][index[minplace]].seq = packet.seq;
        tables[minplace][index[minplace]].t = packet.time;
        return;
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        TS->Insert((char *)&packet.id);

        if (TS->Query((char *)&packet.id) <= 14) // 如果TS判断是小流 或者 新流，不认为断流，不插入bucket
            return false;
        // 如果是旧流

        int index[4];
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((char *)&packet.id, sizeof(packet.id))) % w;

        int min_i;
        uint16_t min_delta = 0xffff;
        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].seq == (uint16_t)0 || tables[i][index[i]].seq >= packet.seq) // 空桶 或序列号大于等于seq 则跳过
                continue;
            uint16_t delta = packet.seq - tables[i][index[i]].seq;
            if (delta < min_delta)
            {
                min_delta = delta;
                min_i = i;
            }
        }

        if (min_delta < (uint16_t)50) // 找到seq，更新对应seq，以及非空的tag
        {
            tables[min_i][index[min_i]].seq = packet.seq;
            tables[min_i][index[min_i]].t = packet.time;
            return min_delta > 3;
        }
        else // 发生断流，且丢失了原流
        {
            this->Insert(packet);
#ifdef JUDGEMENT
            return true;
#else
            return false;
#endif
        }
    }
    string Name()
    {
        return string("Break_Sketch_basic_1");
    }

    ~Break_Sketch_basic_1()
    {
        for (int i = 0; i < 4; i++)
        {
            delete[] tables[i];
            delete hash[i];
        }
        delete TS;
    }
};

#endif