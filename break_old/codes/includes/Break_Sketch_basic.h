#ifndef _BREAK_SKETCH_BASIC_H
#define _BREAK_SKETCH_BASIC_H

#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "Packet.h"
#include <string.h>
#include <iostream>
#include "Break_Sketch.h"
using namespace std;

class Break_Sketch_basic : public Break_Sketch
{
public:
    struct cell
    {
        uint32_t seq;
        int LRU_tag;
        cell() { seq = LRU_tag = 0; }
    };
    int w;           // bucket的个数
    cell *tables[4]; // 4张table，每张w个cell
    BOBHash *hash[4];
    int index[4];
    BloomFilter *BF;

public:
    Break_Sketch_basic(int memory, int BFmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        BF = new BloomFilter(BFmemory);
        w = (memory - BFmemory) / 32;

        for (int i = 0; i < 4; i++)
        {
            tables[i] = new cell[w];
        }

        for (int i = 0; i < 4; i++) // init d hash functions 创建d个不同的hash函数
        {
            hash[i] = new BOBHash(i + hash_seed);
        }
    }

    void Insert(Packet packet)
    {
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;

        int empty = 0;
        int min_i, min_LRUtag = 6; // 不会比5更大
        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].seq == 0)
            { // 位置是空的
                if (!empty)
                {
                    tables[i][index[i]].seq = packet.seq;
                    tables[i][index[i]].LRU_tag = 5;
                    empty = 1;
                }
            }
            else
            { // 非空
                tables[i][index[i]].LRU_tag--;
                if (tables[i][index[i]].LRU_tag < min_LRUtag)
                {
                    min_LRUtag = tables[i][index[i]].LRU_tag;
                    min_i = i;
                }
            }
        }
        if (!empty)
        {
            tables[min_i][index[min_i]].seq = packet.seq;
            tables[min_i][index[min_i]].LRU_tag = 5;
        }
    }

    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        if (!BF->Query((char *)&packet.id)) // 如果BF判断是新流
        {
            BF->Insert((char *)&packet.id);
            this->Insert(packet);
            return false;
        }
        else // 如果是旧流
        {
            for (int i = 0; i < 4; i++)
                index[i] = (hash[i]->run((char *)&packet.id, sizeof(packet.id))) % w;

            int min_i;
            uint32_t min_delta = 0xffffffff;
            for (int i = 0; i < 4; i++)
            {
                tables[i][index[i]].LRU_tag--;                                             // 无论是否为空，都把tag--（空的--无所谓）
                if (tables[i][index[i]].seq == 0 || tables[i][index[i]].seq >= packet.seq) // 空桶 或序列号大于等于seq 则跳过
                    continue;
                uint32_t delta = packet.seq - tables[i][index[i]].seq;
                if (delta < min_delta)
                {
                    min_delta = delta;
                    min_i = i;
                }
            }

            if (min_delta < 50) // 找到seq，更新对应seq，以及非空的tag
            {
                tables[min_i][index[min_i]].seq = packet.seq;
                tables[min_i][index[min_i]].LRU_tag = 5;
                if (min_delta <= 3)
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
        return string("Break_Sketch_basic");
    }

    ~Break_Sketch_basic()
    {
        for (int i = 0; i < 4; i++)
        {
            delete[] tables[i];
            delete hash[i];
        }
        delete BF;
    }
};

#endif