#include <bits/stdc++.h>
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/params.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch.h"
using namespace std;

struct flow
{
    uint32_t id;
    uint32_t init_num;
    uint32_t now_num;
    uint32_t loss_num;
    uint32_t stdbreak;
    uint32_t repbreak;
    uint32_t wrongrep;
    uint32_t lossrep;
    flow(uint32_t id_=0, uint32_t init_num_=0,
    uint32_t now_num_=0, uint32_t loss_num_=0,
    uint32_t stdbreak_=0, uint32_t repbreak_=0,
    uint32_t wrongrep_=0, uint32_t lossrep_=0):
    id(id_), init_num(init_num_), now_num(now_num_),
    loss_num(loss_num_), stdbreak(stdbreak_), repbreak(repbreak_),
    wrongrep(wrongrep_), lossrep(lossrep_){}
};
vector<flow> flows;

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;

vector<Packet> complete_data;
map<uint32_t, uint32_t> complete_data_flowcnt;
map<uint32_t, uint32_t> loss_data_cnt;
map<uint32_t, uint32_t> repbreak_cnt;
map<uint32_t, uint32_t> stdbreak_cnt;
map<uint32_t, uint32_t> wrongrep_cnt;
map<uint32_t, uint32_t> lossrep_cnt;

vector<Packet> loss_data;
vector<char> standard_output;

// 对照标准：<出现次数,流id>
vector<pair<uint32_t, uint32_t>> groundtruth__(vector<pair<uint32_t, uint32_t>> &input, int read_num = -1)
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
    sort(ans.begin(), ans.end(), less<pair<uint32_t, uint32_t>>());

    return ans;
}

int main()
{
    input = loadCAIDA18();
    gt = groundtruth__(input); //flows按初始时流的大小顺序升序排列
    for (auto elem : gt)
        flows.push_back(flow(elem.second, elem.first));

    flowSetMoreThan256 = getFlowLargerThan256(gt);
    Addseq_with256(input, complete_data_flowcnt, complete_data, RANDOM_SEED);
    lossPacket_with256_New(complete_data, flowSetMoreThan256, loss_data, 
                    standard_output, LOSS_PROB, RANDOM_SEED);

    Break_Sketch* sketch = new Break_Sketch_improved(24*1024, 24*1024/8);

    uint32_t m = loss_data.size();
    for (int i = 0; i < m; i++)
    {
        Packet packet = loss_data[i];
        ++loss_data_cnt[packet.id];
        bool stdres = (standard_output[i] == 'b');
        bool res = sketch->Solution(packet);
        if(res)
            ++repbreak_cnt[packet.id];
        if(stdres)
            ++stdbreak_cnt[packet.id];
        if(stdres && !res)
            ++lossrep_cnt[packet.id];
        if(!stdres && res)
            ++wrongrep_cnt[packet.id];
    }

    uint32_t n = flows.size();
    for (uint32_t i = 0; i < n; i++)
    {
        flows[i].now_num = loss_data_cnt[flows[i].id];
        flows[i].loss_num = flows[i].init_num - flows[i].now_num;
        
        flows[i].stdbreak = stdbreak_cnt[flows[i].id];
        flows[i].repbreak = repbreak_cnt[flows[i].id];
        flows[i].lossrep = lossrep_cnt[flows[i].id];
        flows[i].wrongrep = wrongrep_cnt[flows[i].id];
    }


    /* 统计写入文件 */
    ofstream data_fout("../data/analysize/flowData.csv", ios::trunc | ios::out);
    data_fout << "size,flownum,initpnum,nowpnum,lossnum,stdbreak,repbreak,lossrep,wrongrep\n"; 
    uint32_t delta = 1000;
    uint32_t nowsize = 1000, flow_num = 0, init_packets_num = 0, now_packets_num = 0;
    uint32_t stdbreaknum = 0, repbreaknum = 0, lossrepnum = 0, wrongrepnum = 0;
    for (auto elem : flows)
    {
        if(nowsize >= elem.init_num){
            flow_num ++;
            init_packets_num += elem.init_num;
            now_packets_num += elem.now_num;
            stdbreaknum += elem.stdbreak;
            repbreaknum += elem.repbreak;
            lossrepnum += elem.lossrep;
            wrongrepnum += elem.wrongrep;
        }
        else{
    data_fout << nowsize << ',' << flow_num << ',' << init_packets_num 
            << ',' << now_packets_num << ',' << init_packets_num - now_packets_num
            << ',' << stdbreaknum << ',' <<repbreaknum
            << ',' << lossrepnum << ',' << wrongrepnum << '\n';
            nowsize = ((elem.init_num + delta - 1) / delta) * delta;
            flow_num = 1;
            init_packets_num = elem.init_num;
            now_packets_num = elem.now_num;
            stdbreaknum = elem.stdbreak;
            repbreaknum = elem.repbreak;
            lossrepnum = elem.lossrep;
            wrongrepnum = elem.wrongrep;
        }
    }
    data_fout << nowsize << ',' << flow_num << ',' << init_packets_num 
            << ',' << now_packets_num << ',' << init_packets_num - now_packets_num
            << ',' << stdbreaknum << ',' <<repbreaknum
            << ',' << lossrepnum << ',' << wrongrepnum << '\n';
    data_fout.close();

    return 0;
}