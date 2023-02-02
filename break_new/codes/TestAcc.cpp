#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "includes/TestAcc.h"
#include "includes/Packet.h"
#include "includes/Break_Sketch_straw.h"
#include "includes/Break_Sketch_basic_1.h"
#include "includes/Break_Sketch_basic_2.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
#include "includes/Break_Sketch_Optimization_3.h"
#include "omp.h"
using namespace std;

/*写成宏定义，要不每次改都得全改一遍，烦死了  [｀Д´ ] */
#define Repeating_statement                                                                                   \
    Break_Sketch *break_sketch[8];                                                                            \
    break_sketch[1] = new Break_Sketch_straw(mem, hash_seed);                                                 \
    break_sketch[2] = new Break_Sketch_basic_1(mem, TSmem, hash_seed);                                        \
    break_sketch[3] = new Break_Sketch_basic_2(mem, TSmem, hash_seed);                                        \
    break_sketch[4] = new Break_Sketch_improved(mem, TSmem, hash_seed);                                       \
    break_sketch[5] = new Break_Sketch_Optimization_1(mem, TSmem, hash_seed, hash_seed + 10, hash_seed + 20); \
    break_sketch[6] = new Break_Sketch_Optimization_2(mem, TSmem, hash_seed, hash_seed + 10, hash_seed + 20); \
    break_sketch[7] = new Break_Sketch_Optimization_3(mem, TSmem, hash_seed, hash_seed + 10, hash_seed + 20); \
                                                                                                              \
    for (int i = 1; i <= 7; i++)

const char csvheader[1024] =
    "X\
,Break_Sketch_straw\
,Break_Sketch_basic_1\
,Break_Sketch_basic_2\
,Break_Sketch_improved\
,Break_Sketch_Optimization_1\
,Break_Sketch_Optimization_2\
,Break_Sketch_Optimization_3\n";

int memarray[] = {96, 192, 384, 768, 1536, 3 * 1024, 6 * 1024, 12 * 1024, 24 * 1024, 48 * 1024};

vector<Packet> dataset;

/*
以下函数参数 n 控制向 result/.../[F1/Recall/Precision/time]_n.csv 里输出当次测量的结果
最终 paint.py 计算均值并绘图
*/

/*测量TS占内存不同比例的准确率*/
void diff_TSmem(int n)
{
    char FunName[32] = "diff_TSmem";
    string no = "0";
    no[0] += n;

    /* 随机生成hashseed，使结果更普遍 */
    srand(n * 101);
    int hash_seed = rand() % 1201;
    cout << hash_seed << endl;

    if (access("../result/TSmem_res", 0))
        mkdir("../result/TSmem_res", S_IRWXU);
    ofstream F1out("../result/TSmem_res/F1_" + no + ".csv", ios::trunc | ios::out);
    ofstream RRout("../result/TSmem_res/Recall_" + no + ".csv", ios::trunc | ios::out);
    ofstream PRout("../result/TSmem_res/Precision_" + no + ".csv", ios::trunc | ios::out);
    char filename_str[128];
    F1out << csvheader, RRout << csvheader, PRout << csvheader;

    for (int j = 1; j <= 17; j += 2)
    {
        int mem = 24 * 1024;
        int TSmem = j * mem / 64;
        F1out << j << "/64";
        RRout << j << "/64";
        PRout << j << "/64";

        Repeating_statement
        {
            sprintf(filename_str, "%s-%d-%s-%s", break_sketch[i]->Name().c_str(), mem, no.c_str(), FunName);
            GetOutput(break_sketch[i], dataset, filename_str);
            delete break_sketch[i];
            Acc acc = CompareOutput(filename_str, "../data/standard_output.txt");
            remove((string("../result/output/") + filename_str + string(".txt")).c_str());
            F1out << ',' << acc.F1;
            RRout << ',' << acc.RR;
            PRout << ',' << acc.PR;
        }
        F1out << endl;
        RRout << endl;
        PRout << endl;
    }
}

/* 测量不同内存大小的准确率 */
void diff_Totmem(int n)
{
    char FunName[32] = "diff_Totmem";
    string no = "0";
    no[0] += n;

    /* 随机生成hashseed，使结果更普遍 */
    srand(n * 101);
    int hash_seed = rand() % 1201;
    cout << hash_seed << endl;

    if (access("../result/Totmem_res", 0))
        mkdir("../result/Totmem_res", S_IRWXU);
    ofstream F1out("../result/Totmem_res/F1_" + no + ".csv", ios::trunc | ios::out);
    ofstream RRout("../result/Totmem_res/Recall_" + no + ".csv", ios::trunc | ios::out);
    ofstream PRout("../result/Totmem_res/Precision_" + no + ".csv", ios::trunc | ios::out);
    char filename_str[128];
    F1out << csvheader, RRout << csvheader, PRout << csvheader;
    for (int j = 0, totmemory = 96 * 8; totmemory <= 48 * 1024 * 64; j++, totmemory *= 2)
    {
        F1out << totmemory;
        RRout << totmemory;
        PRout << totmemory;

        int mem = totmemory;
        int TSmem = totmemory / 8;

        Repeating_statement
        {
            sprintf(filename_str, "%s-%d-%s-%s", break_sketch[i]->Name().c_str(), mem, no.c_str(), FunName);
            GetOutput(break_sketch[i], dataset, filename_str);
            delete break_sketch[i];
            Acc acc = CompareOutput(filename_str, "../data/standard_output.txt");
            remove((string("../result/output/") + filename_str + string(".txt")).c_str());
            F1out << ',' << acc.F1;
            RRout << ',' << acc.RR;
            PRout << ',' << acc.PR;
        }
        F1out << endl;
        RRout << endl;
        PRout << endl;
    }
}

