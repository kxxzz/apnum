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




APNUM_int APNUM_intDup(const APNUM_int* x)
{
    APNUM_int a = { 0 };
    vec_dup(&a.data, &x->data);
    a.neg = x->neg;
    return a;
}





bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str)
{
    // todo
    assert(10 == base);
    APNUM_int x = { 0 };
    u32 len = (u32)strlen(str);
    if ((1 == len) && ('0' == str[0]))
    {
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
    if (0 == x->data.length)
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









static int APNUM_absCmpInt(const u8* a, u32 na, const u8* b, u32 nb)
{
    if (na > nb)
    {
        return 1;
    }
    else if (na < nb)
    {
        return -1;
    }
    else
    {
        assert(na == nb);
        for (u32 i = 0; i < na; ++i)
        {
            u32 j = na - 1 - i;
            s8 ea = a[j];
            s8 eb = b[j];
            if (ea > eb)
            {
                return 1;
            }
            else if (ea < eb)
            {
                return -1;
            }
        }
    }
    return 0;
}



static int APNUM_intCmpAbs(const APNUM_int* a, const APNUM_int* b)
{
    return APNUM_absCmpInt(a->data.data, a->data.length, b->data.data, b->data.length);
}






void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_int r = { 0 };
    u32 len = max(a->data.length, b->data.length);
    bool outNeg = false;
    if (a->neg && b->neg)
    {
        outNeg = true;
    }
    else if (a->neg || b->neg)
    {
        outNeg = APNUM_intCmpAbs(a, b) < 0;
    }

    vec_resize(&r.data, len);
    s8 carry = 0;
    for (u32 i = 0; i < len; ++i)
    {
        s8 ea = (i < a->data.length) ? a->data.data[i] : 0;
        s8 eb = (i < b->data.length) ? b->data.data[i] : 0;
        ea = a->neg ? -ea : ea;
        eb = b->neg ? -eb : eb;
        ea = outNeg ? -ea : ea;
        eb = outNeg ? -eb : eb;
        s8 ec = ea + eb + carry;

        if (ec < 0)
        {
            r.data.data[i] = ec + 10;
            carry = -1;
        }
        else if (ec >= 10)
        {
            r.data.data[i] = ec - 10;
            carry = 1;
        }
        else
        {
            r.data.data[i] = ec;
            carry = 0;
        }
    }
    assert(carry >= 0);
    if (carry > 0)
    {
        vec_push(&r.data, 1);
    }
    while (r.data.length && (vec_last(&r.data) == 0))
    {
        vec_pop(&r.data);
    }
    if (outNeg)
    {
        r.neg = true;
    }
    *out = r;
}





void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_int negb;
    negb.data = b->data;
    negb.neg = !b->neg;
    APNUM_intAdd(out, a, &negb);
}





void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_int sum = { 0 };
    if (APNUM_intCmpAbs(a, b) < 0)
    {
        const APNUM_int* t = a;
        a = b;
        b = t;
    }
    APNUM_int ea = APNUM_intDup(a);
    for (u32 i = 0; i < b->data.length; ++i)
    {
        u32 n = b->data.data[i];
        for (u32 i = 0; i < n; ++i)
        {
            APNUM_intAdd(&sum, &sum, &ea);
        }
        vec_insert(&ea.data, 0, 0);
    }
    APNUM_intFree(&ea);
    sum.neg = a->neg ? !b->neg : b->neg;
    *out = sum;
}





void APNUM_intDiv(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_int q = { 0 };
    APNUM_int r = { 0 };
    APNUM_int eb = { b->data };
    int rel = APNUM_intCmpAbs(a, b);
    if (0 == rel)
    {
        vec_push(&q.data, 1);
        goto out;
    }
    if (rel < 0)
    {
        r = APNUM_intDup(a);
        goto out;
    }
    assert(b->data.length > 0);
    for (u32 i = 0; i < a->data.length; ++i)
    {
        u32 j = a->data.length - 1 - i;
        vec_insert(&r.data, 0, a->data.data[j]);
        u32 er = 0;
        for (;; ++er)
        {
            rel = APNUM_intCmpAbs(&r, b);
            if (rel < 0)
            {
                break;
            }
            APNUM_intSub(&r, &r, &eb);
        }
        if ((q.data.length > 0) || er)
        {
            vec_insert(&q.data, 0, er);
        }
    }
out:
    q.neg = a->neg ? !b->neg : b->neg;
    r.neg = a->neg;
    *outQ = q;
    *outR = r;
}










int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b)
{
    if (!a->neg && b->neg)
    {
        return 1;
    }
    else if (a->neg && !b->neg)
    {
        return -1;
    }
    int r = 1;
    if (a->neg && b->neg)
    {
        r = -1;
    }
    return APNUM_intCmpAbs(a, b) * r;
}













































































