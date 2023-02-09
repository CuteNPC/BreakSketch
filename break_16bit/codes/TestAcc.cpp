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
#include "includes/Break_Sketch_improved_noTS.h"
#include "includes/Break_Sketch_new.h"
#include "includes/Break_Sketch_upbound.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
#include "omp.h"
using namespace std;

vector<Packet> dataset;

/*写成宏定义，要不每次改都得全改一遍，烦死了  [｀Д´ ] */
#define Repeating_statement                                                                                   \
    Break_Sketch *break_sketch[9];                                                                            \
    break_sketch[1] = new Break_Sketch_straw(mem, hash_seed);                                                 \
    break_sketch[2] = new Break_Sketch_basic_1(mem, TSmem, hash_seed);                                        \
    break_sketch[3] = new Break_Sketch_basic_2(mem, TSmem, hash_seed);                                        \
    break_sketch[4] = new Break_Sketch_improved(mem, TSmem, hash_seed);                                       \
    break_sketch[5] = new Break_Sketch_improved_noTS(mem, hash_seed);                                         \
    break_sketch[6] = new Break_Sketch_new(mem, hash_seed);                                                   \
    break_sketch[7] = new Break_Sketch_Optimization_1(mem, TSmem, hash_seed, hash_seed + 10, hash_seed + 20); \
    break_sketch[8] = new Break_Sketch_Optimization_2(mem, TSmem, hash_seed, hash_seed + 10, hash_seed + 20); \
                                                                                                              \
    for (int i = 1; i <= 8; i++)

const char csvheader[1024] =
    "X\
,Break_Sketch_straw\
,Break_Sketch_basic_1\
,Break_Sketch_basic_2\
,Break_Sketch_improved\
,Break_Sketch_improved_noTS\
,Break_Sketch_new\
,Break_Sketch_Optimization_1\
,Break_Sketch_Optimization_2\n";

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

    for (int j = 1; j <= 49; j += 2)
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
            if (i == 6)
            {
                printf("i=%d\n", i);
                acc.Print();
            }
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

int main(int argc, char *argv[])
{
    cout << Load(dataset, "../data/loss_data.txt") << endl;

    if (access("../result", 0))
        mkdir("../result", S_IRWXU);
    if (access("../result/output", 0))
        mkdir("../result/output", S_IRWXU);

#pragma omp parallel for
    for (int i = 0; i < 9; i++)
    {
        diff_Totmem(i);
    }

    sleep(3);
    // for (int i = 0; i < 10; i++)
    //{
    //     time(i);
    // }

    return 0;
}