/* 测量不同输入包数的时间(保持内存与包数的比例固定) */
void time(int n)
{
    string no = "0";
    no[0] += n;

    srand(n * 101);
    int hash_seed = rand() % 1201;

    clock_t start, finish;
    if (access("../result/time_res", 0))
        mkdir("../result/time_res", S_IRWXU);
    ofstream Tout("../result/time_res/time_" + no + ".csv", ios::trunc | ios::out);
    char filename_str[128];
    Tout << csvheader;

    for (int readnum = 3000000; readnum <= 27100000; readnum += 3000000)
    {
        Tout << readnum;
        int mem = readnum / 10;
        int TSmem = mem / 8;

        Repeating_statement
        {
            start = clock();
            for (int index = 0; index < readnum; index++)
                break_sketch[i]->Solution(dataset[index]);
            finish = clock();
            delete break_sketch[i];
            Tout << ',' << ((double)finish - start) / CLOCKS_PER_SEC;
        }
        Tout << endl;
    }
}

/* 测量不同的Op12 */
void diff_Op(int n)
{
    string no = "0";
    no[0] += n;

    /* 随机生成hashseed，使结果更普遍 */
    srand(n * 101);
    int hash_seed = rand() % 1201;
    cout << hash_seed << endl;

    if (access("../result/Op_res", 0))
        mkdir("../result/Op_res", S_IRWXU);
    ofstream F1out("../result/Op_res/F1_" + no + ".csv", ios::trunc | ios::out);
    ofstream RRout("../result/Op_res/Recall_" + no + ".csv", ios::trunc | ios::out);
    ofstream PRout("../result/Op_res/Precision_" + no + ".csv", ios::trunc | ios::out);
    char filename_str[128];
    F1out << "X,Break_Sketch_straw,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2,Break_Sketch_Optimization_3" << endl;
    RRout << "X,Break_Sketch_straw,Break_Sketch_basic_1,Break_Sketch_basic_2,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;
    PRout << "X,Break_Sketch_straw,Break_Sketch_basic_1,Break_Sketch_basic_2,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;

    for (int j = 0, totmemory = 96 * 8; totmemory <= 48 * 1024 * 64; j++, totmemory *= 2)
    {
        F1out << totmemory;
        RRout << totmemory;
        PRout << totmemory;

        int mem = totmemory;
        int TSmem = totmemory / 8;

        Break_Sketch *break_sketch[7];
        break_sketch[1] = new Break_Sketch_straw(mem, hash_seed);
        break_sketch[2] = new Break_Sketch_basic_1(mem, TSmem, hash_seed);
        break_sketch[3] = new Break_Sketch_basic_2(mem, TSmem, hash_seed);
        break_sketch[4] = new Break_Sketch_improved(mem, TSmem, hash_seed);
        break_sketch[5] = new Break_Sketch_Optimization_1(mem, TSmem, hash_seed);
        break_sketch[6] = new Break_Sketch_Optimization_2(mem, TSmem, hash_seed);

        for (int i = 1; i <= 6; i++)
        {
            sprintf(filename_str, "%s-%d", break_sketch[i]->Name().c_str(), mem);
            GetOutput(break_sketch[i], dataset, filename_str);
            delete break_sketch[i];
            Acc acc = CompareOutput(filename_str, "../data/standard_output.txt");
            remove((string("../result/output/") + filename_str + string(".txt")).c_str());
            F1out << ',' << acc.F1;
            RRout << ',' << acc.RR;
            PRout << ',' << acc.PR;
        }
        F1out << endl;
        RRout << endl;
        PRout << endl;
    }
}

int main(int argc, char *argv[])
{
    cout << omp_get_num_threads() << endl;
    if (access("../result", 0))
        mkdir("../result", S_IRWXU);
    if (access("../result/output", 0))
        mkdir("../result/output", S_IRWXU);
    cout << Load(dataset, "../data/loss_data.txt") << endl;
    if (argc == 1)
    {
#pragma omp parallel for // 并行计算
        for (int i = 0; i < 20; i++)
        {
            if (i % 2)
                diff_Totmem(i / 2);
            else
                diff_TSmem(i / 2);
        }

        sleep(3);
        /*时间单线程计算，以防互相干扰*/
        for (int i = 0; i < 10; i++)
        {
            time(i);
        }
    }
    return 0;
}