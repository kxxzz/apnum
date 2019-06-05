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




#define zalloc(sz) calloc(1, sz)







typedef u32 APNUM_Digit;
typedef s64 APNUM_Wigit;
typedef vec_t(APNUM_Digit) APNUM_DigitVec;


typedef struct APNUM_int
{
    APNUM_DigitVec digits;
    bool neg;
} APNUM_int;






void APNUM_intFree(APNUM_int* x)
{
    vec_free(&x->digits);
    free(x);
}




void APNUM_intDup(APNUM_int* out, const APNUM_int* x)
{
    vec_dup(&out->digits, &x->digits);
    out->neg = x->neg;
}






APNUM_int* APNUM_intZero(void)
{
    APNUM_int* a = zalloc(sizeof(*a));
    return a;
}





bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str)
{
    APNUM_int* x = out;
    vec_resize(&x->digits, 0);
    // todo
    assert(10 == base);
    u32 len = (u32)strlen(str);
    if ((1 == len) && ('0' == str[0]))
    {
        x->neg = false;
        return true;
    }
    bool neg = '-' == str[0];
    u32 sp = (('-' == str[0]) || ('+' == str[0])) ? 1 : 0;
    for (u32 i = sp; i < len; ++i)
    {
        if ((str[i] < '0') || (str[i] > '9'))
        {
            return false;
        }
    }
    x->neg = neg;
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
    APNUM_int* e = APNUM_intZero();
    APNUM_int* x1 = APNUM_intZero();

    e->neg = x->neg;
    vec_resize(&e->digits, 0);
    vec_push(&e->digits, str[sp] - '0');
    APNUM_intAddInP(x, e);

    for (u32 i = sp + 1; i < len; ++i)
    {
        e->neg = false;
        vec_resize(&e->digits, 0);
        vec_push(&e->digits, 10);
        APNUM_intMul(x1, x, e);
        APNUM_int t = *x1;
        *x1 = *x;
        *x = t;

        e->neg = x->neg;
        vec_resize(&e->digits, 0);
        vec_push(&e->digits, str[i] - '0');
        APNUM_intAddInP(x, e);
    }
    APNUM_intFree(x1);
    APNUM_intFree(e);
    return true;
}







u32 APNUM_intToStr(const APNUM_int* x, u32 base, char* strBuf, u32 strBufSize)
{
    // todo
    assert(10 == base);
    if (0 == x->digits.length)
    {
        strBuf[0] = '0';
        strBuf[1] = 0;
        return 1;
    }
    u32 sp = x->neg ? 1 : 0;
    u32 len = x->digits.length + sp;
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
        u32 j = x->digits.length - 1 - (i - sp);
        strBuf[i] = x->digits.data[j] + '0';
    }
    strBuf[len] = 0;
    return len;
}













static int APNUM_absCmpInt(const APNUM_Digit* a, u32 na, const APNUM_Digit* b, u32 nb)
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
            APNUM_Digit ea = a[j];
            APNUM_Digit eb = b[j];
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
    return APNUM_absCmpInt(a->digits.data, a->digits.length, b->digits.data, b->digits.length);
}













void APNUM_intAddInP(APNUM_int* a, const APNUM_int* b)
{
    u32 alen = a->digits.length;
    u32 blen = b->digits.length;
    u32 len = max(alen, blen);
    bool outNeg = false;
    if (a->neg && b->neg)
    {
        outNeg = true;
    }
    else if (a->neg || b->neg)
    {
        outNeg = APNUM_intCmpAbs(a, b) < 0;
    }
    vec_resize(&a->digits, len);
    APNUM_Wigit ec = 0;
    for (u32 i = 0; i < len; ++i)
    {
        APNUM_Wigit ea = (i < alen) ? a->digits.data[i] : 0;
        APNUM_Wigit eb = (i < blen) ? b->digits.data[i] : 0;
        ea = a->neg ? (outNeg ? ea : -ea) : (outNeg ? -ea : ea);
        eb = b->neg ? (outNeg ? eb : -eb) : (outNeg ? -eb : eb);
        ec = ec + ea + eb;

        if (ec < 0)
        {
            ec += 10;
            a->digits.data[i] = (APNUM_Digit)ec;
            ec = -1;
        }
        else if (ec >= 10)
        {
            ec -= 10;
            a->digits.data[i] = (APNUM_Digit)ec;
            ec = 1;
        }
        else
        {
            a->digits.data[i] = (APNUM_Digit)ec;
            ec = 0;
        }
    }
    assert(ec >= 0);
    if (ec > 0)
    {
        vec_push(&a->digits, 1);
    }
    while (a->digits.length && (vec_last(&a->digits) == 0))
    {
        vec_pop(&a->digits);
    }
    if (outNeg)
    {
        a->neg = true;
    }
}


void APNUM_intSubInP(APNUM_int* a, const APNUM_int* b)
{
    APNUM_int t = { 0 };
    APNUM_int* negb = &t;
    negb->digits = b->digits;
    negb->neg = !b->neg;
    APNUM_intAddInP(a, negb);
}













void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intAddInP(out, b);
}


void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intSubInP(out, b);
}











void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_int* sum = out;
    vec_resize(&sum->digits, 0);
    if (APNUM_intCmpAbs(a, b) < 0)
    {
        const APNUM_int* t = a;
        a = b;
        b = t;
    }
    APNUM_int* ea = APNUM_intZero();
    APNUM_intDup(ea, a);
    for (u32 i = 0; i < b->digits.length; ++i)
    {
        u32 n = b->digits.data[i];
        for (u32 i = 0; i < n; ++i)
        {
            APNUM_intAddInP(sum, ea);
        }
        vec_insert(&ea->digits, 0, 0);
    }
    APNUM_intFree(ea);
    sum->neg = a->neg ? !b->neg : b->neg;
}











void APNUM_intDiv(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* a, const APNUM_int* b)
{
    assert(outQ != a);
    assert(outQ != b);
    assert(outR != a);
    assert(outR != b);
    APNUM_int* q = outQ;
    APNUM_int* r = outR;
    vec_resize(&q->digits, 0);
    vec_resize(&r->digits, 0);
    APNUM_int eb = { b->digits };
    int rel = APNUM_intCmpAbs(a, b);
    if (0 == rel)
    {
        vec_push(&q->digits, 1);
        goto out;
    }
    if (rel < 0)
    {
        APNUM_intDup(r, a);
        goto out;
    }
    assert(b->digits.length > 0);
    for (u32 i = 0; i < a->digits.length; ++i)
    {
        u32 j = a->digits.length - 1 - i;
        vec_insert(&r->digits, 0, a->digits.data[j]);
        u32 er = 0;
        for (;; ++er)
        {
            rel = APNUM_intCmpAbs(r, b);
            if (rel < 0)
            {
                break;
            }
            APNUM_intSubInP(r, &eb);
        }
        if ((q->digits.length > 0) || er)
        {
            vec_insert(&q->digits, 0, er);
        }
    }
out:
    q->neg = a->neg ? !b->neg : b->neg;
    r->neg = a->neg;
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













































































