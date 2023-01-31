#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "includes/TestAcc.h"
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/Break_Sketch_straw.h"
#include "includes/Break_Sketch_basic_1.h"
#include "includes/Break_Sketch_basic_2.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
using namespace std;
#define TESTSIZE 10
int memarray[] = {96, 192, 384, 768, 1536, 3 * 1024, 6 * 1024, 12 * 1024, 24 * 1024, 48 * 1024};

vector<Packet> dataset;
int main()
{
    cout << Load(dataset, "../data/loss_data.txt") << endl;
    if (access("res2", 0))
        mkdir("res2", S_IRWXU);
    ofstream F1out("res2/F1.csv", ios::trunc | ios::out);
    ofstream RRout("res2/Recall.csv", ios::trunc | ios::out);
    ofstream PRout("res2/Precision.csv", ios::trunc | ios::out);
    char filename_str[128];
    F1out << "X,Break_Sketch_straw,Break_Sketch_basic_1,Break_Sketch_basic_2,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;
    RRout << "X,Break_Sketch_straw,Break_Sketch_basic_1,Break_Sketch_basic_2,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;
    PRout << "X,Break_Sketch_straw,Break_Sketch_basic_1,Break_Sketch_basic_2,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;
    /*
        for (int j = 0, totmemory = 96 * 8; totmemory <= 48 * 1024 * 64; j++, totmemory *= 2)
        {
            F1out  << totmemory;
            RRout  << totmemory;
            PRout  << totmemory;

            int mem = totmemory;
            int BFmem = totmemory / 8;
    */
    for (int j = 1; j <= 7; j++)
    {
        int mem = 24 * 1024;
        int TSmem = j * mem / 8;
        F1out << j << "/8";
        RRout << j << "/8";
        PRout << j << "/8";
        Break_Sketch *break_sketch[7];
        break_sketch[1] = new Break_Sketch_straw(mem);
        break_sketch[2] = new Break_Sketch_basic_1(mem, TSmem);
        break_sketch[3] = new Break_Sketch_basic_2(mem, TSmem);
        break_sketch[4] = new Break_Sketch_improved(mem, TSmem);
        break_sketch[5] = new Break_Sketch_Optimization_1(mem, TSmem);
        break_sketch[6] = new Break_Sketch_Optimization_2(mem, TSmem);

        for (int i = 1; i <= 6; i++)
        {
            sprintf(filename_str, "%s-%d", break_sketch[i]->Name().c_str(), mem);
            GetOutput(break_sketch[i], dataset, filename_str);
            delete break_sketch[i];
            Acc acc = CompareOutput(filename_str, "../data/standard_output.txt");
            remove((string("output/") + filename_str + string(".txt")).c_str());
            F1out << ',' << acc.F1;
            RRout << ',' << acc.RR;
            PRout << ',' << acc.PR;
        }
        F1out << endl;
        RRout << endl;
        PRout << endl;
    }

    return 0;
}