#ifndef _SIMD_H
#define _SIMD_H

#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;

static uint16_t __attribute__((aligned(64))) num0_16[32] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t __attribute__((aligned(64))) num4_16[32] =
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
static uint16_t __attribute__((aligned(64))) num50_16[32] =
    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
static uint8_t __attribute__((aligned(64))) num0_8[64] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t __attribute__((aligned(64))) num4_8[64] =
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
     4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
static uint8_t __attribute__((aligned(64))) num50_8[64] =
    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
     50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};

static long long shuffle_8_16[16] =
    {0x0706050403020100, 0x0f0e0d0c0b0a0908, 0x0706050401000302, 0x0f0e0d0c0b0a0908,
     0x0706030201000504, 0x0f0e0d0c0b0a0908, 0x0504030201000706, 0x0f0e0d0c0b0a0908,
     0x0504030201000908, 0x0f0e0d0c0b0a0706, 0x0504030201000b0a, 0x0f0e0d0c09080706,
     0x0504030201000d0c, 0x0f0e0b0a09080706, 0x0504030201000f0e, 0x0d0c0b0a09080706};

static long long shuffle_8_8[8] =
    {0x0706050403020100, 0x0706050403020001, 0x0706050403010002, 0x0706050402010003,
     0x0706050302010004, 0x0706040302010005, 0x0705040302010006, 0x0605040302010007};

static long long shuffle_8_8_new[8] =
    {0x1007060504030201, 0x1107060504030200, 0x1207060504030100, 0x1307060504020100,
     0x1407060503020100, 0x1507060403020100, 0x1607050403020100, 0x1706050403020100};

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

class SIMD_Bucket_2_16
{
public:
    uint16_t num[2];
    SIMD_Bucket_2_16()
    {
        *(int *)num = 0;
    }

    SIMD_Bucket_2_16(void *a)
    {
        *(int *)num = *(int *)a;
    }

    int Insert(uint16_t visitnum)
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

class SIMD_Bucket_4_16
{
public:
    __m64 m64_num;

    SIMD_Bucket_4_16()
    {
        memset(&m64_num, 0, sizeof(__m64));
    }

    SIMD_Bucket_4_16(void *a)
    {
        memcpy(&m64_num, a, sizeof(__m64));
    }

    int Insert(uint16_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m64 m64_visitnum = (__m64)_mm_set1_pi16(visitnum);
        __m64 m64_diff = _mm_subs_pi16(m64_visitnum, m64_num);

        __m64 m64_diff0res = _mm_cmpgt_pi16(m64_diff, *(__m64 *)num0_16);
        __m64 m64_diff3res = _mm_cmpgt_pi16(*(__m64 *)num4_16, m64_diff);
        m64_diff3res = _mm_and_si64(m64_diff3res, m64_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff3res));
        if (tailzero == 32)
        {
            __m64 m64_diff50res = _mm_cmpgt_pi16(*(__m64 *)num50_16, m64_diff);
            m64_diff50res = _mm_and_si64(m64_diff50res, m64_diff0res);

            tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff50res));
        }
        matchplace = tailzero / 2;

        res = matchplace < 4 ? visitnum - ((uint16_t *)&m64_num)[matchplace] >=4 : 2;
        switch (matchplace)
        {
        case 0:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11100100);
            break;
        case 1:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11100001);
            break;
        case 2:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11010010);
            break;
        case 3:
            m64_num = _mm_shuffle_pi16(m64_num, 0b10010011);
            break;
        default:
            m64_num = _mm_shuffle_pi16(m64_num, 0b10010011);
            break;
        }
        *(uint16_t *)&m64_num = visitnum;
        return res;
    }

    int MatchAndInsert(uint16_t visitnum, uint16_t &kickednum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m64 m64_visitnum = (__m64)_mm_set1_pi16(visitnum);
        __m64 m64_diff = _mm_subs_pi16(m64_visitnum, m64_num);

        __m64 m64_diff0res = _mm_cmpgt_pi16(m64_diff, *(__m64 *)num0_16);
        __m64 m64_diff3res = _mm_cmpgt_pi16(*(__m64 *)num4_16, m64_diff);
        m64_diff3res = _mm_and_si64(m64_diff3res, m64_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff3res));
        if (tailzero == 32)
        {
            __m64 m64_diff50res = _mm_cmpgt_pi16(*(__m64 *)num50_16, m64_diff);
            m64_diff50res = _mm_and_si64(m64_diff50res, m64_diff0res);

            tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff50res));
        }
        matchplace = tailzero / 2;

        res = matchplace < 4 ? visitnum - ((uint16_t *)&m64_num)[matchplace] >=4 : 2;
        switch (matchplace)
        {
        case 0:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11100100);
            break;
        case 1:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11100001);
            break;
        case 2:
            m64_num = _mm_shuffle_pi16(m64_num, 0b11010010);
            break;
        case 3:
            m64_num = _mm_shuffle_pi16(m64_num, 0b10010011);
            break;
        default:
            m64_num = _mm_shuffle_pi16(m64_num, 0b10010011);
            break;
        }
        kickednum = *(uint16_t *)&m64_num;
        *(uint16_t *)&m64_num = visitnum;
        return res;
    }


    void Print()
    {
        uint16_t *p = (uint16_t *)&m64_num;
        for (int i = 0; i < 4; i++)
            cout << *p++ << ' ';
        cout << endl;
        return;
    }
};

class SIMD_Bucket_8_16
{
public:
    __m128i m128i_num;

    SIMD_Bucket_8_16()
    {
        memset(&m128i_num, 0, sizeof(__m128i));
    }

