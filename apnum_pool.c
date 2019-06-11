#include "apnum_a.h"









APNUM_pool_t APNUM_poolNew(void)
{
    APNUM_pool_t pool = zalloc(sizeof(APNUM_pool));
    return pool;
}







void APNUM_intFreeMem(APNUM_int* a);
void APNUM_ratFreeMem(APNUM_rat* a);



void APNUM_poolFree(APNUM_pool_t pool)
{
    vec_free(pool->freeRationals);
    for (u32 i = 0; i < pool->rationals->length; ++i)
    {
        APNUM_ratFreeMem(pool->rationals->data[i]);
    }
    vec_free(pool->rationals);

    vec_free(pool->freeIntegers);
    for (u32 i = 0; i < pool->integers->length; ++i)
    {
        APNUM_intFreeMem(pool->integers->data[i]);
    }
    vec_free(pool->integers);

    free(pool);
}
























































































































