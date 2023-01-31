#ifndef _PAINT_H_
#define _PAINT_H_

#include <bits/stdc++.h>
#include "params.h"
#include "Count_Break.h"
#include "Break_Sketch_straw.h"
#include "Break_Sketch_basic_1.h"
#include "Break_Sketch_basic_2.h"
#include "Break_Sketch_improved.h"
#include "Break_Sketch_Optimization_1.h"
#include "Break_Sketch_Optimization_2.h"

void measureTime_toCSV_(vector<Packet> &dataset, int memory, int TSmemory, int Xnum)
{
    clock_t start, finish;
    double duration;

    Break_Sketch *break_sketch[6];

    break_sketch[0] = new Break_Sketch_straw(memory);
    break_sketch[1] = new Break_Sketch_basic_1(memory, TSmemory);
    break_sketch[2] = new Break_Sketch_basic_2(memory, TSmemory);
    break_sketch[3] = new Break_Sketch_improved(memory, TSmemory);
    break_sketch[4] = new Break_Sketch_Optimization_1(memory, TSmemory);
    break_sketch[5] = new Break_Sketch_Optimization_2(memory, TSmemory);

    for (int i = 0; i < 6; i++)
    {
        string outfile = string("res/time/") + break_sketch[i]->Name() + string("_Time.txt");

        ifstream ftest(outfile);
        if (ftest)
            ftest.close();
        else
        {
            ofstream fout(outfile, ios_base::out | ios_base::app);
            fout << "X,Y\n";
            fout.close();
        }

        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch[i]->Solution(packet);
        finish = clock();

        ofstream fout(outfile, ios_base::out | ios_base::app);
        fout << Xnum << "," << ((double)finish - start) / CLOCKS_PER_SEC << endl;

        fout.close();
        delete break_sketch[i];
    }
}

// 新增 model 参数，决定是向 res/F1_diff_memory/ 还是向 res/F1_diff_packets/ 打印测试结果
void measureF1_toCSV_(vector<Packet> &dataset, int memory, int TSmemory, int Xnum, int model)
{
    uint32_t correct_report[6] = {0}, wrong_report[6] = {0}, loss_report[6] = {0};

    clock_t start, finish;
    double duration;

    Break_Sketch *break_sketch[6];

    break_sketch[0] = new Break_Sketch_straw(memory);
    break_sketch[1] = new Break_Sketch_basic_1(memory, TSmemory);
    break_sketch[2] = new Break_Sketch_basic_2(memory, TSmemory);
    break_sketch[3] = new Break_Sketch_improved(memory, TSmemory);
    break_sketch[4] = new Break_Sketch_Optimization_1(memory, TSmemory);
    break_sketch[5] = new Break_Sketch_Optimization_2(memory, TSmemory);

    Count_Break CB;

    for (Packet packet : dataset)
    {
        bool C_res = CB.Solution(packet);
        for (int i = 0; i < 6; i++)
        {
            int Sketch_res = break_sketch[i]->Solution(packet);
            if (Sketch_res && C_res)
                correct_report[i]++;
            else if (Sketch_res && !C_res)
            {
                wrong_report[i]++;
            }
            else if (!Sketch_res && C_res)
            {
                loss_report[i]++;
                //if (i >= 2) // 输出发生loss的seq号
                //    printf("BS[%d]: no.%d loss_report seq is %u\n", i, loss_report[i], packet.seq);
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        string outfile;
        if (model == 1)
            outfile = string("res/F1_diff_memory/") + break_sketch[i]->Name() + string("_F1.txt");
        else if (model == 2)
            outfile = string("res/F1_diff_packets/") + break_sketch[i]->Name() + string("_F1.txt");

        ifstream ftest(outfile);
        if (ftest)
            ftest.close();
        else
        {
            ofstream fout(outfile, ios_base::out | ios_base::app);
            fout << "X,Y\n";
            fout.close();
        }

        double PR = (double)correct_report[i] / ((double)wrong_report[i] + correct_report[i]);
        double RR = (double)correct_report[i] / ((double)loss_report[i] + correct_report[i]);
        double F1score = (2 * PR * RR) / (PR + RR);

        ofstream fout(outfile, ios_base::out | ios_base::app);
        fout << Xnum << "," << F1score << endl;
        fout.close();
        delete break_sketch[i];
    }
}

void measureBFF1_toCSV_(vector<Packet> &dataset, int memory, int TSmemory, int Xnum)
{
    uint32_t correct_report[6] = {0}, wrong_report[6] = {0}, loss_report[6] = {0};

    clock_t start, finish;
    double duration;

    Break_Sketch *break_sketch[5];

    break_sketch[0] = new Break_Sketch_straw(memory);
    break_sketch[1] = new Break_Sketch_basic_1(memory, TSmemory);
    break_sketch[2] = new Break_Sketch_basic_2(memory, TSmemory);
    break_sketch[3] = new Break_Sketch_improved(memory, TSmemory);
    break_sketch[4] = new Break_Sketch_Optimization_1(memory, TSmemory);
    break_sketch[5] = new Break_Sketch_Optimization_2(memory, TSmemory);

    Count_Break CB;

    for (Packet packet : dataset)
    {
        bool C_res = CB.Solution(packet);
        for (int i = 0; i < 6; i++)
        {
            int Sketch_res = break_sketch[i]->Solution(packet);
            if (Sketch_res && C_res)
                correct_report[i]++;
            else if (Sketch_res && !C_res)
                wrong_report[i]++;
            else if (!Sketch_res && C_res)
                loss_report[i]++;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        string outfile = string("res/BFF1/") + break_sketch[i]->Name() + string("_BFF1.txt");

        ifstream ftest(outfile);
        if (ftest)
            ftest.close();
        else
        {
            ofstream fout(outfile, ios_base::out | ios_base::app);
            fout << "X,Y\n";
            fout.close();
        }

        double PR = (double)correct_report[i] / ((double)wrong_report[i] + correct_report[i]);
        double RR = (double)correct_report[i] / ((double)loss_report[i] + correct_report[i]);
        double F1score = (2 * PR * RR) / (PR + RR);

        ofstream fout(outfile, ios_base::out | ios_base::app);
        fout << Xnum << "," << F1score << endl;
        fout.close();
        delete break_sketch[i];
    }
}

#endif