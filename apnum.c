#include "apnum.h"




#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))







void APNUM_intFree(APNUM_int* x)
{
    vec_free(&x->data);
}



bool APNUM_intFromDecStr(APNUM_int* out, const char* dec)
{
    APNUM_int x = { 0 };
    x.neg = '-' == dec[0];
    u32 len = (u32)strlen(dec);
    u32 sp = (('-' == dec[0]) || ('+' == dec[0])) ? 1 : 0;
    for (u32 i = sp; i < len; ++i)
    {
        if ((dec[i] < '0') || (dec[i] > '9'))
        {
            return false;
        }
    }
    u32 dataSize = len - sp;
    vec_resize(&x.data, dataSize);
    for (u32 i = sp; i < len; ++i)
    {
        u32 j = dataSize - 1 - (i - sp);
        x.data.data[j] = dec[i] - '0';
    }
    *out = x;
    return true;
}




u32 APNUM_intToDecStr(const APNUM_int* x, char* decBuf, u32 decBufSize)
{
    u32 sp = x->neg ? 1 : 0;
    u32 len = x->data.length + sp;
    if (decBufSize <= len)
    {
        return len;
    }
    if (x->neg)
    {
        decBuf[0] = '-';
    }
    for (u32 i = sp; i < len; ++i)
    {
        u32 j = x->data.length - 1 - (i - sp);
        decBuf[i] = x->data.data[j] + '0';
    }
    decBuf[len] = 0;
    return len;
}

































































































