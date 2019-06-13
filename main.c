#include <stdlib.h>
#ifdef _WIN32
# include <crtdbg.h>
#endif



#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "apnum.h"





#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))




static void test(void)
{
    u32 n;

    APNUM_pool_t pool = APNUM_poolNew();

    {
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, "");
        assert(0 == n);
        char buf[1024];
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, "-");
        assert(0 == n);
        char buf[1024];
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        APNUM_int* a = APNUM_intNew(pool);
        APNUM_intFromS64(pool, a, -1234567890123456789);
        char buf[1024];
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        const char astr[] = "-1234567890123456789";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        s64 s;
        bool r = APNUM_intToS64(pool, a, &s);
        assert(r);
        char buf[1024];
        n = sprintf(buf, "%"PRId64, s);
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        const char astr[] = "-12345678901234567890";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        s64 s = 0;
        bool r = APNUM_intToS64(pool, a, &s);
        assert(!r);
        assert(!s);
        char buf[1024];
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        const char astr[] = "-999999999";
        const char bstr[] = "56789";
        APNUM_int* a = APNUM_intNew(pool);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_int* d = APNUM_intNew(pool);
        APNUM_intDiv(pool, c, d, a, b);
        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, d, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
        APNUM_intFree(pool, d);
    }

    {
        const char astr[] = "-000bc5ea8";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 16, astr);
        assert((u32)strlen(astr) == n);

        u32 n0 = APNUM_intToStr(pool, a, 10, NULL, 0);

        char buf[1024];
        n = APNUM_intToStr(pool, a, 2, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 8, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        assert(n0 == n);
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 16, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        const char astr[] = "-0xbc5ea8";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStrWithBaseFmt(pool, a, astr);
        assert((u32)strlen(astr) == n);

        u32 n0 = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_HEX, NULL, 0);

        char buf[1024];
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_DEC, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_BIN, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_OCT, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_HEX, buf, sizeof(buf));
        assert(n0 == n);
        buf[n] = 0;
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        const char astr[] = "-56789";
        const char bstr[] = "92345";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_intSub(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        APNUM_int* a = APNUM_intNew(pool);
        APNUM_intFromS32(pool, a, -56789);
        APNUM_int* b = APNUM_intNew(pool);
        APNUM_intFromS32(pool, b, 92345);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_intSub(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        const char astr[] = "0";
        const char bstr[] = "-12";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        int c = APNUM_intCmp(a, b);
        assert(1 == c);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
    }

    {
        const char astr[] = "23958233";
        const char bstr[] = "5830";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_intMul(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        const char astr[] = "2222222222222222222222222222222222222222222222999999999999999999999999999999999";
        const char bstr[] = "1239999999999999999999999999999999999999999";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_int* d = APNUM_intNew(pool);
        APNUM_intDiv(pool, c, d, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = ' ';
        n = APNUM_intToStr(pool, d, 10, buf + n + 1, sizeof(buf) - n - 1);
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
        APNUM_intFree(pool, d);
    }

    {
        const char astr[] = "-54";
        const char bstr[] = "-24";
        APNUM_int* a = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, a, 10, astr);
        assert((u32)strlen(astr) == n);
        APNUM_int* b = APNUM_intNew(pool);
        n = APNUM_intFromStr(pool, b, 10, bstr);
        assert((u32)strlen(bstr) == n);
        APNUM_int* c = APNUM_intNew(pool);
        APNUM_intGCD(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        APNUM_rat* a = APNUM_ratNew(pool);
        APNUM_ratFromStr(pool, a, 10, "480/-24");

        char buf[1024];
        n = APNUM_ratToStr(pool, a, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_ratFree(pool, a);
    }

    {
        APNUM_rat* a = APNUM_ratNew(pool);
        APNUM_ratFromStrWithBaseFmt(pool, a, "54/-0x11");

        char buf[1024];
        n = APNUM_ratToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_HEX, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_ratFree(pool, a);
    }

    {
        int r;
        APNUM_rat* a = APNUM_ratNew(pool);
        APNUM_rat* b = APNUM_ratNew(pool);
        APNUM_rat* c = APNUM_ratNew(pool);
        APNUM_ratFromStr(pool, a, 10, "10/-30");
        APNUM_ratFromStr(pool, b, 10, "33/99");

        r = APNUM_ratCmp(pool, a, b);
        assert(-1 == r);

        APNUM_ratAbs(a);
        r = APNUM_ratCmp(pool, a, b);
        assert(0 == r);

        APNUM_ratFromStr(pool, a, 10, "-100/-3");
        APNUM_ratFromStr(pool, b, 10, "33/99");
        APNUM_ratDiv(pool, c, a, b);

        char buf[1024];
        n = APNUM_ratToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_ratFree(pool, a);
        APNUM_ratFree(pool, b);
        APNUM_ratFree(pool, c);
    }

    {
        APNUM_rat* a = APNUM_ratNew(pool);
        APNUM_ratFromStrWithBaseFmt(pool, a, "1/-3");

        f32 f;
        APNUM_ratToF32(pool, a, &f);

        char buf[1024];
        n = sprintf(buf, "%f", f);
        buf[n] = 0;
        printf("%s\n", buf);

        APNUM_ratFree(pool, a);
    }

    APNUM_poolFree(pool);
}






int main(int argc, char* argv[])
{
    test();
    return 0;
}



















































































