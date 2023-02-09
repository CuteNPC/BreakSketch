/*
break_16bit 里将所有的seq设置为uint16_t，
计划之后将flowID增加到8字节，并测试学长的算法
*/
#include <bits/stdc++.h>
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/params.h"
//#define MOREPROCESS
using namespace std;

static int read_flow = 0; // 读取流总数

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;

vector<Packet> complete_data;
map<uint32_t, uint16_t> complete_data_flowcnt;

vector<Packet> loss_data;
vector<char> standard_output;

map<uint32_t, uint32_t> break_count;
vector<RankNode> flowrank;

GlobalInfo globalinfo;
map<uint32_t, FlowInfo> flowInfo;
int main()
{
    input = loadCAIDA18();              // 加载caida数据集
    gt = groundtruth(input, read_flow); // 获得<出现次数，流id>

    cout << "加入seq..." << flush;
    flowSetMoreThan256 = getFlowLargerThan256(gt);                            // 获得大于256的包id
    Addseq_with256(input, complete_data_flowcnt, complete_data, flowSetMoreThan256, RANDOM_SEED); // 在数据集中加入seq
    cout << "done!" << endl;

    cout << "将完整数据集输出到“../data/complete_data.txt”..." << flush;
    ofstream complete_data_fout("../data/complete_data.txt", ios::trunc | ios::out);
    for (auto elem : complete_data)
        complete_data_fout << elem.id << ' ' << elem.seq << ' ' << elem.time << '\n';
    complete_data_fout.close();
    cout << "done!" << endl;

    cout << "生成丢包数据集..." << flush;
    lossPacket_with256_New(complete_data, flowSetMoreThan256, loss_data, standard_output, LOSS_PROB, RANDOM_SEED);
    cout << "done!" << endl;

    cout << "将丢包数据集输出到“../data/loss_data.txt”..." << flush;
    ofstream loss_data_fout("../data/loss_data.txt", ios::trunc | ios::out);
    for (auto elem : loss_data)
        loss_data_fout << elem.id << ' ' << elem.seq << ' ' << elem.time << '\n';
    loss_data_fout.close();
    cout << "done!" << endl;

    cout << "将标准输出集输出到“../data/standard_output.txt”..." << flush;
    ofstream standard_output_fout("../data/standard_output.txt", ios::trunc | ios::out);
    for (auto elem : standard_output)
        standard_output_fout << elem << '\n';
    standard_output_fout.close();
    cout << "done!" << endl;

    cout << "统计信息..." << flush;
    globalinfo.TotFlowCount = complete_data_flowcnt.size();
    globalinfo.OriTotPacketCount = complete_data.size();
    globalinfo.TotPacketCount = loss_data.size();
    for (int i = 0; i < globalinfo.TotPacketCount; i++)
    {
        if (standard_output[i] == 'b')
        {
            globalinfo.TotBreakCount++;
            break_count[loss_data[i].id]++;
        }
    }
    cout << "done!" << endl;

#ifdef MOREPROCESS

    for (auto packet : complete_data)
    {
        flowInfo[packet.id].flowID = packet.id;
        flowInfo[packet.id].oripkt++;
    }
    for (int i = 0; i < globalinfo.TotPacketCount; i++)
    {
        uint32_t id = loss_data[i].id;
        if (flowInfo[id].startplace == -1)
            flowInfo[id].startplace = i;
        flowInfo[id].endplace = i;
        flowInfo[id].totpkt++;
    }
    for (auto it = flowInfo.begin(); it != flowInfo.end(); it++)
    {
        it->second.duration = it->second.endplace - it->second.startplace + 1;
        it->second.losspkt = it->second.totpkt - it->second.curpkt;
        it->second.log10 = log10(it->second.totpkt);
        flowrank.push_back(RankNode(it->second.flowID, it->second.losspkt));
    }
    sort(flowrank.begin(), flowrank.end(), RankNodeCmp);
    for (int i = 0; i < globalinfo.TotFlowCount; i++)
        flowInfo[flowrank[i].fid].rank = i + 1;

    cout << "将信息输出到“../data/flowinfo.txt”..." << flush;
    ofstream flowinfo_output_fout("../data/flowinfo.txt", ios::trunc | ios::out);
    for (auto elem : flowInfo)
        flowinfo_output_fout << elem.second << endl;
    flowinfo_output_fout.close();
    cout << "done!" << endl;

    cout << "将其他输出到“../data/globalinfo.txt”..." << flush;
    ofstream globalinfo_output_fout("../data/globalinfo.txt", ios::trunc | ios::out);
    globalinfo_output_fout << globalinfo.TotFlowCount << endl;
    globalinfo_output_fout << globalinfo.TotPacketCount << endl;
    globalinfo_output_fout << globalinfo.OriTotPacketCount << endl;
    globalinfo_output_fout << globalinfo.TotBreakCount << endl;
    globalinfo_output_fout.close();
    cout << "done!" << endl;
#endif

    cout << "总流数: " << globalinfo.TotFlowCount << endl;
    cout << "完全数据集包数: " << globalinfo.OriTotPacketCount << endl;
    cout << "丢失数据集包数: " << globalinfo.TotPacketCount << endl;
    cout << "总断流数: " << globalinfo.TotBreakCount << endl;

    return 0;
}