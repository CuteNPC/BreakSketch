#ifndef _DATAPROCESS_H
#define _DATAPROCESS_H

#include <bits/stdc++.h>
#include "params.h"
#include "Packet.h"
using namespace std;

vector<pair<uint32_t, uint32_t>> loadCAIDA18(int read_num = -1, const char *filename = "../data/caida.dat")
{
    printf("Open %s \n", filename);
    FILE *pf = fopen(filename, "rb");
    if (!pf)
    {
        printf("%s not found!\n", filename);
        exit(-1);
    }

    vector<pair<uint32_t, uint32_t>> vec;
    double ftime = -1;
    char trace[30];
    uint32_t i = 0;
    while (fread(trace, 1, 21, pf) /* && vec.size()<1e5*/)
    {
        uint32_t tkey = *(uint32_t *)(trace);
        double ttime = *(double *)(trace + 13);
        if (ftime < 0)
            ftime = ttime;
        vec.push_back(pair<uint32_t, uint32_t>(tkey, uint32_t((ttime - ftime) * 10000000)));
        // if( i % 10000 ==0 ) printf("%u %u\n", tkey, int((ttime-ftime)*10000000));
        // 流持续60s
        if (++i == read_num)
            break;
    }
    printf("load %d packets\n", i);
    fclose(pf);
    return vec;
}

// 对照标准：<出现次数,流id>
vector<pair<uint32_t, uint32_t>> groundtruth(vector<pair<uint32_t, uint32_t>> &input, int &read_flow, int read_num = -1)
{
    map<uint32_t, uint32_t> cnt;
    uint32_t i = 0;
    for (auto [tkey, ttime] : input)
    {
        ++cnt[tkey];
        if (++i == read_num)
            break;
    }

    vector<pair<uint32_t, uint32_t>> ans;
    for (auto flow : cnt)
        ans.push_back({flow.second, flow.first});
    sort(ans.begin(), ans.end(), greater<pair<uint32_t, uint32_t>>());

    read_flow = (uint32_t)ans.size();
    printf("there are %d flows\n", read_flow);

    return ans;
}

// 获得包数大于等于256的流
set<uint32_t> getFlowLargerThan256(vector<pair<uint32_t, uint32_t>> &gt)
{
    set<uint32_t> ret;
    for (auto [cnt, flow] : gt)
        if (cnt >= 256)
            ret.insert(flow);
    return ret;
}

// 从<f,T>构造<f,T,seq>，抛弃包数小于256的包
uint32_t Addseq(vector<pair<uint32_t, uint32_t>> &input, set<uint32_t> &flowLargerThan256,
                map<uint32_t, uint32_t> &flowcnt, vector<Packet> &ret, int random_seed) /* double prob = LOSS_PROB, int read_num = -1)*/
{
    // 初始化随机数引擎

    mt19937 gen(random_seed);
    uniform_int_distribution<uint32_t> random_num;

    ret.clear();
    flowcnt.clear();
    for (auto elem : input)
    {
        if (flowLargerThan256.count(elem.first))
        {
            uint32_t this_seq;
            if (flowcnt.count(elem.first))
                this_seq = ++flowcnt[elem.first];
            else
                this_seq = flowcnt[elem.first] = random_num(gen);
            ret.push_back(Packet(elem.first, elem.second, this_seq));
        }
    }
    return flowcnt.size();
}

uint32_t lossPacket(vector<Packet> &complete_data, vector<Packet> &loss_data, 
                vector<char> &standard_output, double loss_prob, int random_seed)
{
    mt19937 gen(random_seed);
    uniform_real_distribution<double> random_num;
    loss_data.clear();
    standard_output.clear();
    map<uint32_t, uint32_t> losscnt;
    int cnt = 0;
    for (Packet packet : complete_data)
    {
        if (random_num(gen) <= loss_prob) // 发生丢包
        {
            if (losscnt.count(packet.id))
                losscnt[packet.id]++;
        }
        else // 不丢包
        {
            loss_data.push_back(packet);
            if (losscnt.count(packet.id))
                standard_output.push_back(losscnt[packet.id] >= 3 ? 'b' : 'n');
            else
                standard_output.push_back('n');
            losscnt[packet.id] = 0;
        }
    }
    return losscnt.size();
}

