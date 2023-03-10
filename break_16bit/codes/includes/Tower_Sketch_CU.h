#ifndef _TOWER_SKETCH_CU_H
#define _TOWER_SKETCH_CU_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"

#ifdef DEBUGx
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

using namespace std;

class Tower_Sketch_CU
{
public:
    uint32_t w_2, w_4;
    unsigned char *table_2;
    unsigned char *table_4;
    BOBHash *hash_2, *hash_4;
    unsigned char mask_2[4] = {0b00000011, 0b00001100, 0b00110000, 0b11000000};
    unsigned char delt_2[4] = {0b00000001, 0b00000100, 0b00010000, 0b01000000};
    unsigned char mask_4[2] = {0b00001111, 0b11110000};
    unsigned char delt_4[2] = {0b00000001, 0b00010000};

public:
    Tower_Sketch_CU(int TSmemory, int hash_seed = 1000)
    {
        w_2 = TSmemory * 2;
        w_4 = TSmemory;
        table_2 = new unsigned char[TSmemory / 2];
        table_4 = new unsigned char[TSmemory / 2];
        memset(table_2, 0, TSmemory / 2);
        memset(table_4, 0, TSmemory / 2);

        hash_2 = new BOBHash(hash_seed + 2);
        hash_4 = new BOBHash(hash_seed + 4);
    }

    void Insert(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;

        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m_2 = index_2 / 4, n_2 = index_2 % 4;
        res_2 = (table_2[m_2] & mask_2[n_2]) / (1 << (n_2 * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        dbg_printf("Before Insert: ");
        dbg_printf("index_2 = %d, index_4 = %d, ", index_2, index_4);
        dbg_printf("res_2 = %d, res_4 = %d\n", res_2, res_4);

        if (res_2 == 3 && res_4 == 15)
            ;
        else if ((res_2 == 3 && res_4 != 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 > res_4))
            table_4[m_4] += delt_4[n_4];
        else if ((res_2 != 3 && res_4 == 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 < res_4))
            table_2[m_2] += delt_2[n_2];
        else
        {
            table_4[m_4] += delt_4[n_4];
            table_2[m_2] += delt_2[n_2];
        }

        res_2 = (table_2[m_2] & mask_2[n_2]) / (1 << (n_2 * 2));
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));
        dbg_printf("After Insert:  ");
        dbg_printf("index_2 = %d, index_4 = %d, ", index_2, index_4);
        dbg_printf("res_2 = %d, res_4 = %d\n", res_2, res_4);
        return;
    }

    unsigned int Query(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;
        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m = index_2 / 4, n = index_2 % 4;
        res_2 = (table_2[m] & mask_2[n]) / (1 << (n * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        m = index_4 / 2, n = index_4 % 2;
        res_4 = (table_4[m] & mask_4[n]) / (1 << (n * 4));

        if (res_2 == 3)
            return res_4;
        else
            return res_2 < res_4 ? res_2 : res_4;
    }

    unsigned int InsertAndQuery(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;

        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m_2 = index_2 / 4, n_2 = index_2 % 4;
        res_2 = (table_2[m_2] & mask_2[n_2]) / (1 << (n_2 * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        if (res_2 == 3 && res_4 == 15) // ?????????
        {
            return 15;
        }
        else if ((res_2 == 3 && res_4 != 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 > res_4))
        {
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
        else if ((res_2 != 3 && res_4 == 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 < res_4))
        {
            table_2[m_2] += delt_2[n_2];
            /*if(res_2 == 2) return res_4;
            else return res_2 + 1;*/
            return res_2 + 1;
            /*????????????????????????????????????2bit?????????2?????????+1??????3(????????????)???
            ????????????towersketch?????????????????????????????????????????????????????????????????????????????????????????????
            ??????????????????????????????????????????????????????InsertAndQuery ??? Insert + Query???????????????????????????
            ??????caida??????????????????3000???????????????????????????2bit????????????3??????4bit???????????????3???
            ???????????????????????????3?????????????????????????????????
            */
        }
        else
        {
            table_2[m_2] += delt_2[n_2];
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
    }

    ~Tower_Sketch_CU()
    {
        delete table_2;
        delete table_4;
        delete hash_2;
        delete hash_4;
    }
};

#undef dbg_printf

#endif