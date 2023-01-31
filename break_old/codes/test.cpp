#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "includes/TestAcc.h"
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/Break_Sketch_straw.h"
#include "includes/Break_Sketch_basic.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
#include "includes/Count_Break.h"

using namespace std;

vector<Packet> dataset;

int main()
{
    Load(dataset);
    Break_Sketch *break_sketch = new Count_Break();
    //GetOutput(break_sketch, dataset, "count");
    Acc acc = CompareOutput("count");
    acc.Print();
    cout << acc.FN << acc.FP;
}