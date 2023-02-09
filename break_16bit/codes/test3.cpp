#include <bits/stdc++.h>
#include "includes/simd.h"
#include <x86intrin.h>
using namespace std;

int main()
{
    SIMD_Bucket_8_8 object;
    uint16_t num;
    while (cin>>num)
    {
        object.Insert(num);
        object.Print();
    }
    return 0;
}