#include "apnum_a.h"





// https://en.wikipedia.org/wiki/Euclidean_algorithm
// http://www.blackwasp.co.uk/RationalNumberArithmetic_2.aspx

void APNUM_intGCD_Euclid(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a0, const APNUM_int* b0)
{
    APNUM_int* q = APNUM_intNew(pool);
    APNUM_int* r = APNUM_intNew(pool);
    APNUM_int* a = APNUM_intNew(pool);
    APNUM_int* b = APNUM_intNew(pool);
    APNUM_intDup(a, a0);
    APNUM_intDup(b, b0);
    APNUM_intAbs(a);
    APNUM_intAbs(b);
next:
    if (APNUM_intIsZero(b))
    {
        APNUM_intDup(out, a);
        APNUM_intFree(pool, q);
        APNUM_intFree(pool, r);
        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
    }
    else
    {
        APNUM_intDiv(pool, q, r, a, b);
        APNUM_intDup(a, b);
        APNUM_intDup(b, r);
        goto next;
    }
}






// https://en.wikipedia.org/wiki/Greatest_common_divisor

void APNUM_intGCD(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_intGCD_Euclid(pool, out, a, b);
}





































































































