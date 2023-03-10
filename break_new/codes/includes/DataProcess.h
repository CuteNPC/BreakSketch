#ifndef _DATAPROCESS_H
#define _DATAPROCESS_H
#define Jstart 3
#define Jend 20
#define Base 0.6
/*记录：
    Base： 0.1     0.2    0.25     0.3     0.4     0.5     0.6
    断流： 1400   13000   27000    50000  140000  315000  630000
*/

#include <bits/stdc++.h>
#include "params.h"
#include "Packet.h"
using namespace std;

class RankNode
{
public:
    uint32_t fid;
    uint32_t pkt;
    uint32_t rank;
    RankNode(uint32_t fid_, uint32_t pkt_)
    {
        fid = fid_, pkt = pkt_;
    }
    friend bool RankNodeCmp(RankNode a, RankNode b)
    {
        return a.pkt > b.pkt;
    }
};

class FlowInfo
{
public:
    uint32_t flowID = 0; // 完全数据集获得

    uint32_t curplace = 0;    // 不获得
    uint32_t startplace = -1; // 丢失数据集中获得
    uint32_t endplace = 0;    // 丢失数据集中获得
    uint32_t duration = 0;    // 处理获得

    uint32_t curpkt = 0; // 不获得
    uint32_t totpkt = 0; // 丢失数据集中获得
    uint32_t rank = 0;   // 处理获得
    float log10 = 0;     // 处理获得

    uint32_t oripkt = 0;  // 完全数据集获得
    uint32_t losspkt = 0; // 处理获得

    uint32_t curbreak = 0;    // 不获得
    uint32_t reportbreak = 0; // 测量获得
    uint32_t totbreak = 0;    // 标准输出集中获得

    friend ostream &operator<<(ostream &o, const FlowInfo &f)
    {
        o << f.flowID << ' '; // 完全数据集获得

        o << f.curplace << ' ';
        o << f.startplace << ' ';
        o << f.endplace << ' ';
        o << f.duration << ' ';

        o << f.curpkt << ' ';
        o << f.totpkt << ' ';
        o << f.rank << ' ';
        o << f.log10 << ' ';

        o << f.oripkt << ' ';
        o << f.losspkt << ' ';

        o << f.curbreak << ' ';
        o << f.reportbreak << ' ';
        o << f.totbreak << ' ';
        return o;
    }

    friend istream &operator>>(istream &i, FlowInfo &f)
    {
        i >> f.flowID; // 完全数据集获得

        i >> f.curplace;
        i >> f.startplace;
        i >> f.endplace;
        i >> f.duration;

        i >> f.curpkt;
        i >> f.totpkt;
        i >> f.rank;
        i >> f.log10;

        i >> f.oripkt;
        i >> f.losspkt;

        i >> f.curbreak;
        i >> f.reportbreak;
        i >> f.totbreak;
        return i;
    }
};

class GlobalInfo
{
public:
    int TotFlowCount;

    int TotPacketCount;
    int OriTotPacketCount;

    int TotBreakCount;

    void save(string filename = "../data/globalinfo.txt")
    {
        ofstream o(filename, ios::trunc | ios::out);
        o << TotFlowCount << endl;
        o << TotPacketCount << endl;
        o << OriTotPacketCount << endl;
        o << TotBreakCount << endl;
        o.close();
    }

    void load(string filename = "../data/globalinfo.txt")
    {
        ifstream i(filename);
        i >> TotFlowCount;
        i >> TotPacketCount;
        i >> OriTotPacketCount;
        i >> TotBreakCount;
        i.close();
    }
};

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

// 从<f,T>构造<f,T,seq>，但不抛弃包数小于256的包
uint32_t Addseq_with256(vector<pair<uint32_t, uint32_t>> &input,
                        map<uint32_t, uint32_t> &flowcnt, vector<Packet> &ret, int random_seed)
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

// 不去小流，不一次丢多个包的版本
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

// 不去小流，可以一次丢多个包的版本
uint32_t lossPacket_with256_New(vector<Packet> &complete_data, set<uint32_t> &flowLargerThan256,
                                vector<Packet> &loss_data, vector<char> &standard_output, double loss_prob, int random_seed)
{
    mt19937 gen(random_seed);
    uniform_real_distribution<double> random_real;
    uniform_int_distribution<uint32_t> random_int(Jstart, Jend); /*需要改一下函数名*/
    loss_data.clear();                                           /*丢包后的序列*/
    standard_output.clear();                                     /*生成标准输出*/
    map<uint32_t, uint32_t> losscnt;                             /*已连续丢失的计数，用来生成standard_output*/
    map<uint32_t, uint32_t> ToBeLost;                            /*还需要连续丢失的计数*/
    int cnt = 0;
    uint32_t total = complete_data.size();
    uint32_t totbreak = 0;
    for (uint32_t i = 0; i < total; ++i)
    {
        Packet packet = complete_data[i];
        if (!flowLargerThan256.count(packet.id)) /*是小流*/
        {
            /*不丢包*/
            loss_data.push_back(packet);
            standard_output.push_back('n');
        }
        else /*不是小流*/
        {
            if (ToBeLost[packet.id] == 0) /*没有还应该丢的包，开始取随机*/
            {
                uint32_t j = random_int(gen); /*生成随机数*/
                if (random_real(gen) <= pow(Base, j))
                {
                    /*中奖*/
                    ToBeLost[packet.id] = j;
                }
                else if (random_real(gen) <= 0.01)
                {
                    ToBeLost[packet.id] = 1;
                }
            }

            if (ToBeLost[packet.id] == 0)
            {
                /*仍不丢*/
                loss_data.push_back(packet);
                if (losscnt.count(packet.id))
                    standard_output.push_back(losscnt[packet.id] >= 3 ? 'b' : 'n');
                else
                    standard_output.push_back('n');
                losscnt[packet.id] = 0;
            }
            else
            {
                /*丢包*/
                if (losscnt.count(packet.id))
                    losscnt[packet.id]++;
                /*待丢包计数减一*/
                ToBeLost[packet.id]--;
            }
        }
    }
    return losscnt.size();
}

uint32_t Load(vector<Packet> &dataset, string filename = "../data/loss_data.txt", int readnum = -1)
{
    dataset.clear();
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

void LoadFlowInfo(map<uint32_t, FlowInfo> &Flowinfo, string filename = "../data/flowinfo.txt", int readnum = -1)
{
    Flowinfo.clear();
    ifstream FlowInfoInput(filename);
    FlowInfo flowinfo;
    while (FlowInfoInput >> flowinfo)
        Flowinfo[flowinfo.flowID] = flowinfo;
    FlowInfoInput.close();
    return;
}

void LoadStandard(vector<char> &vec, string filename = "../data/standard_output.txt", int readnum = -1)
{
    vec.clear();
    ifstream Input(filename);
    char c;
    while (Input >> c)
        vec.push_back(c);
    Input.close();
    return;
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

        t++; // 总计数
        if (t == read_num)
            break;
    }
    return res;
}

#endif