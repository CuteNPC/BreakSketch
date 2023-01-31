#ifndef _TESTACC_H
#define _TESTACC_H

#include <bits/stdc++.h>
#include "Packet.h"
#include "Break_Sketch.h"
using namespace std;

class Acc
{
public:
    uint32_t TP = 0;
    uint32_t FP = 0;
    uint32_t TN = 0;
    uint32_t FN = 0;
    double PR = 0;
    double RR = 0;
    double AC = 0;
    double F1 = 0;
    void calc()
    {
        PR = (double)TP / (TP + FP);
        RR = (double)TP / (TP + FN);
        AC = (double)(TP + TN) / (TP + FP + TN + FN);
        F1 = 2 * PR * RR / (PR + RR);
    }
    void Print()
    {
        cout << "F1=" << F1 << endl;
        cout << "PR=" << PR << endl;
        cout << "RR=" << RR << endl;
        cout << "AC=" << AC << endl;
    }
};

void GetOutput(Break_Sketch *break_sketch, vector<Packet> &dataset, string filename)
{
    vector<char> output;
    for (Packet packet : dataset)
        output.push_back(break_sketch->Solution(packet) ? 'b' : 'n');

    string path = string("output/") + filename + string(".txt");
    ofstream fout(path, ios::trunc | ios::out);

    for (char c : output)
        fout << c << '\n';
    fout.close();
}

Acc CompareOutput(string Testfile, string Standardfile = "output/standard_output.txt")
{
    ifstream testinput(string("output/") + Testfile + string(".txt"));
    ifstream standardinput(Standardfile);
    Acc acc;
    char t, s;
    while ((testinput >> t) && (standardinput >> s))
    {
        if (s == 'b')
        {
            if (t == 'b')
                acc.TP++;
            else
            {
                acc.FN++;
            }
        }
        else
        {
            if (t == 'b')
                acc.FP++;
            else
                acc.TN++;
        }
    }

    acc.calc();
    testinput.close();
    standardinput.close();
    return acc;
}

#endif