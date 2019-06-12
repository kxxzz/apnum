#pragma once




#include "apnum.h"

#include <malloc.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>

#include <vec.h>




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










typedef vec_t(APNUM_int*) APNUM_intPtrVec;
typedef vec_t(APNUM_rat*) APNUM_ratPtrVec;


typedef struct APNUM_pool
{
    APNUM_intPtrVec integers[1];
    APNUM_intPtrVec freeIntegers[1];

    APNUM_ratPtrVec rationals[1];
    APNUM_ratPtrVec freeRationals[1];
} APNUM_pool;











void APNUM_intSetNeg(APNUM_int* a, bool neg);









































































































