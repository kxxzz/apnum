#include "apnum_a.h"





typedef struct APNUM_rat
{
    APNUM_int* numerator;
    APNUM_int* denominator;
} APNUM_rat;

void APNUM_ratFreeMem(APNUM_rat* a)
{
    free(a);
}







APNUM_rat* APNUM_ratNew(APNUM_pool_t pool)
{
    if (pool->freeRationals->length > 0)
    {
        APNUM_rat* a = vec_last(pool->freeRationals);
        vec_pop(pool->freeRationals);

        a->numerator = APNUM_intNew(pool);
        a->denominator = APNUM_intNew(pool);
        APNUM_intFromS32(pool, a->numerator, 0);
        APNUM_intFromS32(pool, a->denominator, 1);
        return a;
    }
    else
    {
        APNUM_rat* a = zalloc(sizeof(*a));
        vec_push(pool->rationals, a);

        a->numerator = APNUM_intNew(pool);
        a->denominator = APNUM_intNew(pool);
        APNUM_intFromS32(pool, a->denominator, 1);
        return a;
    }
}

void APNUM_ratFree(APNUM_pool_t pool, APNUM_rat* a)
{
    APNUM_intFree(pool, a->numerator);
    APNUM_intFree(pool, a->denominator);
    vec_push(pool->freeRationals, a);
}









void APNUM_ratDup(APNUM_rat* out, const APNUM_rat* a)
{
    APNUM_intDup(out->numerator, a->numerator);
    APNUM_intDup(out->denominator, a->denominator);
}

void APNUM_ratAbs(APNUM_rat* a)
{
    APNUM_intAbs(a->numerator);
}

void APNUM_ratNeg(APNUM_rat* a)
{
    APNUM_intNeg(a->numerator);
}

void APNUM_ratNum(APNUM_int* out, const APNUM_rat* a)
{
    APNUM_intDup(out, a->numerator);
}

void APNUM_ratDom(APNUM_int* out, const APNUM_rat* a)
{
    APNUM_intDup(out, a->denominator);
}

bool APNUM_ratIsZero(const APNUM_rat* a)
{
    return APNUM_intIsZero(a->numerator);
}

bool APNUM_ratIsOne(const APNUM_rat* a)
{
    return APNUM_intIsOne(a->numerator) && APNUM_intIsOne(a->denominator);
}

bool APNUM_ratIsNeg(const APNUM_rat* a)
{
    return APNUM_intIsNeg(a->numerator);
}

bool APNUM_ratIsInt(const APNUM_rat* a)
{
    return APNUM_intIsOne(a->denominator);
}

bool APNUM_ratEq(const APNUM_rat* a, const APNUM_rat* b)
{
    return APNUM_intEq(a->numerator, b->numerator) && APNUM_intEq(a->denominator, b->denominator);
}








int APNUM_ratCmp(APNUM_pool_t pool, const APNUM_rat* a, const APNUM_rat* b)
{
    if (!APNUM_intIsNeg(a->numerator) && APNUM_intIsNeg(b->numerator))
    {
        return 1;
    }
    else if (APNUM_intIsNeg(a->numerator) && !APNUM_intIsNeg(b->numerator))
    {
        return -1;
    }
    else
    {
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_int* d = APNUM_intNew(pool);
        APNUM_intMul(pool, c, a->numerator, b->denominator);
        APNUM_intMul(pool, d, b->numerator, a->denominator);
        int r = APNUM_intCmp(c, d);
        APNUM_intFree(pool, c);
        APNUM_intFree(pool, d);
        return r;
    }
}















void APNUM_ratFromU32(APNUM_pool_t pool, APNUM_rat* out, u32 n, u32 d, bool neg)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromU32(pool, numerator, n, neg);
    APNUM_intFromU32(pool, denominator, d, false);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
}

void APNUM_ratFromU64(APNUM_pool_t pool, APNUM_rat* out, u64 n, u64 d, bool neg)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromU64(pool, numerator, n, neg);
    APNUM_intFromU64(pool, denominator, d, false);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
}

void APNUM_ratFromS32(APNUM_pool_t pool, APNUM_rat* out, s32 n, s32 d)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromS32(pool, numerator, n);
    APNUM_intFromS32(pool, denominator, d);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
}

void APNUM_ratFromS64(APNUM_pool_t pool, APNUM_rat* out, s64 n, s64 d)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromS64(pool, numerator, n);
    APNUM_intFromS64(pool, denominator, d);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
}











void APNUM_ratFromInt(APNUM_pool_t pool, APNUM_rat* out, const APNUM_int* n, const APNUM_int* d)
{
    APNUM_int* gcd = APNUM_intNew(pool);
    APNUM_int* r = APNUM_intNew(pool);
    APNUM_intGCD(pool, gcd, n, d);
    APNUM_intDiv(pool, out->numerator, r, n, gcd);
    assert(APNUM_intIsZero(r));
    APNUM_intDiv(pool, out->denominator, r, d, gcd);
    assert(APNUM_intIsZero(r));
    assert(!APNUM_intIsZero(out->denominator));
    APNUM_intSetNeg(out->numerator, APNUM_intIsNeg(n) ^ APNUM_intIsNeg(d));
    APNUM_intAbs(out->denominator);
    APNUM_intFree(pool, gcd);
    APNUM_intFree(pool, r);
}










