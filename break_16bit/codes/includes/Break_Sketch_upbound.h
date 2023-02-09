#ifndef _BREAK_SKETCH_UPBOUND_H
#define _BREAK_SKETCH_UPBOUND_H

#include "BOBHash.h"
#include "params.h"
#include "Packet.h"
#include <bits/stdc++.h>
#include "Break_Sketch.h"
using namespace std;

class Break_Sketch_upbound : public Break_Sketch
{
public:
    struct cell
    {
        uint32_t flowID;
        uint16_t seq;
        uint32_t time;
        cell(){flowID = seq = time = 0;}
    };
    uint32_t w;
    cell *table;
    uint32_t tot = 0;

public:
    Break_Sketch_upbound(int memory)
        : Break_Sketch(memory)
    {
        w = memory / 2; /* 假设flowID和time不占空间 */
        table = new cell[w];
        memset(table, 0, sizeof(cell) * w);
    }

    bool Solution(const Packet &packet)
    {
        tot++;
        uint32_t min_t = 0xffffffff;
        uint32_t min_i = -1, ans_i =-1;
        int ans = 0;
        for(uint32_t i = 0; i < w ; i++)
        {
            if(table[i].flowID == packet.id){
                ans_i = i;
                ans = 1;
                break;
            }
            if(table[i].time < min_t){
                min_t = table[i].time;
                min_i = i;
            }
        }

        if(ans){
            uint16_t delta = packet.seq - table[ans_i].seq;
            table[ans_i].seq = packet.seq;
            table[ans_i].time = tot;
            return delta >= (uint16_t)4;
        }
        else{
            table[min_i].flowID = packet.id;
            table[min_i].seq = packet.seq;
            table[min_i].time = tot;
            return false;
        }
    }

    string Name()
    {
        return string("Break_Sketch_upbound");
    }

    ~Break_Sketch_upbound()
    {
        delete[] table;
    }
};

#endif