// 从<f,T>构造<f,T,seq>，但不会抛弃包数小于256的包
uint32_t Addseq_with256(vector<pair<uint32_t, uint32_t>> &input,
                map<uint32_t, uint32_t> &flowcnt, vector<Packet> &ret, int random_seed) /* double prob = LOSS_PROB, int read_num = -1)*/
{
    // 初始化随机数引擎

    mt19937 gen(random_seed);
    uniform_int_distribution<uint32_t> random_num;

    ret.clear();
    flowcnt.clear();
    for (auto elem : input)
    {
        uint32_t this_seq;
        if (flowcnt.count(elem.first))
            this_seq = ++flowcnt[elem.first];
        else
            this_seq = flowcnt[elem.first] = random_num(gen);
        ret.push_back(Packet(elem.first, elem.second, this_seq));
    }
    return flowcnt.size();
}


//不去小流的版本
uint32_t lossPacket_with256(vector<Packet> &complete_data, set<uint32_t> &flowLargerThan256, 
                vector<Packet> &loss_data, vector<char> &standard_output, double loss_prob, int random_seed)
{
    mt19937 gen(random_seed);
    uniform_real_distribution<double> random_num;
    loss_data.clear();
    standard_output.clear();
    map<uint32_t, uint32_t> losscnt;
    int cnt = 0;
    for (Packet packet : complete_data)
    {
        if ((flowLargerThan256.count(packet.id)) && random_num(gen) <= loss_prob)
        {
            if (losscnt.count(packet.id))
                losscnt[packet.id]++;
        }
        else
        {
            loss_data.push_back(packet);
            if (losscnt.count(packet.id))
                standard_output.push_back(losscnt[packet.id] >= 3 ? 'b' : 'n');
            else
                standard_output.push_back('n');
            losscnt[packet.id] = 0;
        }
    }
    return losscnt.size();
}

uint32_t Load(vector<Packet> &dataset, string filename = "../data/loss_data.txt", int readnum = -1)
{
    ifstream datainput(filename);
    Packet packet;
    int packcnt = 0;
    while (datainput >> packet.id >> packet.seq >> packet.time)
    {
        dataset.push_back(packet);
        if (++packcnt == readnum)
            break;
    }
    datainput.close();
    return packcnt;
}
// 从<f,T>构造<f,T,seq>并丢包
vector<Packet> Addseq_and_LossPacket(vector<pair<uint32_t, uint32_t>> &input, set<uint32_t> flowLargerThan256, double prob = LOSS_PROB, int read_num = -1)
{
    // 初始化随机数引擎
    mt19937 gen(RANDOM_SEED);
    uniform_real_distribution<double> random_double(0.0, 1.0);
    uniform_int_distribution<Seq> random_Seqtype;

    // 定义变量
    int t = 0;                   // 总计数
    vector<Packet> res;          // 返回的处理结束的数据集
    map<uint32_t, uint32_t> cnt; // 每个流的计数

    for (auto elem : input)
    {
        short this_seq;
        if (cnt.count(elem.first))
            this_seq = ++cnt[elem.first];
        else
            this_seq = cnt[elem.first] = random_Seqtype(gen);

        // 丢包条件
        if ((!flowLargerThan256.count(elem.first)) ||
            ((flowLargerThan256.count(elem.first)) && random_double(gen) > prob))
            res.push_back(Packet(elem.first, elem.second, this_seq));

        // if((flowLargerThan256.count(elem.first)) && random_double(gen) > prob)
        //     res.push_back(Packet(elem.first, elem.second, this_seq));

        t++; // 总计数
        if (t == read_num)
            break;
    }
    return res;
}
#endif