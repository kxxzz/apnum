#include "apnum.h"
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <assert.h>



#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))



#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))





void APNUM_intFree(APNUM_int* x)
{
    vec_free(&x->data);
}



bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str)
{
    // todo
    assert(10 == base);
    APNUM_int x = { 0 };
    u32 len = (u32)strlen(str);
    if ((1 == len) && ('0' == str[0]))
    {
        vec_push(&x.data, 0);
        *out = x;
        return true;
    }
    x.neg = '-' == str[0];
    u32 sp = (('-' == str[0]) || ('+' == str[0])) ? 1 : 0;
    for (u32 i = sp; i < len; ++i)
    {
        if ((str[i] < '0') || (str[i] > '9'))
        {
            return false;
        }
    }
    for (u32 i = sp; i < len; ++i)
    {
        if ('0' == str[i])
        {
            ++sp;
        }
        else
        {
            break;
        }
    }
    u32 dataSize = len - sp;
    vec_resize(&x.data, dataSize);
    for (u32 i = sp; i < len; ++i)
    {
        u32 j = dataSize - 1 - (i - sp);
        x.data.data[j] = str[i] - '0';
    }
    *out = x;
    return true;
}




u32 APNUM_intToStr(const APNUM_int* x, u32 base, char* strBuf, u32 strBufSize)
{
    // todo
    assert(10 == base);
    if ((1 == x->data.length) && (0 == x->data.data[0]))
    {
        strBuf[0] = '0';
        strBuf[1] = 0;
        return 1;
    }
    u32 sp = x->neg ? 1 : 0;
    u32 len = x->data.length + sp;
    if (strBufSize <= len)
    {
        return len;
    }
    if (x->neg)
    {
        strBuf[0] = '-';
    }
    for (u32 i = sp; i < len; ++i)
    {
        u32 j = x->data.length - 1 - (i - sp);
        strBuf[i] = x->data.data[j] + '0';
    }
    strBuf[len] = 0;
    return len;
}









void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    u32 len = max(a->data.length, b->data.length);
    vec_resize(&out->data, len);
    s8 carry = 0;
    for (u32 i = 0; i < len; ++i)
    {
        s8 ea = (i < a->data.length) ? a->data.data[i] : 0;
        s8 eb = (i < b->data.length) ? b->data.data[i] : 0;
        ea = a->neg ? -ea : ea;
        eb = b->neg ? -eb : eb;
        s8 ec = ea + eb + carry;

        if (ec < 0)
        {
            out->data.data[i] = ec + 10;
            carry = -1;
        }
        else if (ec >= 10)
        {
            out->data.data[i] = ec - 10;
            carry = 1;
        }
        else
        {
            out->data.data[i] = ec;
            carry = 0;
        }
    }
    assert(carry >= 0);
    if (carry > 0)
    {
        vec_push(&out->data, carry);
    }
    if (vec_last(&out->data) == 0)
    {
        vec_pop(&out->data);
    }
}



void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}



void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}



void APNUM_intDiv(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}










int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b)
{
    return 0;
}













































































