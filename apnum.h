#pragma once


#include <stdbool.h>
#include <stdint.h>

#include <vec.h>



typedef struct APNUM_int
{
    vec_u8 data;
    bool neg;
} APNUM_int;


void APNUM_intFree(APNUM_int* x);

bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str);

u32 APNUM_intToStr(const APNUM_int* x, u32 base, char* strBuf, u32 strBufSize);


void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);


int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b);













































