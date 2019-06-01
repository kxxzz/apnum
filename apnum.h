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


void APNUM_intFromU8(APNUM_int* out, u8 x);
void APNUM_intFromU16(APNUM_int* out, u16 x);
void APNUM_intFromU32(APNUM_int* out, u32 x);
void APNUM_intFromU64(APNUM_int* out, u64 x);


void APNUM_intFromS8(APNUM_int* out, s8 x);
void APNUM_intFromS16(APNUM_int* out, s16 x);
void APNUM_intFromS32(APNUM_int* out, s32 x);
void APNUM_intFromS64(APNUM_int* out, s64 x);


void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);



bool APNUM_intGT(const APNUM_int* a, const APNUM_int* b);
bool APNUM_intLT(const APNUM_int* a, const APNUM_int* b);
bool APNUM_intGE(const APNUM_int* a, const APNUM_int* b);
bool APNUM_intLE(const APNUM_int* a, const APNUM_int* b);













































