// 跟踪一个流
// 跟踪它所在的TS bucket和Sketch bucket的变化情况
// 每当有一个包访问对应TS或者对应Sketch bucket时 打印信息
// 打印信息：流ID 包总序号 当前包数/总包数 正确输出 实际输出 是否正确
// TS：res2和res4的变化
// bucket：插入值 插入结束后的桶的情况 实际还应该追踪一下bucket中的cell到底是哪个flow填的（因为bucket只记seq不记flow）

// 待完成

#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <x86intrin.h>
#include "includes/Packet.h"
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/Break_Sketch.h"
#include "includes/BOBHash.h"
#include "includes/params.h"

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

using namespace std;

vector<uint32_t> from, to;
map<uint32_t, FlowInfo> Flowinfo;

class SIMD_Bucket_4_32_trace // 128位
{
public:
    uint32_t seq[4] = {0};
    uint32_t id[4] = {0};

    int Insert(uint32_t seq_, uint32_t id_, bool b)
    {
        for (int matchplace = 0; matchplace < 4; matchplace++)
        {
            if (1 <= seq_ - seq[matchplace] && seq_ - seq[matchplace] <= 3)
            {
                for (int i = matchplace; i > 0; i--)
                    seq[i] = seq[i - 1], id[i] = id[i - 1];
                seq[0] = seq_, id[0] = id_;
                return 0;
            }
        }
        for (int matchplace = 0; matchplace < 4; matchplace++)
        {
            if (4 <= seq_ - seq[matchplace] && seq_ - seq[matchplace] < 50)
            {
                for (int i = matchplace; i > 0; i--)
                    seq[i] = seq[i - 1], id[i] = id[i - 1];
                seq[0] = seq_, id[0] = id_;
                return 1;
            }
        }
        if (id[3])
        {
            from.push_back(Flowinfo[id_].totpkt);
            to.push_back(Flowinfo[id[3]].totpkt);
        }
        for (int i = 3; i > 0; i--)
            seq[i] = seq[i - 1], id[i] = id[i - 1];
        seq[0] = seq_, id[0] = id_;
        return 2;
    }
    void Print()
    {
        printf("seq: %11u,%11u,%11u,%11u\n", seq[0], seq[1], seq[2], seq[3]);
        printf("id : %11u,%11u,%11u,%11u\n", id[0], id[1], id[2], id[3]);
    }
};
class Tower_Sketch_CU_trace
{
public:
    uint32_t w_2, w_4;
    unsigned char *table_2;
    unsigned char *table_4;
    BOBHash *hash_2, *hash_4;
    unsigned char mask_2[4] = {0b00000011, 0b00001100, 0b00110000, 0b11000000};
    unsigned char delt_2[4] = {0b00000001, 0b00000100, 0b00010000, 0b01000000};
    unsigned char mask_4[2] = {0b00001111, 0b11110000};
    unsigned char delt_4[2] = {0b00000001, 0b00010000};

public:
    Tower_Sketch_CU_trace(int TSmemory, int hash_seed = 1000)
    {
        w_2 = TSmemory * 2;
        w_4 = TSmemory;
        table_2 = new unsigned char[TSmemory / 2];
        table_4 = new unsigned char[TSmemory / 2];
        memset(table_2, 0, TSmemory / 2);
        memset(table_4, 0, TSmemory / 2);

        hash_2 = new BOBHash(hash_seed + 2);
        hash_4 = new BOBHash(hash_seed + 4);
    }

