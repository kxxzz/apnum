#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include <vec.h>



typedef struct APNUM_int
{
    vec_u8 data;
    bool neg;
} APNUM_int;


void APNUM_intFree(APNUM_int* x);

bool APNUM_intFromDecStr(APNUM_int* out, const char* dec);

u32 APNUM_intToDecStr(const APNUM_int* x, char* decBuf, u32 decBufSize);


void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);


int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b);













































