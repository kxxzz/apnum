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

    {
        APNUM_int* a = APNUM_intZero();
        APNUM_int* b = APNUM_intZero();
        APNUM_intFromStr(a, 10, "344");
        APNUM_intFromStr(b, 10, "33");
        APNUM_int* c = APNUM_intZero();
        APNUM_int* d = APNUM_intZero();
        APNUM_intDiv(c, d, a, b);
        char buf[1024];
        n = APNUM_intToStr(c, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(d, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(a);
        APNUM_intFree(b);
        APNUM_intFree(c);
        APNUM_intFree(d);
    }

    {
        APNUM_int* a = APNUM_intZero();
        APNUM_intFromStr(a, 16, "-000bc5ea8");
        char buf[1024];
        n = APNUM_intToStr(a, 2, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(a, 8, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(a, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStr(a, 16, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(a);
    }

    {
        APNUM_int* a = APNUM_intZero();
        APNUM_intFromStrWithBaseFmt(a, "-0xbc5ea8");
        char buf[1024];
        n = APNUM_intToStrWithBaseFmt(a, APNUM_int_StrBaseFmtType_DEC, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(a, APNUM_int_StrBaseFmtType_BIN, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(a, APNUM_int_StrBaseFmtType_OCT, buf, sizeof(buf));
        printf("%s\n", buf);
        n = APNUM_intToStrWithBaseFmt(a, APNUM_int_StrBaseFmtType_HEX, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(a);
    }

    {
        APNUM_int* a = APNUM_intZero();
        r = APNUM_intFromStr(a, 10, "56789");
        assert(r);
        APNUM_int* b = APNUM_intZero();
        r = APNUM_intFromStr(b, 10, "92345");
        assert(r);
        APNUM_int* c = APNUM_intZero(); 
        APNUM_intSub(c, a, b);

        char buf[1024];
        n = APNUM_intToStr(c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(a);
        APNUM_intFree(b);
        APNUM_intFree(c);
    }

    {
        APNUM_int* a = APNUM_intZero();
        r = APNUM_intFromStr(a, 10, "0");
        assert(r);
        APNUM_int* b = APNUM_intZero();
        r = APNUM_intFromStr(b, 10, "-12");
        assert(r);
        int c = APNUM_intCmp(a, b);
        assert(1 == c);

        APNUM_intFree(a);
        APNUM_intFree(b);
    }

    {
        APNUM_int* a = APNUM_intZero();
        r = APNUM_intFromStr(a, 10, "23958233");
        assert(r);
        APNUM_int* b = APNUM_intZero();
        r = APNUM_intFromStr(b, 10, "5830");
        assert(r);
        APNUM_int* c = APNUM_intZero(); 
        APNUM_intMul(c, a, b);

        char buf[1024];
        n = APNUM_intToStr(c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(a);
        APNUM_intFree(b);
        APNUM_intFree(c);
    }

    {
        APNUM_int* a = APNUM_intZero();
        r = APNUM_intFromStr(a, 10, "2222222222222222222222222222222222222222222222");
        assert(r);
        APNUM_int* b = APNUM_intZero();
        r = APNUM_intFromStr(b, 10, "1239999999999999999999999999999999999999999");
        assert(r);
        APNUM_int* c = APNUM_intZero();
        APNUM_int* d = APNUM_intZero();
        APNUM_intDiv(c, d, a, b);

        char buf[1024];
        n = APNUM_intToStr(c, 10, buf, sizeof(buf));
        buf[n] = ' ';
        n = APNUM_intToStr(d, 10, buf + n + 1, sizeof(buf) - n - 1);
        printf("%s\n", buf);

        APNUM_intFree(a);
        APNUM_intFree(b);
        APNUM_intFree(c);
        APNUM_intFree(d);
    }
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



















































































