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

APNUM_int* APNUM_intNew(APNUM_pool_t pool);
void APNUM_intFree(APNUM_pool_t pool, APNUM_int* a);


void APNUM_intDup(APNUM_int* out, const APNUM_int* a);
void APNUM_intAbs(APNUM_int* a);
void APNUM_intNeg(APNUM_int* a);
bool APNUM_intIsZero(const APNUM_int* a);
bool APNUM_intIsOne(const APNUM_int* a);
bool APNUM_intIsNeg(const APNUM_int* a);
bool APNUM_intEq(const APNUM_int* a, const APNUM_int* b);


int APNUM_intCmpAbs(const APNUM_int* a, const APNUM_int* b);
int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b);


void APNUM_intFromU32(APNUM_pool_t pool, APNUM_int* out, u32 u, bool neg);
void APNUM_intFromU64(APNUM_pool_t pool, APNUM_int* out, u64 u, bool neg);
void APNUM_intFromS32(APNUM_pool_t pool, APNUM_int* out, s32 i);
void APNUM_intFromS64(APNUM_pool_t pool, APNUM_int* out, s64 i);

bool APNUM_intToU32(APNUM_pool_t pool, APNUM_int* a, u32* out);
bool APNUM_intToU64(APNUM_pool_t pool, APNUM_int* a, u64* out);
bool APNUM_intToS32(APNUM_pool_t pool, APNUM_int* a, s32* out);
bool APNUM_intToS64(APNUM_pool_t pool, APNUM_int* a, s64* out);
bool APNUM_intToF32(APNUM_pool_t pool, APNUM_int* a, f32* out);
bool APNUM_intToF64(APNUM_pool_t pool, APNUM_int* a, f64* out);

u32 APNUM_intFromStr(APNUM_pool_t pool, APNUM_int* out, u32 base, const char* str);
u32 APNUM_intToStr(APNUM_pool_t pool, const APNUM_int* a, u32 base, char* strBuf, u32 strBufSize);

u32 APNUM_intFromStrWithBaseFmt(APNUM_pool_t pool, APNUM_int* out, const char* str);
u32 APNUM_intToStrWithBaseFmt(APNUM_pool_t pool, const APNUM_int* a, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize);


void APNUM_intAddInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b);
void APNUM_intSubInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b);
void APNUM_intMulInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b);
void APNUM_intDivInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b, APNUM_int* r);

void APNUM_intAdd(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_pool_t pool, APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D);





typedef struct APNUM_rat APNUM_rat;

APNUM_rat* APNUM_ratNew(APNUM_pool_t pool);
void APNUM_ratFree(APNUM_pool_t pool, APNUM_rat* a);


void APNUM_ratDup(APNUM_rat* out, const APNUM_rat* a);
void APNUM_ratAbs(APNUM_rat* a);
void APNUM_ratNeg(APNUM_rat* a);
void APNUM_ratNum(APNUM_int* out, const APNUM_rat* a);
void APNUM_ratDom(APNUM_int* out, const APNUM_rat* a);
bool APNUM_ratIsZero(const APNUM_rat* a);
bool APNUM_ratIsOne(const APNUM_rat* a);
bool APNUM_ratIsNeg(const APNUM_rat* a);
bool APNUM_ratIsInt(const APNUM_rat* a);
bool APNUM_ratEq(const APNUM_rat* a, const APNUM_rat* b);


int APNUM_ratCmp(APNUM_pool_t pool, const APNUM_rat* a, const APNUM_rat* b);


void APNUM_ratFromU32(APNUM_pool_t pool, APNUM_rat* out, u32 n, u32 d, bool neg);
void APNUM_ratFromU64(APNUM_pool_t pool, APNUM_rat* out, u64 n, u64 d, bool neg);
void APNUM_ratFromS32(APNUM_pool_t pool, APNUM_rat* out, s32 n, s32 d);
void APNUM_ratFromS64(APNUM_pool_t pool, APNUM_rat* out, s64 n, s64 d);

void APNUM_ratFromInt(APNUM_pool_t pool, APNUM_rat* out, const APNUM_int* n, const APNUM_int* d);

bool APNUM_ratToU32(APNUM_pool_t pool, APNUM_rat* a, u32* out);
bool APNUM_ratToU64(APNUM_pool_t pool, APNUM_rat* a, u64* out);
bool APNUM_ratToS32(APNUM_pool_t pool, APNUM_rat* a, s32* out);
bool APNUM_ratToS64(APNUM_pool_t pool, APNUM_rat* a, s64* out);
bool APNUM_ratToF32(APNUM_pool_t pool, APNUM_rat* a, f32* out);
bool APNUM_ratToF64(APNUM_pool_t pool, APNUM_rat* a, f64* out);


u32 APNUM_ratFromStr(APNUM_pool_t pool, APNUM_rat* out, u32 base, const char* str);
u32 APNUM_ratToStr(APNUM_pool_t pool, const APNUM_rat* a, u32 base, char* strBuf, u32 strBufSize);

u32 APNUM_ratFromStrWithBaseFmt(APNUM_pool_t pool, APNUM_rat* out, const char* str);
u32 APNUM_ratToStrWithBaseFmt(APNUM_pool_t pool, const APNUM_rat* a, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize);


void APNUM_ratAddInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratSubInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratMulInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratDivInP(APNUM_pool_t pool, APNUM_rat* a, const APNUM_rat* b);

void APNUM_ratAdd(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratSub(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratMul(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b);
void APNUM_ratDiv(APNUM_pool_t pool, APNUM_rat* out, const APNUM_rat* a, const APNUM_rat* b);









void APNUM_intGCD(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b);































