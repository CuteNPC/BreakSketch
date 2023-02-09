/*
4哈希，4张哈希表；
cell里存<seq>，随机踢走旧记录
*/
#ifndef _BREAK_SKETCH_BASIC_2_H
#define _BREAK_SKETCH_BASIC_2_H

#include "BOBHash.h"
#include "params.h"
#include "Packet.h"
#include "Break_Sketch.h"
#include "Tower_Sketch_CU.h"
#include <string.h>
#include <iostream>
using namespace std;

class Break_Sketch_basic_2 : public Break_Sketch
{
public:
    struct cell
    {
        uint16_t seq;
        cell() { seq = (uint16_t)0; }
    };
    int w;           // bucket的个数
    cell *tables[4]; // 4张table，每张w个cell
    BOBHash *hash[4];
    int index[4];
    Tower_Sketch_CU *TS;

public:
    Break_Sketch_basic_2(int memory, int TSmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        TS = new Tower_Sketch_CU(TSmemory);
        w = (memory - TSmemory) / 8;

        for (int i = 0; i < 4; i++)
        {
            tables[i] = new cell[w];
        }
        for (int i = 0; i < 4; i++)
        {
            hash[i] = new BOBHash(i + hash_seed);
        }

        srand(1898);
    }

    void Insert(Packet packet)
    {
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;

        bool empty = false;
        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].seq == (uint16_t)0)// 位置是空的
            { 
                if (!empty)
                {
                    tables[i][index[i]].seq = packet.seq;
                    empty = true;
                }
            }
        }

        if (!empty) //随机踢出
        {
            int rand_i = rand() % 4;
            tables[rand_i][index[rand_i]].seq = packet.seq;
        }
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        unsigned int num = TS->InsertAndQuery((char *)&packet.id);
        
        if(num <= 14) // 如果TS判断是小流 或者 新流，不认为断流，不插入bucket
            return false;
        else // 如果是旧流
        {
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
                if (min_delta <= (uint16_t)3)
                    return false;
                else
                    return true;
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
    }
    
    string Name()
    {
        return string("Break_Sketch_basic_2");
    }

    ~Break_Sketch_basic_2()
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