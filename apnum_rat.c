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

        APNUM_intFromU32(pool, a->numerator, 0, 0);
        APNUM_intFromU32(pool, a->denominator, 1, 0);
        return a;
    }
    else
    {
        APNUM_rat* a = zalloc(sizeof(*a));
        vec_push(pool->rationals, a);

        a->numerator = APNUM_intNew(pool);
        a->denominator = APNUM_intNew(pool);
        APNUM_intFromU32(pool, a->denominator, 1, 0);
        return a;
    }
}

void APNUM_ratFree(APNUM_pool_t pool, APNUM_rat* a)
{
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

bool APNUM_ratIsZero(APNUM_rat* a)
{
    return APNUM_intIsZero(a->numerator);
}

bool APNUM_ratIsNeg(APNUM_rat* a)
{
    return APNUM_intIsNeg(a->numerator);
}

int APNUM_ratCmp(const APNUM_rat* a, const APNUM_rat* b)
{
    // todo
    return 0;
}















void APNUM_ratFromU32(APNUM_pool_t pool, APNUM_rat* out, u32 n, u32 d, bool neg)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromU32(pool, numerator, n, neg);
    APNUM_intFromU32(pool, denominator, d, false);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, denominator);
    APNUM_intFree(pool, numerator);
}

void APNUM_ratFromS32(APNUM_pool_t pool, APNUM_rat* out, s32 n, s32 d)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromS32(pool, numerator, n);
    APNUM_intFromS32(pool, denominator, d);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, denominator);
    APNUM_intFree(pool, numerator);
}

void APNUM_ratFromU64(APNUM_pool_t pool, APNUM_rat* out, u64 n, u64 d, bool neg)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromU64(pool, numerator, n, neg);
    APNUM_intFromU64(pool, denominator, d, false);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, denominator);
    APNUM_intFree(pool, numerator);
}

void APNUM_ratFromS64(APNUM_pool_t pool, APNUM_rat* out, s64 n, s64 d)
{
    APNUM_int* numerator = APNUM_intNew(pool);
    APNUM_int* denominator = APNUM_intNew(pool);
    APNUM_intFromS64(pool, numerator, n);
    APNUM_intFromS64(pool, denominator, d);
    APNUM_ratFromInt(pool, out, numerator, denominator);
    APNUM_intFree(pool, denominator);
    APNUM_intFree(pool, numerator);
}











void APNUM_ratFromInt(APNUM_pool_t pool, APNUM_rat* out, const APNUM_int* n, const APNUM_int* d)
{
    APNUM_int* gcd = APNUM_intNew(pool);
    APNUM_int* r = APNUM_intNew(pool);
    APNUM_intGCD(pool, gcd, n, d);
    APNUM_intDiv(pool, out->numerator, r, n, gcd);
    assert(APNUM_intIsZero(r));
    APNUM_intDiv(pool, out->denominator, r, n, gcd);
    assert(APNUM_intIsZero(r));
    assert(!APNUM_intIsZero(out->denominator));
    APNUM_intFree(pool, r);
    APNUM_intFree(pool, gcd);
    APNUM_intSetNeg(out->numerator, APNUM_intIsNeg(n) ^ APNUM_intIsNeg(d));
    APNUM_intAbs(out->denominator);
}













































































































