#define DEBUG
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
#include "includes/Count_Break.h"
using namespace std;

vector<Packet> dataset;
map<uint32_t, uint32_t> mp;
map<uint32_t, uint32_t> mp2;
map<uint32_t, uint32_t> mp3;
vector<uint32_t> vec;
set<uint32_t> s2;

class Node
{
public:
    int id, f;
};

bool mycmp(Node a, Node b)
{
    return a.f > b.f;
}

int main()
{
    Load(dataset);
    for (auto packet : dataset)
    {
        mp[packet.id]++;
    }

    Node *node = new Node[mp.size()];
    int p = 0;
    for (auto elem : mp)
    {
        node[p].id = elem.first;
        node[p].f = elem.second;
        p++;
    }
    sort(node, node + mp.size(), mycmp);

    for (int i = 0; i < mp.size(); i++)
    {
        mp2[node[i].id] = i + 1;
    }
    Break_Sketch_improved Sketch(1024 + 8192, 1024);
    int cnt = 0;
    cout << "当前通过TS的流数/当前总流数\n";
    for (auto packet : dataset)
    {
        // printf("%d : \n", ++cnt);
        // printf("编号：%u 排名：%u 当前包数：%u/%u\n", packet.id, mp2[packet.id], ++mp3[packet.id], mp[packet.id]);
        // packet.PrintInfo();
        int num = Sketch.Solution(packet);
        // cout << "Break ? : " << num << endl;
        // cout << endl;
        s2.insert(packet.id);
        ++cnt;
        if (cnt <= 100000)
        {
            if (cnt % 10000 == 0)
                printf("前%lu个包: %lu / %lu 过滤掉：%lu \n", cnt, s.size(), s2.size(), s2.size() - s.size());
        }
        else if (cnt <= 1000000)
        {
            if (cnt % 100000 == 0)
                printf("前%lu个包: %lu / %lu 过滤掉：%lu \n", cnt, s.size(), s2.size(), s2.size() - s.size());
        }
        else if (cnt <= 10000000)
        {
            if (cnt % 1000000 == 0)
                printf("前%lu个包: %lu / %lu 过滤掉：%lu \n", cnt, s.size(), s2.size(), s2.size() - s.size());
        }
        else
        {
            if (cnt % 10000000 == 0)
                printf("前%lu个包: %lu / %lu 过滤掉：%lu \n", cnt, s.size(), s2.size(), s2.size() - s.size());
        }
    }
}