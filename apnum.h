#pragma once


#include <stdbool.h>
#include <stdint.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;




typedef enum APNUM_int_StrBaseFmtType
{
    APNUM_int_StrBaseFmtType_DEC = 0,
    APNUM_int_StrBaseFmtType_BIN,
    APNUM_int_StrBaseFmtType_OCT,
    APNUM_int_StrBaseFmtType_HEX,
} APNUM_int_StrBaseFmtType;




typedef struct APNUM_pool* APNUM_pool_t;

APNUM_pool_t APNUM_poolNew(void);
void APNUM_poolFree(APNUM_pool_t pool);




typedef struct APNUM_int APNUM_int;

APNUM_int* APNUM_intZero(APNUM_pool_t pool);
void APNUM_intFree(APNUM_pool_t pool, APNUM_int* a);


void APNUM_intDup(APNUM_int* out, const APNUM_int* a);
void APNUM_intNegation(APNUM_int* a);
bool APNUM_intIsZero(APNUM_int* a);
bool APNUM_intIsNeg(APNUM_int* a);
int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b);


void APNUM_intFromU32(APNUM_int* out, u32 u);
void APNUM_intFromS32(APNUM_int* out, s32 i);

u32 APNUM_intFromStr(APNUM_pool_t pool, APNUM_int* out, u32 base, const char* str);
u32 APNUM_intToStr(APNUM_pool_t pool, const APNUM_int* a, u32 base, char* strBuf, u32 strBufSize);

u32 APNUM_intFromStrWithBaseFmt(APNUM_pool_t pool, APNUM_int* out, const char* str);
u32 APNUM_intToStrWithBaseFmt
(
    APNUM_pool_t pool, const APNUM_int* a, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize
);


void APNUM_intAddInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b);
void APNUM_intSubInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b);

void APNUM_intAdd(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_pool_t pool, APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D);





typedef struct APNUM_rat APNUM_rat;

APNUM_rat* APNUM_ratZero(APNUM_pool_t pool);
void APNUM_ratFree(APNUM_pool_t pool, APNUM_rat* a);


void APNUM_ratDup(APNUM_rat* out, const APNUM_rat* a);
void APNUM_ratNegation(APNUM_rat* a);
bool APNUM_ratIsZero(APNUM_rat* a);
bool APNUM_ratIsNeg(APNUM_rat* a);
int APNUM_ratCmp(const APNUM_rat* a, const APNUM_rat* b);





void APNUM_intGCD(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);































