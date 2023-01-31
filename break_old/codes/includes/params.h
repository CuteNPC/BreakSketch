#ifndef _PARAMS_H
#define _PARAMS_H

#define JUDGEMENT

#define COUNTER_SIZE 32 // counter_size

#define WORD_SIZE 64

#define MAX_HASH_NUM 16 // hash_number  = d

#define FILTER_SIZE 32

#define KEY_LEN 4

#define testcycles 5

#define TOTAL_FLOW 253906

#define TOTAL_PACKETS 27121713

//#define BLOOMFILTER_SIZE 254000 * 15 // BF的size应该是多少呢…？是流总数的14倍（精确率大概0.01）

//#define BUCKET_SIZE 254000 * 3 / 4 // cell总数是流总数的2-3倍（取3倍）

#define RANDOM_SEED 547

#define LOSS_PROB 0.1

#endif