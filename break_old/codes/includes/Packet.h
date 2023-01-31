#ifndef _PACKET_H
#define _PACKET_H

#include <bits/stdc++.h>
using namespace std;
typedef uint32_t Seq;

class Packet // 包
{
public:
    uint32_t id;
    uint32_t time;
    uint32_t seq;
    Packet() {}
    Packet(uint32_t id_, uint32_t time_, uint32_t seq_) : id(id_), time(time_), seq(seq_) {}
};
#endif