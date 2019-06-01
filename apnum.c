#include "apnum.h"

#include <malloc.h>
#include <string.h>
#include <math.h>




#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))







void APNUM_intFree(APNUM_int* x)
{
    mpz_clear(&x->impl);
}



bool APNUM_intFromDecStr(APNUM_int* out, const char* dec)
{
    int r = mpz_init_set_str(&out->impl, dec, 10);
    return 0 == r;
}




u32 APNUM_intToDecStr(const APNUM_int* x, char* decBuf, u32 decBufSize)
{
    char* s = mpz_get_str(decBuf, 10, &x->impl);
    u32 n = (u32)strnlen(s, decBufSize);
    return n;
}





void APNUM_intFromU8(APNUM_int* out, u8 x)
{
    mpz_init_set_ui(&out->impl, x);
}
void APNUM_intFromU16(APNUM_int* out, u16 x)
{
    mpz_init_set_ui(&out->impl, x);
}
void APNUM_intFromU32(APNUM_int* out, u32 x)
{
    mpz_init_set_ui(&out->impl, x);
}
void APNUM_intFromU64(APNUM_int* out, u64 x)
{
    mpz_init_set_ui(&out->impl, x);
}





void APNUM_intFromS8(APNUM_int* out, s8 x)
{
    mpz_init_set_si(&out->impl, x);
}
void APNUM_intFromS16(APNUM_int* out, s16 x)
{
    mpz_init_set_si(&out->impl, x);
}
void APNUM_intFromS32(APNUM_int* out, s32 x)
{
    mpz_init_set_si(&out->impl, x);
}
void APNUM_intFromS64(APNUM_int* out, s64 x)
{
    mpz_init_set_si(&out->impl, x);
}








void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}


void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}


void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}


void APNUM_intDiv(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{

}










bool APNUM_intGT(const APNUM_int* a, const APNUM_int* b)
{
    return false;
}





bool APNUM_intLT(const APNUM_int* a, const APNUM_int* b)
{
    return !APNUM_intGE(a, b);
}




bool APNUM_intGE(const APNUM_int* a, const APNUM_int* b)
{
    return false;
}




bool APNUM_intLE(const APNUM_int* a, const APNUM_int* b)
{
    return !APNUM_intGT(a, b);
}











































































