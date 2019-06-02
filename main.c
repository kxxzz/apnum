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
        APNUM_int a = { 0 };
        r = APNUM_intFromStr(&a, 10, "12345000");
        assert(r);
        char buf[1024];
        n = APNUM_intToStr(&a, 10, buf, sizeof(buf));
        printf("%s\n", buf);
        APNUM_intFree(&a);
    }

    {
        APNUM_int a = { 0 };
        APNUM_int b = { 0 };
        r = APNUM_intFromStr(&a, 10, "56789");
        assert(r);
        r = APNUM_intFromStr(&b, 10, "-92345");
        assert(r);
        APNUM_int c = { 0 };
        APNUM_intAdd(&c, &a, &b);

        char buf[1024];
        n = APNUM_intToStr(&c, 10, buf, sizeof(buf));
        printf("%s\n", buf);

        APNUM_intFree(&a);
        APNUM_intFree(&b);
        APNUM_intFree(&c);
    }

    {
        APNUM_int a = { 0 };
        APNUM_int b = { 0 };
        r = APNUM_intFromStr(&a, 10, "12");
        assert(r);
        r = APNUM_intFromStr(&b, 10, "-12");
        assert(r);
        int c = APNUM_intCmp(&a, &b);
        assert(1 == c);

        APNUM_intFree(&a);
        APNUM_intFree(&b);
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



















































