    SIMD_Bucket_8_16(void *a)
    {
        memcpy(&m128i_num, a, sizeof(__m128i));
    }

    int Insert(uint16_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m128i m128i_visitnum;
        m128i_visitnum = (__m128i)_mm_set1_epi16(visitnum);
        __m128i m128i_diff = _mm_sub_epi16(m128i_visitnum, m128i_num);
        __m128i m128i_diff0res = _mm_cmpgt_epi16(m128i_diff, *(__m128i *)num0_16);
        __m128i m128i_diff3res = _mm_cmpgt_epi16(*(__m128i *)num4_16, m128i_diff);
        m128i_diff3res = _mm_and_si128(m128i_diff3res, m128i_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_epi8(m128i_diff3res));

        if (tailzero == 32)
        {
            __m128i m128i_diff50res = _mm_cmpgt_epi16(*(__m128i *)num50_16, m128i_diff);
            m128i_diff50res = _mm_and_si128(m128i_diff50res, m128i_diff0res);
            tailzero = _tzcnt_u32(_mm_movemask_epi8(m128i_diff50res));
        }

        matchplace = tailzero / 2;
        res = matchplace < 8 ? (visitnum - ((uint16_t *)&m128i_num)[matchplace] >=4) : 2;
        m128i_num = _mm_shuffle_epi8(m128i_num, 
            *((__m128i*)&shuffle_8_16[matchplace < 8 ? matchplace * 2 : 14]));
        /*
        switch (matchplace)
        {
        case 0:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c0b0a09080706050403020100);
            break;
        case 1:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c0b0a09080706050401000302);
            break;
        case 2:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c0b0a09080706030201000504);
            break;
        case 3:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c0b0a09080504030201000706);
            break;
        case 4:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c0b0a07060504030201000908);
            break;
        case 5:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0d0c090807060504030201000b0a);
            break;
        case 6:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0f0e0b0a090807060504030201000d0c);
            break;
        case 7:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0d0c0b0a090807060504030201000f0e);
            break;
        default:
            m128i_num = _mm_shuffle_epi8(m128i_num, (__m128i)0x0d0c0b0a090807060504030201000f0e);
        }
        */
        *(uint16_t *)&m128i_num = visitnum;
        return res;
    }

    void Print()
    {
        uint16_t *p = (uint16_t *)&m128i_num;
        for (int i = 0; i < 8; i++)
            cout << *p++ << ' ';
        cout << endl;
        return;
    }
};


class SIMD_Bucket_8_8 // 128位
{
public:
    __m64 m64_num;

    SIMD_Bucket_8_8()
    {
        memset(&m64_num, 0, sizeof(__m64));
    }

    SIMD_Bucket_8_8(void *a)
    {
        memcpy(&m64_num, a, sizeof(__m64));
    }

    int Insert(uint8_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m64 m64_visitnum = (__m64)_mm_set1_pi8((char)visitnum);
        __m64 m64_diff = _mm_subs_pi8(m64_visitnum, m64_num);

        __m64 m64_diff0res = _mm_cmpgt_pi8(m64_diff, *(__m64 *)num0_8);
        __m64 m64_diff3res = _mm_cmpgt_pi8(*(__m64 *)num4_8, m64_diff);
        m64_diff3res = _mm_and_si64(m64_diff3res, m64_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff3res));
        if (tailzero == 32)
        {
            __m64 m64_diff50res = _mm_cmpgt_pi8(*(__m64 *)num50_8, m64_diff);
            m64_diff50res = _mm_and_si64(m64_diff50res, m64_diff0res);

            tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff50res));
        }
        matchplace = tailzero;

        res = matchplace < 8 ? visitnum - ((uint8_t *)&m64_num)[matchplace] >=4 : 2;
        m64_num = _mm_shuffle_pi8(m64_num, 
            *((__m64*)&shuffle_8_8[matchplace < 8 ? matchplace : 7]));

        *(uint8_t *)&m64_num = visitnum;
        return res;
    }

    int MatchAndEmpty(uint8_t visitnum)
    {
        int tailzero;
        int matchplace;
        int res = 0;
        __m64 m64_visitnum = (__m64)_mm_set1_pi8((char)visitnum);
        __m64 m64_diff = _mm_subs_pi8(m64_visitnum, m64_num);

        __m64 m64_diff0res = _mm_cmpgt_pi8(m64_diff, *(__m64 *)num0_8);
        __m64 m64_diff3res = _mm_cmpgt_pi8(*(__m64 *)num4_8, m64_diff);
        m64_diff3res = _mm_and_si64(m64_diff3res, m64_diff0res);
        tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff3res));
        if (tailzero == 32)
        {
            __m64 m64_diff50res = _mm_cmpgt_pi8(*(__m64 *)num50_8, m64_diff);
            m64_diff50res = _mm_and_si64(m64_diff50res, m64_diff0res);

            tailzero = _tzcnt_u32(_mm_movemask_pi8(m64_diff50res));
        }
        matchplace = tailzero;

        res = matchplace < 8 ? visitnum - ((uint8_t *)&m64_num)[matchplace] >=4 : 2;
        if(res < 2) //match
        {
            m64_num = _mm_shuffle_pi8(m64_num, 
                *((__m64*)&shuffle_8_8_new[matchplace]));
        }
        return res;
    }


    void Print()
    {
        uint8_t *p = (uint8_t *)&m64_num;
        for (int i = 0; i < 8; i++)
            cout << (uint32_t)(*p++) << ' ';
        cout << endl;
        return;
    }
};


#endif