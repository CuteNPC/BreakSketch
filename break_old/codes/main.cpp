#include <bits/stdc++.h>
#include "includes/DataProcess.h"
#include "includes/params.h"
#include "includes/paint.h"

using namespace std;

static int readnum = -1;  // 读取包总数
static int read_flow = 0; // 读取流总数

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;
vector<Packet> dataset;
const char *datafile = "../data/loss_data.txt";

/* 绘制 相同输入(全CAIDA集)、不同内存大小下的F1图 */
void paint_F1_diff_memory()
{
    for (uint64_t i = 10, totmemory = 3 * 1024; totmemory <= 48 * 1024 * 1024; i += 2, totmemory *= 4)
    {
        srand(RANDOM_SEED);
        measureF1_toCSV_(dataset, totmemory, totmemory / 5, i, 1);
    }
    printf("paint_F1_diff_memory finished!\n");
}

/* 绘制 内存固定，改变Bloomfilter所占内存比例的F1图 */
void paint_BFF1()
{
    for (int i = 1; i <= 15; i++)
    {
        srand(RANDOM_SEED);
        measureBFF1_toCSV_(dataset, 3 * 1024 * 1024, 1024 * 1024 * 3 * i / 16, i);
    }
    printf("paint_BFF1 finished!\n");
}

/* 绘制 输入包数不同（保持内存与输入流总数的比例固定）下的 F1 图 */
void paint_F1_diff_packets()
{
    for (int readnum = 3000000, i = 3; readnum < 27100000; readnum += 3000000, i += 3)
    {
        /*gt = groundtruth(input, read_flow, readnum);
        dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);*/
        
        int totmemory = read_flow * 3 * 4; // 固定总内存是输入流总数的12倍（使得improved版本的cell数约为流总数的2.5倍）
        measureF1_toCSV_(dataset, totmemory, totmemory / 5, i, 2);
    }
    printf("paint_F1_diff_packets finished!\n");
}

/* 绘制 输入包数不同（保持内存与输入流总数的比例固定）下的 Time 图 */
void paint_Time_diff_packets()
{
    for (int readnum = 3000000, i = 3; readnum < 27100000; readnum += 3000000, i += 3)
    {
        gt = groundtruth(input, read_flow, readnum);
        dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);
        int totmemory = read_flow * 3 * 4; // 固定总内存是输入流总数的12倍（使得improved版本的cell数约为流总数的2.5倍）
        measureTime_toCSV_(dataset, totmemory, totmemory / 5, readnum);
    }
    printf("paint_Time_diff_packets finished!\n");
}

int main(int argc, char *argv[])
{
    /*input = loadCAIDA18(readnum);
    gt = groundtruth(input, read_flow, readnum);
    flowSetMoreThan256 = getFlowLargerThan256(gt);
    dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);
    */
    Load(dataset);

    return 0;
}
