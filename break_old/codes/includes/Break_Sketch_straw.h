#ifndef _BREAK_SKETCH_STRAW_H
#define _BREAK_SKETCH_STRAW_H

#include "BOBHash.h"
#include "params.h"
#include "Packet.h"
#include <bits/stdc++.h>
#include "Break_Sketch.h"
#define REPORT_TIME_BREAK_SKETCH_STRAW 0
using namespace std;

class Break_Sketch_straw : public Break_Sketch
{
public:
    struct cell
    {
        uint32_t flowID;
        uint32_t seq;
    };
    int w;           // w为bucket的个数
    cell *tables[4]; // 4张table，每张w个cell
    BOBHash *hash[4];
    int index[4];

public:
    Break_Sketch_straw(int memory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        w = memory / 32;

        for (int i = 0; i < 4; i++)
        {
            tables[i] = new cell[w];
            memset(tables[i], 0, sizeof(cell) * w);
            hash[i] = new BOBHash(i + hash_seed);
        }

        srand(1898);
        // srand((unsigned)time(NULL)); 引入时间作为随机数种子
    }

    void Insert(const Packet &packet)
    {
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;

        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].seq == 0) // 存在空cell
            {
                tables[i][index[i]].flowID = packet.id;
                tables[i][index[i]].seq = packet.seq;
                return;
            }
        }
        // 随机踢出一个
        int rand_i = rand() % 4;
        tables[rand_i][index[rand_i]].flowID = packet.id;
        tables[rand_i][index[rand_i]].seq = packet.seq;
    }

#if REPORT_TIME_BREAK_SKETCH_STRAW
    bool Solution(Packet packet) // 返回是否发生断流
    {
        static long long tm0 = 0, tm1 = 0, tm2 = 0, tm3 = 0;
        static int cnt = 0;
        clock_t first, second;

        first = clock();
        second = clock();
        int tmp = second - first;
        tm0 += tmp;

        first = clock();
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;
        second = clock();
        tm1 += second - first;

        first = clock();
        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].flowID == packet.id)
            {
                uint32_t delta = packet.seq - tables[i][index[i]].seq;
                tables[i][index[i]].seq = packet.seq;
                if (delta <= 3)
                {
                    second = clock();
                    tm2 += second - first;
                    tm3 += tmp;
                    if ((++cnt) % 100000 == 0)
                    {
                        double sum = tm1 - tm0 + tm2 - tm0 + tm3 - tm0;
                        cout << "straw: " << fixed << setprecision(4) << (tm1 - tm0) / sum << ' ' << (tm2 - tm0) / sum << ' ' << (tm3 - tm0) / sum << endl;
                        cout << "straw: " << fixed << setprecision(4) << tm1 - tm0 << ' ' << tm2 - tm0 << ' ' << tm3 - tm0 << endl;
                    }
                    return false;
                }
                else
                {
                    second = clock();
                    tm2 += second - first;
                    tm3 += tmp;
                    if ((++cnt) % 100000 == 0)
                    {
                        double sum = tm1 - tm0 + tm2 - tm0 + tm3 - tm0;
                        cout << "straw: " << fixed << setprecision(4) << (tm1 - tm0) / sum << ' ' << (tm2 - tm0) / sum << ' ' << (tm3 - tm0) / sum << endl;
                        cout << "straw: " << fixed << setprecision(4) << tm1 - tm0 << ' ' << tm2 - tm0 << ' ' << tm3 - tm0 << endl;
                    }
                    return true;
                }
            }
        }
        second = clock();
        tm2 += second - first;

        first = clock();
        this->Insert(packet);
        second = clock();
        tm3 += second - first;

        if ((++cnt) % 100000 == 0)
        {
            double sum = tm1 - tm0 + tm2 - tm0 + tm3 - tm0;
            cout << "straw: " << fixed << setprecision(4) << (tm1 - tm0) / sum << ' ' << (tm2 - tm0) / sum << ' ' << (tm3 - tm0) / sum << endl;
            cout << "straw: " << fixed << setprecision(4) << tm1 - tm0 << ' ' << tm2 - tm0 << ' ' << tm3 - tm0 << endl;
        }
        return false;
    }

#else
    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        for (int i = 0; i < 4; i++)
            index[i] = (hash[i]->run((const char *)&packet.id, KEY_LEN)) % w;

        for (int i = 0; i < 4; i++)
        {
            if (tables[i][index[i]].flowID == packet.id)
            {
                uint32_t delta = packet.seq - tables[i][index[i]].seq;
                tables[i][index[i]].seq = packet.seq;
                if (delta <= 3)
                    return false;
                else
                    return true;
            }
        }
        this->Insert(packet);
        return false;
    }
    string Name()
    {
        return string("Break_Sketch_straw");
    }

#endif
    ~Break_Sketch_straw()
    {
        for (int i = 0; i < 4; i++)
        {
            delete[] tables[i];
            delete hash[i];
        }
    }
};

#endif