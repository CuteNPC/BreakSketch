#ifndef _BLOOMFILTER_H
#define _BLOOMFILTER_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
using namespace std;

class BloomFilter
{
public:
	int w, m, k; // w:BF总位数；m:用于存储位数组的uint64个数; k: hash函数个数
	uint64_t *bit;
	BOBHash **hash;

public:
	BloomFilter(int BFmemory, int _k = 4, int hash_seed = 1000, double _eps = 0.0) // eps: 期望的错误率
	{
		w = BFmemory * 8;
		m = (BFmemory + 7) / 8;

		w = BFmemory;
		m = BFmemory / 8;

		/*if (_eps > 0) //若有错误率要求
			w = ceil(n * log2(1 / _eps) * 1.44);
		else
			w = read_flow * 6; //这里需要调一下参数，共n个元素时BF设置多少位呢？

		if (_k > 0)
			k = _k;
		else
			k = ceil((w / 1.44 / n) * log(2));*/
		k = _k;
		bit = new uint64_t[m];
		memset(bit, 0, sizeof(uint64_t) * m);

		hash = new BOBHash *[sizeof(BOBHash *) * k];

		for (int i = 0; i < k; i++) // init d hash functions
		{
			hash[i] = new BOBHash(i + hash_seed);
		}
	}

	void Insert(const char *str)
	{
		for (int i = 0; i < k; i++)
		{
			uint32_t index = (hash[i]->run(str, KEY_LEN)) % w;
			uint32_t m_num = index / 64;
			uint32_t b_num = index % 64;
			bit[m_num] |= (0x1 << b_num);
		}
	}

	bool Query(const char *str)
	{
		for (int i = 0; i < k; i++)
		{
			uint32_t index = (hash[i]->run(str, KEY_LEN)) % w;
			uint32_t m_num = index / 64;
			uint32_t b_num = index % 64;
			if ((bit[m_num] & (0x1 << b_num)) == 0)
				return false;
		}
		return true;
	}

	~BloomFilter()
	{
		for (int i = 0; i < k; i++)
		{
			delete hash[i];
		}
		delete hash;
		delete bit;
	}
};
#endif