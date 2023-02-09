#include <bits/stdc++.h>
#include "includes/simd.h"
#include <x86intrin.h>
using namespace std;

int main()
{
    SIMD_Bucket_4_16 object;
    uint16_t num;
    while (cin >> num)
    {
        uint16_t out1, out2;
        out1 = object.MatchAndInsert(num, out2);
        object.Print();
        printf("%d,%d\n", out1, out2);
    }
    return 0;
}