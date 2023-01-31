#ifndef _TOWER_SKETCH_2_H
#define _TOWER_SKETCH_2_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
using namespace std;

static unsigned char mask_2[4] = {0b00000011, 0b00001100, 0b00110000, 0b11000000};
static unsigned char delt_2[4] = {0b00000001, 0b00000100, 0b00010000, 0b01000000};
static unsigned char mask_4[2] = {0b00001111, 0b11110000};
static unsigned char delt_4[2] = {0b00000001, 0b00010000};

class Tower_Sketch
{
public:
	uint32_t w_2, w_4;
	unsigned char* table_2;
	unsigned char* table_4;
	BOBHash *hash_2, *hash_4;

public:
	Tower_Sketch(int TSmemory, int hash_seed = 1000)
	{
		w_2 =TSmemory * 4;
		w_4 =TSmemory * 2;
		table_2 = new unsigned char[TSmemory];
		table_4 = new unsigned char[TSmemory];
		memset(table_2, 0, TSmemory);
		memset(table_4, 0, TSmemory);

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

        if(res_2 == 3 && res_4 == 15)
            return;
        else if((res_2 == 3 && res_4 != 15) ||
            (res_2 != 3 && res_4 != 15 && res_2 > res_4))
            table_4[m_4] += delt_4[n_4];
        else if((res_2 != 3 && res_4 == 15) ||
            (res_2 != 3 && res_4 != 15 && res_2 < res_4))
            table_2[m_2] += delt_2[n_2];
        else{
            table_4[m_4] += delt_4[n_4];
            table_2[m_2] += delt_2[n_2];
        }
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
        
        if(res_2 == 3) return res_4; 
        else return res_2 < res_4 ? res_2 : res_4;
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

        if(res_2 == 3 && res_4 == 15) //都溢出
            return 15;
        else if((res_2 == 3 && res_4 != 15) ||
            (res_2 != 3 && res_4 != 15 && res_2 > res_4))
        {    
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }     
        else if((res_2 != 3 && res_4 == 15) ||
            (res_2 != 3 && res_4 != 15 && res_2 < res_4))
        {   
            table_2[m_2] += delt_2[n_2];
            /*if(res_2 == 2) return res_4;
            else return res_2 + 1;*/
            return res_2 + 1; 
            /*注：此处存在特殊情况：即2bit整数为2，即将+1变成3(溢出状态)，
            如果按原towersketch，插入和查询分开进行，是无法区分刚刚达到溢出，还是早就溢出了，
            但插入和查询一起做可以得到，所以函数InsertAndQuery 与 Insert + Query的结果有几处不同，
            在全caida数据集上共有3000多处不同：仅发生在2bit数将加到3，而4bit数已经超过3，
            此时感觉查询结果为3更合理（等待后续实验）
            */
        }
        else
        {
            table_2[m_2] += delt_2[n_2];
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
    }

    ~Tower_Sketch()
    {
        delete table_2;
        delete table_4;
        delete hash_2;
        delete hash_4;
    }
};
#endif