bool APNUM_ratToU32(APNUM_pool_t pool, APNUM_rat* a, u32* out)
{
    if (APNUM_ratIsInt(a))
    {
        return APNUM_intToU32(pool, a->numerator, out);
    }
    else
    {
        return false;
    }
}

bool APNUM_ratToU64(APNUM_pool_t pool, APNUM_rat* a, u64* out)
{
    if (APNUM_ratIsInt(a))
    {
        return APNUM_intToU64(pool, a->numerator, out);
    }
    else
    {
        return false;
    }
}

bool APNUM_ratToS32(APNUM_pool_t pool, APNUM_rat* a, s32* out)
{
    if (APNUM_ratIsInt(a))
    {
        return APNUM_intToS32(pool, a->numerator, out);
    }
    else
    {
        return false;
    }
}

bool APNUM_ratToS64(APNUM_pool_t pool, APNUM_rat* a, s64* out)
{
    if (APNUM_ratIsInt(a))
    {
        return APNUM_intToS64(pool, a->numerator, out);
    }
    else
    {
        return false;
    }
}













bool APNUM_ratToF32(APNUM_pool_t pool, APNUM_rat* a, f32* out)
{
    f32 n, d;
    if (!APNUM_intToF32(pool, a->numerator, &n))
    {
        return false;
    }
    if (!APNUM_intToF32(pool, a->denominator, &d))
    {
        return false;
    }
    *out = n / d;
    return true;
}

bool APNUM_ratToF64(APNUM_pool_t pool, APNUM_rat* a, f64* out)
{
    f64 n, d;
    if (!APNUM_intToF64(pool, a->numerator, &n))
    {
        return false;
    }
    if (!APNUM_intToF64(pool, a->denominator, &d))
    {
        return false;
    }
    *out = n / d;
    return true;
}
















u32 APNUM_ratFromStr(APNUM_pool_t pool, APNUM_rat* out, u32 base, const char* str)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    u32 nlen = APNUM_intFromStr(pool, numerator, base, str);
    if (!nlen)
    {
        return 0;
    }
    if (str[nlen] != '/')
    {
        APNUM_intDup(out->numerator, numerator);
        APNUM_intFromS32(pool, out->denominator, 1);
        APNUM_intFree(pool, numerator);
        APNUM_intFree(pool, denominator);
        return nlen;
    }
    u32 dlen = APNUM_intFromStr(pool, denominator, base, str + nlen + 1);
    if (!dlen)
    {
        return 0;
    }
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
    return nlen + dlen + 1;
}







u32 APNUM_ratToStr(APNUM_pool_t pool, const APNUM_rat* a, u32 base, char* strBuf, u32 strBufSize)
{
    if (APNUM_intIsOne(a->denominator))
    {
        return APNUM_intToStr(pool, a->numerator, base, strBuf, strBufSize);
    }
    u32 nlen = APNUM_intToStr(pool, a->numerator, base, NULL, 0);
    u32 dlen = APNUM_intToStr(pool, a->denominator, base, NULL, 0);
    if (nlen + dlen + 1 >= strBufSize)
    {
        return nlen + dlen + 1;
    }
    u32 n;
    n = APNUM_intToStr(pool, a->numerator, base, strBuf, strBufSize);
    assert(n == nlen);
    strBuf[nlen] = '/';
    n = APNUM_intToStr(pool, a->denominator, base, strBuf + nlen + 1, strBufSize - nlen - 1);
    assert(n == dlen);
    return nlen + dlen + 1;
}







u32 APNUM_ratFromStrWithBaseFmt(APNUM_pool_t pool, APNUM_rat* out, const char* str)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    u32 nlen = APNUM_intFromStrWithBaseFmt(pool, numerator, str);
    if (!nlen)
    {
        APNUM_intFree(pool, numerator);
        APNUM_intFree(pool, denominator);
        return 0;
    }
    if (str[nlen] != '/')
    {
        APNUM_intDup(out->numerator, numerator);
        APNUM_intFromS32(pool, out->denominator, 1);
        APNUM_intFree(pool, numerator);
        APNUM_intFree(pool, denominator);
        return nlen;
    }
    u32 dlen = APNUM_intFromStrWithBaseFmt(pool, denominator, str + nlen + 1);
    if (!dlen)
    {
        return 0;
    }
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, numerator);
    APNUM_intFree(pool, denominator);
    return nlen + dlen + 1;
}






