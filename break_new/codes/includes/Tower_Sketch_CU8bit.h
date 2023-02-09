#ifndef _TOWER_SKETCH_CU8_H
#define _TOWER_SKETCH_CU8_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"

#ifdef DEBUGx
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

using namespace std;

class Tower_Sketch_CU8
{
public:
    uint32_t w_4, w_8;
    unsigned char *table_4;
    unsigned char *table_8;
    BOBHash *hash_4, *hash_8;
    unsigned char mask_4[2] = {0b00001111, 0b11110000};
    unsigned char delt_4[2] = {0b00000001, 0b00010000};    

public:
    Tower_Sketch_CU8(int TSmemory, int hash_seed = 1000)
    {
        w_4 = TSmemory;
        w_8 = TSmemory / 2;
        table_4 = new unsigned char[TSmemory / 2];
        table_8 = new unsigned char[TSmemory / 2];
        memset(table_4, 0, TSmemory / 2);
        memset(table_8, 0, TSmemory / 2);

        hash_4 = new BOBHash(hash_seed + 2);
        hash_8 = new BOBHash(hash_seed + 4);
    }

    void Insert(const char *str)
    {
        uint32_t res_4 = 0, res_8 = 0;

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        uint32_t index_8 = (hash_8->run(str, KEY_LEN)) % w_8;
        uint32_t m_8 = index_8;
        res_8 = (unsigned int)table_8[m_8];

        dbg_printf("Before Insert: ");
        dbg_printf("index_4 = %d, index_8 = %d, ", index_4, index_8);
        dbg_printf("res_4 = %d, res_8 = %d\n", res_4, res_8);

        if (res_4 == 15 && res_8 == 255)
            ;
        else if ((res_4 == 15 && res_8 != 255) ||
                 (res_4 != 15 && res_8 != 255 && res_4 > res_8))
            table_8[m_8] += 0x1u;
        else if ((res_4 != 15 && res_8 == 255) ||
                 (res_4 != 15 && res_4 != 255 && res_4 < res_8))
            table_4[m_4] += delt_4[n_4];
        else
        {
            table_8[m_8] += 0x1u;
            table_4[m_4] += delt_4[n_4];
        }

        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));
        res_8 = (unsigned int)table_8[m_8];
        dbg_printf("After Insert:  ");
        dbg_printf("index_4 = %d, index_8 = %d, ", index_4, index_8);
        dbg_printf("res_4 = %d, res_8 = %d\n", res_4, res_8);
        return;
    }

    unsigned int Query(const char *str)
    {
        uint32_t res_4 = 0, res_8 = 0;

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        uint32_t index_8 = (hash_8->run(str, KEY_LEN)) % w_8;
        uint32_t m_8 = index_8;
        res_8 = (unsigned int)table_8[m_8];

        if (res_4 == 15)
            return res_8;
        else
            return res_4 < res_8 ? res_4 : res_8;
    }

    unsigned int InsertAndQuery(const char *str)
    {
        uint32_t res_4 = 0, res_8 = 0;

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        uint32_t index_8 = (hash_8->run(str, KEY_LEN)) % w_8;
        uint32_t m_8 = index_8;
        res_8 = (unsigned int)table_8[m_8];

        if (res_4 == 15 && res_8 == 255) // 都溢出
        {
            return 255;
        }
        else if ((res_4 == 15 && res_8 != 255) ||
                 (res_4 != 15 && res_8 != 255 && res_4 > res_8))
        {
            table_8[m_8] += 0x1u;
            return res_8 + 1;
        }
        else if ((res_4 != 15 && res_8 == 255) ||
                 (res_4 != 15 && res_4 != 255 && res_4 < res_8))
        {
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
        else
        {
            table_8[m_8] += 0x1u;
            table_4[m_4] += delt_4[n_4];
            return res_8 + 1;
        }
    }

    ~Tower_Sketch_CU8()
    {
        delete table_4;
        delete table_8;
        delete hash_4;
        delete hash_8;
    }
};

#undef dbg_printf

#endif