#ifndef _SIMD_H
#define _SIMD_H

#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

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

void Print_(__m64 x)
{
    uint32_t *p = (uint32_t *)&x;
    for (int i = 0; i < 4; i++)
        cout << *p++ << ' ';
    cout << endl;
    return;
}

void Print_(__m128i x)
{
    uint32_t *p = (uint32_t *)&x;
    for (int i = 0; i < 4; i++)
        cout << *p++ << ' ';
    cout << endl;
    return;
}

void Print_(__m256i x)
{
    uint32_t *p = (uint32_t *)&x;
    for (int i = 0; i < 8; i++)
        cout << *p++ << ' ';
    cout << endl;
    return;
}

class SIMD_Bucket_2_32 // 64位
{
public:
    uint32_t num[2];
    SIMD_Bucket_2_32()
    {
        *(long long *)num = 0;
    }

    SIMD_Bucket_2_32(void *a)
    {
        *(long long *)num = *(long long *)a;
    }

    int Insert(uint32_t visitnum)
    {
        if (visitnum - num[0] < 3)
        {
            num[0] = visitnum;
            return 0;
        }
        if (visitnum - num[1] < 3)
        {
            num[1] = num[0];
            num[0] = visitnum;
            return 0;
        }
        if (visitnum - num[0] < 50)
        {
            num[0] = visitnum;
            return 1;
        }
        if (visitnum - num[1] < 50)
        {
            num[1] = num[0];
            num[0] = visitnum;
            return 1;
        }
        num[1] = num[0];
        num[0] = visitnum;
        return 2;
    }

    void Print()
    {
        cout << num[0] << ' ' << num[1] << endl;
        return;
    }
};

class SIMD_Bucket_4_32 // 128位
{
public:
    __m128i m128i_num;

    SIMD_Bucket_4_32()
    {
        memset(&m128i_num, 0, sizeof(__m128i));
    }

    SIMD_Bucket_4_32(void *a)
    {
        memcpy(&m128i_num, a, sizeof(__m128i));
    }

    int Insert(uint32_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m128i m128i_visitnum = (__m128i)_mm_load1_ps((float *)&visitnum);
        __m128i m128i_diff = _mm_sub_epi32(m128i_visitnum, m128i_num);

        __m128i m128i_diff0res = _mm_cmplt_epi32(*(__m128i *)num0_32, m128i_diff);
        __m128i m128i_diff3res = _mm_cmpgt_epi32(*(__m128i *)num4_32, m128i_diff);
        m128i_diff3res = _mm_and_si128(m128i_diff3res, m128i_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_epi8(m128i_diff3res));
        if (tailzero == 32)
        {
            __m128i m128i_diff50res = _mm_cmpgt_epi32(*(__m128i *)num50_32, m128i_diff);
            m128i_diff50res = _mm_and_si128(m128i_diff50res, m128i_diff0res);

            tailzero = _tzcnt_u32(_mm_movemask_epi8(m128i_diff50res));
        }
        matchplace = tailzero / 4;

        res = matchplace < 4 ? visitnum - ((uint32_t *)&m128i_num)[matchplace] >=4 : 2;

        switch (matchplace)
        {
        case 0:
            m128i_num = _mm_shuffle_epi32(m128i_num, 0b11100100);
            break;
        case 1:
            m128i_num = _mm_shuffle_epi32(m128i_num, 0b11100001);
            break;
        case 2:
            m128i_num = _mm_shuffle_epi32(m128i_num, 0b11010010);
            break;
        case 3:
            m128i_num = _mm_shuffle_epi32(m128i_num, 0b10010011);
            break;
        default:
            m128i_num = _mm_shuffle_epi32(m128i_num, 0b10010011);
            break;
        }

        *(uint32_t *)&m128i_num = visitnum;
        return res;
    }

    void Print()
    {
        uint32_t *p = (uint32_t *)&m128i_num;
        for (int i = 0; i < 4; i++)
            cout << *p++ << ' ';
        cout << endl;
        return;
    }
};

class SIMD_Bucket_8_32 // 256位
{
public:
    __m256i m256i_num;

    SIMD_Bucket_8_32()
    {
        memset(&m256i_num, 0, sizeof(__m256i));
    }

    SIMD_Bucket_8_32(void *a)
    {
        memcpy(&m256i_num, a, sizeof(__m256i));
    }

    int Insert(uint32_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m256i m256i_visitnum;
        ((__m128i *)&m256i_visitnum)[0] = (__m128i)_mm_load1_ps((float *)&visitnum);
        ((__m128i *)&m256i_visitnum)[1] = (__m128i)_mm_load1_ps((float *)&visitnum);
        __m256i m256i_diff = _mm256_sub_epi32(m256i_visitnum, m256i_num);
        __m256i m256i_diff0res = _mm256_cmpgt_epi32(m256i_diff, *(__m256i *)num0_32);
        __m256i m256i_diff3res = _mm256_cmpgt_epi32(*(__m256i *)num4_32, m256i_diff);
        m256i_diff3res = _mm256_and_si256(m256i_diff3res, m256i_diff0res);
        tailzero = _tzcnt_u32(_mm256_movemask_epi8(m256i_diff3res));

        if (tailzero == 32)
        {

            __m256i m256i_diff50res = _mm256_cmpgt_epi32(*(__m256i *)num50_32, m256i_diff);
            m256i_diff50res = _mm256_and_si256(m256i_diff50res, m256i_diff0res);
            tailzero = _tzcnt_u32(_mm256_movemask_epi8(m256i_diff50res));
        }

        matchplace = tailzero / 4;
        res = matchplace < 8 ? (visitnum - ((uint32_t *)&m256i_num)[matchplace] >=4) : 2;
        // 效率低，可能需要改进
        __m128i *p1 = ((__m128i *)&m256i_num) + 1;
        switch (matchplace)
        {
        case 0:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b11100100);
            break;

        case 1:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b11100001);
            break;

        case 2:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b11010010);
            break;

        case 3:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            break;

        case 4:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            ((uint32_t *)&m256i_num)[4] = *((uint32_t *)&m256i_num);
            *p1 = _mm_shuffle_epi32(*p1, 0b11100100);
            break;

        case 5:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            ((uint32_t *)&m256i_num)[5] = *((uint32_t *)&m256i_num);
            *p1 = _mm_shuffle_epi32(*p1, 0b11100001);
            break;

        case 6:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            ((uint32_t *)&m256i_num)[6] = *((uint32_t *)&m256i_num);
            *p1 = _mm_shuffle_epi32(*p1, 0b11010010);
            break;

        case 7:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            ((uint32_t *)&m256i_num)[7] = *((uint32_t *)&m256i_num);
            *p1 = _mm_shuffle_epi32(*p1, 0b10010011);
            break;

        default:
            *((__m128i *)&m256i_num) = _mm_shuffle_epi32(*((__m128i *)&m256i_num), 0b10010011);
            ((uint32_t *)&m256i_num)[7] = *((uint32_t *)&m256i_num);
            *p1 = _mm_shuffle_epi32(*p1, 0b10010011);
        }
        *(uint32_t *)&m256i_num = visitnum;
        return res;
    }

    void Print()
    {
        uint32_t *p = (uint32_t *)&m256i_num;
        for (int i = 0; i < 8; i++)
            cout << *p++ << ' ';
        cout << endl;
        return;
    }
};

#endif