#include <bits/stdc++.h>
#include "includes/DataProcess.h"
#include "includes/params.h"

using namespace std;

static int readnum = -1;  // 读取包总数
static int read_flow = 0; // 读取流总数

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;
vector<Packet> dataset;
const char *datafile = "../data/loss_data.txt";

int main(int argc, char *argv[])
{
    /*
    input = loadCAIDA18(readnum);
    gt = groundtruth(input, read_flow, readnum);
    flowSetMoreThan256 = getFlowLargerThan256(gt);
    dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);
    */

    return 0;
}