u32 APNUM_ratToStrWithBaseFmt(APNUM_pool_t pool, const APNUM_rat* a, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize)
{
    if (APNUM_intIsOne(a->denominator))
    {
        return APNUM_intToStrWithBaseFmt(pool, a->numerator, baseFmt, strBuf, strBufSize);
    }
    u32 nlen = APNUM_intToStrWithBaseFmt(pool, a->numerator, baseFmt, NULL, 0);
    u32 dlen = APNUM_intToStrWithBaseFmt(pool, a->denominator, baseFmt, NULL, 0);
    if (nlen + dlen + 1 >= strBufSize)
    {
        return nlen + dlen + 1;
    }
    u32 n;
    n = APNUM_intToStrWithBaseFmt(pool, a->numerator, baseFmt, strBuf, strBufSize);
    assert(n == nlen);
    strBuf[nlen] = '/';
    n = APNUM_intToStrWithBaseFmt(pool, a->denominator, baseFmt, strBuf + nlen + 1, strBufSize - nlen - 1);
    assert(n == dlen);
    return nlen + dlen + 1;
}





















void APNUM_ratAddInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intAddInP(pool, a->numerator, b->numerator);
        return;
    }
    APNUM_rat* c = APNUM_ratNew(pool);
    APNUM_ratAdd(pool, c, a, b);
    APNUM_ratDup(a, c);
    APNUM_ratFree(pool, c);
}


void APNUM_ratSubInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intSubInP(pool, a->numerator, b->numerator);
        return;
    }
    APNUM_rat* c = APNUM_ratNew(pool);
    APNUM_ratSub(pool, c, a, b);
    APNUM_ratDup(a, c);
    APNUM_ratFree(pool, c);
}


void APNUM_ratMulInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intMulInP(pool, a->numerator, b->numerator);
        return;
    }
    APNUM_rat* c = APNUM_ratNew(pool);
    APNUM_ratMul(pool, c, a, b);
    APNUM_ratDup(a, c);
    APNUM_ratFree(pool, c);
}


void APNUM_ratDivInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_int* numerator = APNUM_intNew(pool);
        APNUM_intDup(numerator, a->numerator);
        APNUM_ratFromInt(pool, a, numerator, b->numerator);
        return;
    }
    APNUM_rat* c = APNUM_ratNew(pool);
    APNUM_ratDiv(pool, c, a, b);
    APNUM_ratDup(a, c);
    APNUM_ratFree(pool, c);
}
















void APNUM_ratAdd(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intAdd(pool, out->numerator, a->numerator, b->numerator);
        APNUM_intFromS32(pool, out->denominator, 1);
        return;
    }
    APNUM_int* n = APNUM_intNew(pool);
    APNUM_int* d = APNUM_intNew(pool);
    APNUM_intMul(pool, n, a->numerator, b->denominator);
    APNUM_intMul(pool, d, b->numerator, a->denominator);
    APNUM_intAddInP(pool, n, d);
    APNUM_intMul(pool, d, a->denominator, b->denominator);
    APNUM_ratFromInt(pool, out, n, d);
    APNUM_intFree(pool, n);
    APNUM_intFree(pool, d);
}




void APNUM_ratSub(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intSub(pool, out->numerator, a->numerator, b->numerator);
        APNUM_intFromS32(pool, out->denominator, 1);
        return;
    }
    APNUM_int* n = APNUM_intNew(pool);
    APNUM_int* d = APNUM_intNew(pool);
    APNUM_intMul(pool, n, a->numerator, b->denominator);
    APNUM_intMul(pool, d, b->numerator, a->denominator);
    APNUM_intSubInP(pool, n, d);
    APNUM_intMul(pool, d, a->denominator, b->denominator);
    APNUM_ratFromInt(pool, out, n, d);
    APNUM_intFree(pool, n);
    APNUM_intFree(pool, d);
}




void APNUM_ratMul(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b)
{
    if (APNUM_ratIsInt(a) && APNUM_ratIsInt(b))
    {
        APNUM_intMul(pool, out->numerator, a->numerator, b->numerator);
        APNUM_intFromS32(pool, out->denominator, 1);
        return;
    }
    APNUM_int* n = APNUM_intNew(pool);
    APNUM_int* d = APNUM_intNew(pool);
    APNUM_intMul(pool, n, a->numerator, b->numerator);
    APNUM_intMul(pool, d, a->denominator, b->denominator);
    APNUM_ratFromInt(pool, out, n, d);
    APNUM_intFree(pool, n);
    APNUM_intFree(pool, d);
}




void APNUM_ratDiv(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b)
{
    if (0 == APNUM_intCmp(a->denominator, b->denominator))
    {
        APNUM_ratFromInt(pool, out, a->numerator, b->numerator);
        return;
    }
    APNUM_int* n = APNUM_intNew(pool);
    APNUM_int* d = APNUM_intNew(pool);
    APNUM_intMul(pool, n, a->numerator, b->denominator);
    APNUM_intMul(pool, d, a->denominator, b->numerator);
    APNUM_ratFromInt(pool, out, n, d);
    APNUM_intFree(pool, n);
    APNUM_intFree(pool, d);
}





















































































