    void Insert(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;

        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m_2 = index_2 / 4, n_2 = index_2 % 4;
        res_2 = (table_2[m_2] & mask_2[n_2]) / (1 << (n_2 * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        if (res_2 == 3 && res_4 == 15)
            ;
        else if ((res_2 == 3 && res_4 != 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 > res_4))
            table_4[m_4] += delt_4[n_4];
        else if ((res_2 != 3 && res_4 == 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 < res_4))
            table_2[m_2] += delt_2[n_2];
        else
        {
            table_4[m_4] += delt_4[n_4];
            table_2[m_2] += delt_2[n_2];
        }

        return;
    }

    unsigned int Query(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;
        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m = index_2 / 4, n = index_2 % 4;
        res_2 = (table_2[m] & mask_2[n]) / (1 << (n * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        m = index_4 / 2, n = index_4 % 2;
        res_4 = (table_4[m] & mask_4[n]) / (1 << (n * 4));

        if (res_2 == 3)
            return res_4;
        else
            return res_2 < res_4 ? res_2 : res_4;
    }

    unsigned int InsertAndQuery(const char *str)
    {
        uint32_t res_2 = 0, res_4 = 0;

        uint32_t index_2 = (hash_2->run(str, KEY_LEN)) % w_2;
        uint32_t m_2 = index_2 / 4, n_2 = index_2 % 4;
        res_2 = (table_2[m_2] & mask_2[n_2]) / (1 << (n_2 * 2));

        uint32_t index_4 = (hash_4->run(str, KEY_LEN)) % w_4;
        uint32_t m_4 = index_4 / 2, n_4 = index_4 % 2;
        res_4 = (table_4[m_4] & mask_4[n_4]) / (1 << (n_4 * 4));

        if (res_2 == 3 && res_4 == 15) // 都溢出
        {
            return 15;
        }
        else if ((res_2 == 3 && res_4 != 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 > res_4))
        {
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
        else if ((res_2 != 3 && res_4 == 15) ||
                 (res_2 != 3 && res_4 != 15 && res_2 < res_4))
        {
            table_2[m_2] += delt_2[n_2];
            /*if(res_2 == 2) return res_4;
            else return res_2 + 1;*/
            return res_2 + 1;
            /*注：此处存在特殊情况：即2bit整数为2，即将+1变成3(溢出状态)，
            如果按原towersketch，插入和查询分开进行，是无法区分刚刚达到溢出，还是早就溢出了，
            但插入和查询一起做可以得到，所以函数InsertAndQuery 与 Insert + Query的结果有几处不同，
            在全caida数据集上共有3000多处不同：仅发生在2bit数将加到3，而4bit数已经超过3，
            此时感觉查询结果为3更合理（等待后续实验）
            */
        }
        else
        {
            table_2[m_2] += delt_2[n_2];
            table_4[m_4] += delt_4[n_4];
            return res_4 + 1;
        }
    }

    ~Tower_Sketch_CU_trace()
    {
        delete table_2;
        delete table_4;
        delete hash_2;
        delete hash_4;
    }
    pair<uint32_t, uint32_t> getindex(uint32_t id)
    {
        uint32_t index_2 = (hash_2->run((char *)&id, sizeof(id))) % w_2;
        uint32_t index_4 = (hash_4->run((char *)&id, sizeof(id))) % w_4;
        return make_pair(index_2, index_4);
    }
};
class Break_Sketch_improved_trace : public Break_Sketch
{
public:
    SIMD_Bucket_4_32_trace *bucket;
    int size;
    BOBHash *hash;
    Tower_Sketch_CU_trace *TS;

public:
    Break_Sketch_improved_trace(int memory, int TSmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory - TSmemory) / 16;
        bucket = new SIMD_Bucket_4_32_trace[size];
        hash = new BOBHash(hash_seed);
        TS = new Tower_Sketch_CU_trace(TSmemory);
    }

    bool Solution(const Packet &packet, bool b) // 返回是否发生断流
    {
        TS->Insert((char *)&packet.id);
        unsigned int num = TS->Query((char *)&packet.id);
        // unsigned int num = TS->InsertAndQuery((char *)&packet.id);

        if (num <= 14)
            return false;
        else
        {
            int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
            int bucket_res = bucket[index].Insert(packet.seq, packet.id, b);
#ifdef JUDGEMENT
            return bucket_res != 0;
#else
            return bucket_res == 1;
#endif
        }
    }

    string Name()
    {
        return string("Break_Sketch_improved_trace");
    }
    uint32_t getindex(uint32_t id)
    {
        int index = hash->run((char *)&id, sizeof(id)) % size;
        return index;
    }
    ~Break_Sketch_improved_trace()
    {
        delete hash;
        delete TS;
        delete[] bucket;
    }
};

static uint32_t __attribute__((aligned(64))) num0_32[16] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint32_t __attribute__((aligned(64))) num4_32[16] =
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
static uint32_t __attribute__((aligned(64))) num50_32[16] =
    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static uint16_t __attribute__((aligned(64))) num0_16[32] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t __attribute__((aligned(64))) num4_16[32] =
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
static uint16_t __attribute__((aligned(64))) num50_16[32] =
    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};

void Print_(__m128i x)
{
    uint32_t *p = (uint32_t *)&x;
    for (int i = 0; i < 4; i++)
        cout << *p++ << ' ';
    cout << endl;
    return;
}

// map<uint32_t, uint32_t> FlowCnt;
vector<Packet> dataset;
GlobalInfo globalinfo;
vector<char> standard;

int main(int argc, char *argv[])
{
    globalinfo.load();
    LoadFlowInfo(Flowinfo);
    Load(dataset);
    LoadStandard(standard);
    for (int no = 1; no <= 128; no*=2)
    {
        char buf[8];
        from.clear();
        to.clear();
        sprintf(buf, "%d", no);
        string filename = string("../result/traceinfo/trace") + buf + ".txt";

        Break_Sketch_improved_trace *Sketch = new Break_Sketch_improved_trace(512 * no + 4096 * no, 512 * no);

        FILE *fp = fopen(filename.c_str(), "w");
        for (int i = 0; i < globalinfo.TotPacketCount; i++)
        {
            int res = Sketch->Solution(dataset[i], standard[i] == 'b');
        }
        for (uint32_t num : from)
            fprintf(fp, "%u ", num);
        fprintf(fp, "\n");
        for (uint32_t num : to)
            fprintf(fp, "%u ", num);
        fclose(fp);
        delete Sketch;
    }
}