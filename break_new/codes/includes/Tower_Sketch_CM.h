#ifndef _TOWER_SKETCH_CM_H
#define _TOWER_SKETCH_CM_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
using namespace std;

static unsigned char mask_2[4] = {0b00000011, 0b00001100, 0b00110000, 0b11000000};
static unsigned char delt_2[4] = {0b00000001, 0b00000100, 0b00010000, 0b01000000};
static unsigned char mask_4[2] = {0b00001111, 0b11110000};
static unsigned char delt_4[2] = {0b00000001, 0b00010000};

class Tower_Sketch_CM
{
public:
	uint32_t w_2, w_4;
	unsigned char* table_2;
	unsigned char* table_4;
	BOBHash *hash_2, *hash_4;

public:
	Tower_Sketch_CM(int TSmemory, int hash_seed = 1000)
	{
		w_2 =TSmemory * 2;
		w_4 =TSmemory;
		table_2 = new unsigned char[TSmemory / 2];
		table_4 = new unsigned char[TSmemory / 2];
		memset(table_2, 0, TSmemory / 2);
		memset(table_4, 0, TSmemory / 2);

        hash_2 = new BOBHash(hash_seed + 2);
        hash_4 = new BOBHash(hash_seed + 4);

	}
    
    void Insert(const char *str)
    {
		uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m = index_2 / 4, n = index_2 % 4;
        if((table_2[m] & mask_2[n]) ^ mask_2[n]) table_2[m] += delt_2[n];

		uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        m = index_4 / 2, n = index_4 % 2;
        if((table_4[m] & mask_4[n]) ^ mask_4[n]) table_4[m] += delt_4[n];
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
        
        if(res_2 == 3) return res_4; 
        else return res_2 < res_4 ? res_2 : res_4;
	}

    ~Tower_Sketch_CM()
    {
        delete table_2;
        delete table_4;
        delete hash_2;
        delete hash_4;
    }
};
#endif