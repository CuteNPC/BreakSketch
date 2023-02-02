#include <bits/stdc++.h>
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/params.h"
using namespace std;

static int read_flow = 0; // 读取流总数

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;

vector<Packet> complete_data;
map<uint32_t, uint32_t> complete_data_flowcnt;

vector<Packet> loss_data;
vector<char> standard_output;

int main()
{
    input = loadCAIDA18();
    gt = groundtruth(input, read_flow);
    flowSetMoreThan256 = getFlowLargerThan256(gt);
    Addseq_with256(input, complete_data_flowcnt, complete_data, RANDOM_SEED);

    ofstream complete_data_fout("../data/complete_data.txt", ios::trunc | ios::out);
    for (auto elem : complete_data)
        complete_data_fout << elem.id << ' ' << elem.seq << ' ' << elem.time << '\n';
    complete_data_fout.close();

    cout << lossPacket_with256_New(complete_data, flowSetMoreThan256, loss_data, standard_output, LOSS_PROB, RANDOM_SEED) << endl;

    ofstream loss_data_fout("../data/loss_data.txt", ios::trunc | ios::out);
    for (auto elem : loss_data)
        loss_data_fout << elem.id << ' ' << elem.seq << ' ' << elem.time << '\n';
    loss_data_fout.close();

    int total_break = 0;
    ofstream standard_output_fout("../data/standard_output.txt", ios::trunc | ios::out);
    for (auto elem : standard_output){
        if(elem == 'b') total_break ++;
        standard_output_fout << elem << '\n';
    }
    standard_output_fout.close();

    cout << total_break <<endl;
    return 0;
}