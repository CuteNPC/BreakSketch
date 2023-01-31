/*
试验用
*/
#include <bits/stdc++.h>
#include "includes/BOBHash.h"
#include "includes/params.h"
#include "includes/Tower_Sketch_CU.h"
#include "includes/DataProcess.h"
#include <time.h>

using namespace std;

static int readnum = 20;  // 读取包总数
static int read_flow = 0; // 读取流总数

vector<pair<uint32_t, uint32_t>> input; //<流id，时间戳>，保证时间戳单调递增。
vector<pair<uint32_t, uint32_t>> gt;    // <包个数，流id>
set<uint32_t> flowSetMoreThan256;
vector<Packet> dataset;

int main()
{	
	input = loadCAIDA18(readnum);
    gt = groundtruth(input, read_flow, readnum);
    flowSetMoreThan256 = getFlowLargerThan256(gt);
    dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);

	unsigned int num1, num2,sum=0;
	clock_t start, finish;
	Tower_Sketch *TS1 = new Tower_Sketch(1000000); 
	Tower_Sketch *TS2 = new Tower_Sketch(1000000); 

	start = clock();
    for (Packet packet : dataset)
	{
    	num1 = TS1->InsertAndQuery((char *)&packet.id);
		TS2->Insert((char *)&packet.id);
    	num2 = TS2->Query((char *)&packet.id);
		if(num1!=num2){
			sum++;
		}
		cout<<packet.id<<" "<<packet.seq<<" "<<num1<<" "<<num2<<endl;
	}
	finish = clock();
	cout << ((double)finish - start) / CLOCKS_PER_SEC << endl;
	//cout<<sum<<endl;

	delete TS1;
	delete TS2;

	TS1 = new Tower_Sketch(1000000); 
	TS2 = new Tower_Sketch(1000000); 
	start = clock();
    for (Packet packet : dataset)
	{
		TS2->Insert((char *)&packet.id);
    	num2 = TS2->Query((char *)&packet.id);
	}
	finish = clock();
	cout << ((double)finish - start) / CLOCKS_PER_SEC << endl;

	
	start = clock();
    for (Packet packet : dataset)
	{
    	num1 = TS1->InsertAndQuery((char *)&packet.id);
	}
	finish = clock();
	cout << ((double)finish - start) / CLOCKS_PER_SEC << endl;
	
	delete TS1;
	delete TS2;

	return 0;
}