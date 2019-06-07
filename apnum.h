#pragma once


#include <stdbool.h>
#include <stdint.h>

#include <vec.h>



typedef enum APNUM_int_StrBaseFmtType
{
    APNUM_int_StrBaseFmtType_DEC = 0,
    APNUM_int_StrBaseFmtType_BIN,
    APNUM_int_StrBaseFmtType_OCT,
    APNUM_int_StrBaseFmtType_HEX,
} APNUM_int_StrBaseFmtType;



typedef struct APNUM_int APNUM_int;

void APNUM_intFree(APNUM_int* x);

APNUM_int* APNUM_intZero(void);
void APNUM_intDup(APNUM_int* out, const APNUM_int* x);

bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str);
u32 APNUM_intToStr(const APNUM_int* x, u32 base, char* strBuf, u32 strBufSize);

bool APNUM_intFromStrWithBaseFmt(APNUM_int* out, const char* str);
u32 APNUM_intToStrWithBaseFmt(const APNUM_int* x, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize);


void APNUM_intAddInP(APNUM_int* a, const APNUM_int* b);
void APNUM_intSubInP(APNUM_int* a, const APNUM_int* b);

void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b);
void APNUM_intDiv(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* a, const APNUM_int* b);


int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b);













































