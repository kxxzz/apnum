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







APNUM_rat* APNUM_ratZero(APNUM_pool_t pool)
{
    if (pool->freeRationals->length > 0)
    {
        APNUM_rat* a = vec_last(pool->freeRationals);
        vec_pop(pool->freeRationals);

        a->numerator = APNUM_intZero(pool);
        a->denominator = APNUM_intZero(pool);
        APNUM_intFromU32(a->denominator, 1, 0);
        return a;
    }
    else
    {
        APNUM_rat* a = zalloc(sizeof(*a));
        return a;
    }
}

void APNUM_ratFree(APNUM_pool_t pool, APNUM_rat* a)
{
    APNUM_intFree(pool, a->denominator);
    APNUM_intFree(pool, a->numerator);
    vec_push(pool->freeRationals, a);
}









void APNUM_ratDup(APNUM_rat* out, const APNUM_rat* a)
{

}

void APNUM_ratNegation(APNUM_rat* a)
{

}

bool APNUM_ratIsZero(APNUM_rat* a)
{
    return APNUM_intIsZero(a->numerator);
}

bool APNUM_ratIsNeg(APNUM_rat* a)
{
    // todo
    return false;
}

int APNUM_ratCmp(const APNUM_rat* a, const APNUM_rat* b)
{
    // todo
    return 0;
}






















































































































