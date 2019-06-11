#include <stdlib.h>
#ifdef _WIN32
# include <crtdbg.h>
#endif



#include <stdio.h>
#include <assert.h>

#include "apnum.h"





#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))




static void test(void)
{
    bool r;
    u32 n;

    APNUM_pool_t pool = APNUM_poolNew();
    {
        APNUM_int* a = APNUM_intZero(pool);
        APNUM_int* b = APNUM_intZero(pool);
        APNUM_intFromStr(pool, a, 10, "-999999999");
        APNUM_intFromStr(pool, b, 10, "56789");
        APNUM_int* c = APNUM_intZero(pool);
        APNUM_int* d = APNUM_intZero(pool);
        APNUM_intDiv(pool, c, d, a, b);
        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, d, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
        APNUM_intFree(pool, d);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        APNUM_intFromStr(pool, a, 16, "-000bc5ea8");
        char buf[1024];
        n = APNUM_intToStr(pool, a, 2, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 8, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(pool, a, 16, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        APNUM_intFromStrWithBaseFmt(pool, a, "-0xbc5ea8");
        char buf[1024];
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_DEC, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_BIN, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_OCT, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(pool, a, APNUM_int_StrBaseFmtType_HEX, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(pool, a);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, a, 10, "-56789");
        assert(r);
        APNUM_int* b = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, b, 10, "92345");
        assert(r);
        APNUM_int* c = APNUM_intZero(pool);
        APNUM_intSub(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        APNUM_intFromS32(a, -56789);
        APNUM_int* b = APNUM_intZero(pool);
        APNUM_intFromS32(b, 92345);
        APNUM_int* c = APNUM_intZero(pool);
        APNUM_intSub(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, a, 10, "0");
        assert(r);
        APNUM_int* b = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, b, 10, "-12");
        assert(r);
        int c = APNUM_intCmp(a, b);
        assert(1 == c);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, a, 10, "23958233");
        assert(r);
        APNUM_int* b = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, b, 10, "5830");
        assert(r);
        APNUM_int* c = APNUM_intZero(pool);
        APNUM_intMul(pool, c, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
    }

    {
        APNUM_int* a = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, a, 10, "2222222222222222222222222222222222222222222222999999999999999999999999999999999");
        assert(r);
        APNUM_int* b = APNUM_intZero(pool);
        r = APNUM_intFromStr(pool, b, 10, "1239999999999999999999999999999999999999999");
        assert(r);
        APNUM_int* c = APNUM_intZero(pool);
        APNUM_int* d = APNUM_intZero(pool);
        APNUM_intDiv(pool, c, d, a, b);

        char buf[1024];
        n = APNUM_intToStr(pool, c, 10, buf, sizeof(buf));
        buf[n] = ' ';
        n = APNUM_intToStr(pool, d, 10, buf + n + 1, sizeof(buf) - n - 1);
        printf("%s\n", buf);

        APNUM_intFree(pool, a);
        APNUM_intFree(pool, b);
        APNUM_intFree(pool, c);
        APNUM_intFree(pool, d);
    }

    APNUM_poolFree(pool);
}





static int mainReturn(int r)
{
#if !defined(NDEBUG) && defined(_WIN32)
    system("pause");
#endif
    return r;
}


int main(int argc, char* argv[])
{
    test();
    mainReturn(0);
}



















































































