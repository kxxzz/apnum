#include "apnum.h"
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <assert.h>



#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))



#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))




#define zalloc(sz) calloc(1, sz)






typedef u32 APNUM_Digit;
typedef s64 APNUM_Wigit;
typedef vec_t(APNUM_Digit) APNUM_DigitVec;



#define APNUM_Digit_Base (APNUM_Digit)-1
static_assert(APNUM_Digit_Base <= (APNUM_Digit)-1, "");



#define APNUM_StrChar_Base_MAX 35u









typedef struct APNUM_int
{
    APNUM_DigitVec digits[1];
    bool neg;
} APNUM_int;






void APNUM_intFree(APNUM_int* x)
{
    vec_free(x->digits);
    free(x);
}




void APNUM_intDup(APNUM_int* out, const APNUM_int* x)
{
    vec_dup(out->digits, x->digits);
    out->neg = x->neg;
}





APNUM_int* APNUM_intZero(void)
{
    APNUM_int* a = zalloc(sizeof(*a));
    return a;
}





static void APNUM_intSwap(APNUM_int* a, APNUM_int* b)
{
    APNUM_int t = *a;
    *a = *b;
    *b = t;
}





static void APNUM_intDigitsByU32(APNUM_int* a, u32 x)
{
    vec_resize(a->digits, 0);
    while (x)
    {
        u32 r = x % APNUM_Digit_Base;
        vec_push(a->digits, r);
        x /= APNUM_Digit_Base;
    }
}




static void APNUM_intDightsInsertAt0(APNUM_int* a, APNUM_Digit x)
{
    assert(x < APNUM_Digit_Base);
    if ((a->digits->length > 0) || x)
    {
        vec_insert(a->digits, 0, x);
    }
}





static APNUM_Digit APNUM_digitFromChar(char c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return (APNUM_Digit)c - '0';
    }
    else if ((c >= 'a') && (c <= 'z'))
    {
        return (APNUM_Digit)c - 'a' + 10;
    }
    else if ((c >= 'A') && (c <= 'Z'))
    {
        return (APNUM_Digit)c - 'A' + 10;
    }
    else
    {
        return APNUM_StrChar_Base_MAX + 1;
    }
}





static int APNUM_absCmpInt(const APNUM_Digit* a, u32 na, const APNUM_Digit* b, u32 nb)
{
    if (na > nb)
    {
        return 1;
    }
    else if (na < nb)
    {
        return -1;
    }
    else
    {
        assert(na == nb);
        for (u32 i = 0; i < na; ++i)
        {
            u32 j = na - 1 - i;
            APNUM_Digit ea = a[j];
            APNUM_Digit eb = b[j];
            if (ea > eb)
            {
                return 1;
            }
            else if (ea < eb)
            {
                return -1;
            }
        }
    }
    return 0;
}





static int APNUM_intCmpAbs(const APNUM_int* a, const APNUM_int* b)
{
    return APNUM_absCmpInt(a->digits->data, a->digits->length, b->digits->data, b->digits->length);
}





static void APNUM_intTruncate(APNUM_int* a)
{
    while (a->digits->length && (vec_last(a->digits) == 0))
    {
        vec_pop(a->digits);
    }
}




























static bool APNUM_intFromStrWithHead(APNUM_int* out, u32 base, const char* str, u32 headLen)
{
    if (base > APNUM_StrChar_Base_MAX)
    {
        return false;
    }
    APNUM_int* a = out;
    vec_resize(a->digits, 0);

    u32 len = (u32)strlen(str);
    if ((1 == len) && ('0' == str[0]))
    {
        a->neg = false;
        return true;
    }
    bool neg = '-' == str[0];
    u32 sp = (('-' == str[0]) || ('+' == str[0])) ? 1 : 0;
    sp += headLen;
    for (u32 i = sp; i < len; ++i)
    {
        APNUM_Digit d = APNUM_digitFromChar(str[i]);
        if (d > base)
        {
            return false;
        }
    }
    a->neg = neg;
    for (u32 i = sp; i < len; ++i)
    {
        if ('0' == str[i])
        {
            ++sp;
        }
        else
        {
            break;
        }
    }

    APNUM_int* b = APNUM_intZero();
    APNUM_int* a1 = APNUM_intZero();

    b->neg = a->neg;
    APNUM_intDigitsByU32(b, APNUM_digitFromChar(str[sp]));
    APNUM_intAddInP(a, b);

    for (u32 i = sp + 1; i < len; ++i)
    {
        b->neg = false;
        APNUM_intDigitsByU32(b, base);
        APNUM_intMul(a1, a, b);
        APNUM_intSwap(a, a1);

        b->neg = a->neg;
        APNUM_intDigitsByU32(b, APNUM_digitFromChar(str[i]));
        APNUM_intAddInP(a, b);
    }

    APNUM_intFree(a1);
    APNUM_intFree(b);
    return true;
}







static u32 APNUM_intToStrWithHead(const APNUM_int* a, u32 base, char* strBuf, u32 strBufSize, u32 headLen, const char* head)
{
    static const char charTable[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (base > APNUM_StrChar_Base_MAX)
    {
        return 0;
    }
    if (0 == a->digits->length)
    {
        strBuf[0] = '0';
        strBuf[1] = 0;
        return 1;
    }
    u32 sp = a->neg ? 1 : 0;
    if (a->neg)
    {
        strBuf[0] = '-';
    }
    for (u32 i = 0; i < headLen; ++i)
    {
        strBuf[sp + i] = head[i];
    }
    sp += headLen;

    APNUM_int* q = APNUM_intZero();
    APNUM_intDup(q, a);
    q->neg = false;

    APNUM_int* q1 = APNUM_intZero();
    APNUM_int* r = APNUM_intZero();
    APNUM_int* ibase = APNUM_intZero();
    APNUM_intDigitsByU32(ibase, base);

    vec_char buf[1] = { 0 };
    do 
    {
        APNUM_intDiv(q1, r, q, ibase);
        APNUM_intSwap(q, q1);
        char c = 0;
        if (r->digits->length > 0)
        {
            c += r->digits->data[r->digits->length - 1];
            for (u32 i = 1; i < r->digits->length; ++i)
            {
                u32 j = r->digits->length - i - 1;
                c = c * APNUM_Digit_Base + r->digits->data[j];
            }
        }
        assert(c <= (char)base);
        c = charTable[c];
        vec_push(buf, c);
    } while (q->digits->length > 0);

    APNUM_intFree(ibase);
    APNUM_intFree(r);
    APNUM_intFree(q1);
    APNUM_intFree(q);

    u32 len = sp + buf->length;
    if (len >= strBufSize)
    {
        goto out;
    }
    for (u32 i = sp; i < len; ++i)
    {
        u32 j = len - 1 - i;
        strBuf[i] = buf->data[j];
    }
    strBuf[len] = 0;
out:
    vec_free(buf);
    return len;
}









bool APNUM_intFromStr(APNUM_int* out, u32 base, const char* str)
{
    return APNUM_intFromStrWithHead(out, base, str, 0);
}


u32 APNUM_intToStr(const APNUM_int* x, u32 base, char* strBuf, u32 strBufSize)
{
    return APNUM_intToStrWithHead(x, base, strBuf, strBufSize, 0, NULL);
}


bool APNUM_intFromStrWithBaseFmt(APNUM_int* out, const char* str)
{
    u32 base = 10;
    u32 headLen = 0;
    u32 sp = (('-' == str[0]) || ('+' == str[0])) ? 1 : 0;
    if ('0' == str[sp])
    {
        switch (str[sp + 1])
        {
        case 'b':
        case 'B':
            base = 2;
            headLen = 2;
            break;
        case 'o':
        case 'O':
            base = 8;
            headLen = 2;
            break;
        case 'x':
        case 'X':
            base = 16;
            headLen = 2;
            break;
        default:
            break;
        }
    }
    return APNUM_intFromStrWithHead(out, base, str, headLen);
}


u32 APNUM_intToStrWithBaseFmt(const APNUM_int* x, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize)
{
    u32 base = 10;
    u32 headLen = 0;
    const char* head = NULL;
    switch (baseFmt)
    {
    case APNUM_int_StrBaseFmtType_DEC:
        base = 10;
        headLen = 0;
        head = NULL;
        break;
    case APNUM_int_StrBaseFmtType_BIN:
        base = 2;
        headLen = 2;
        head = "0b";
        break;
    case APNUM_int_StrBaseFmtType_OCT:
        base = 8;
        headLen = 2;
        head = "0o";
        break;
    case APNUM_int_StrBaseFmtType_HEX:
        base = 16;
        headLen = 2;
        head = "0x";
        break;
    default:
        assert(false);
        break;
    }
    return APNUM_intToStrWithHead(x, base, strBuf, strBufSize, headLen, head);
}













bool APNUM_intIsZero(APNUM_int* x)
{
    if (x->neg)
    {
        assert(x->digits->length > 0);
    }
    return 0 == x->digits->length;
}















void APNUM_intAddInP(APNUM_int* a, const APNUM_int* b)
{
    u32 alen = a->digits->length;
    u32 blen = b->digits->length;
    u32 len = max(alen, blen);

    bool outNeg = false;
    if (a->neg && b->neg)
    {
        outNeg = true;
    }
    else if (a->neg)
    {
        outNeg = APNUM_intCmpAbs(a, b) > 0;
    }
    else if (b->neg)
    {
        outNeg = APNUM_intCmpAbs(a, b) < 0;
    }
    s8 signA = (a->neg ^ outNeg) ? -1 : 1;
    s8 signB = (b->neg ^ outNeg) ? -1 : 1;
    a->neg = outNeg;

    vec_resize(a->digits, len);
    APNUM_Wigit e = 0;
    s8 carry = 0;
    for (u32 i = 0; i < len; ++i)
    {
        APNUM_Wigit ea = (i < alen) ? a->digits->data[i] : 0;
        APNUM_Wigit eb = (i < blen) ? b->digits->data[i] : 0;
        ea = ea * signA;
        eb = eb * signB;
        e = ea + eb + carry;

        if (e < 0)
        {
            e += APNUM_Digit_Base;
            carry = -1;
        }
        else
        if (e >= APNUM_Digit_Base)
        {
            e -= APNUM_Digit_Base;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        assert(e < APNUM_Digit_Base);
        a->digits->data[i] = (APNUM_Digit)e;
    }
    assert(carry >= 0);
    if (carry > 0)
    {
        vec_push(a->digits, 1);
    }
    APNUM_intTruncate(a);
}


void APNUM_intSubInP(APNUM_int* a, const APNUM_int* b)
{
    APNUM_int negb[1] = { 0 };
    negb->digits[0] = b->digits[0];
    negb->neg = !b->neg;
    APNUM_intAddInP(a, negb);
}













void APNUM_intAdd(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intAddInP(out, b);
}


void APNUM_intSub(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intSubInP(out, b);
}









// https://en.wikipedia.org/wiki/Multiplication_algorithm



// Long multiplication

static void APNUM_intMulLong(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    APNUM_int* sum = out;
    vec_resize(sum->digits, a->digits->length + b->digits->length);
    vec_setzero(sum->digits);
    if (APNUM_intCmpAbs(a, b) < 0)
    {
        const APNUM_int* t = a;
        a = b;
        b = t;
    }
    APNUM_Wigit e;
    for (u32 i = 0; i < b->digits->length; ++i)
    {
        APNUM_Wigit eb = b->digits->data[i];
        APNUM_Wigit carry = 0;
        for (u32 j = 0; j < a->digits->length; ++j)
        {
            APNUM_Wigit ea = a->digits->data[j];
            e = ea * eb + carry + sum->digits->data[i + j];
            carry = e / APNUM_Digit_Base;
            assert(carry < APNUM_Digit_Base);
            e = e % APNUM_Digit_Base;
            assert(e < APNUM_Digit_Base);
            sum->digits->data[i + j] = (APNUM_Digit)e;
        }
        sum->digits->data[i + a->digits->length] = (APNUM_Digit)carry;
    }
    APNUM_intTruncate(sum);
    sum->neg = a->neg ^ b->neg;
}











void APNUM_intMul(APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intMulLong(out, a, b);
}









// https://en.wikipedia.org/wiki/Division_algorithm




// https://en.wikipedia.org/wiki/Long_division

void APNUM_intDivLong(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
{
    APNUM_int* Q = outQ;
    APNUM_int* R = outR;
    vec_resize(Q->digits, 0);
    vec_resize(R->digits, 0);
    APNUM_int eb[1] = { D->digits[0] };
    int rel = APNUM_intCmpAbs(N, D);
    if (0 == rel)
    {
        vec_push(Q->digits, 1);
        goto out;
    }
    if (rel < 0)
    {
        APNUM_intDup(R, N);
        goto out;
    }
    assert(D->digits->length > 0);
    for (u32 i = 0; i < N->digits->length; ++i)
    {
        u32 j = N->digits->length - 1 - i;
        APNUM_intDightsInsertAt0(R, N->digits->data[j]);
        APNUM_Digit er = 0;
        for (;; ++er)
        {
            rel = APNUM_intCmpAbs(R, D);
            if (rel < 0)
            {
                break;
            }
            APNUM_intSubInP(R, eb);
        }
        APNUM_intDightsInsertAt0(Q, er);
    }
out:
    Q->neg = N->neg ^ D->neg;
    R->neg = N->neg;
}







// http://justinparrtech.com/JustinParr-Tech/an-algorithm-for-arbitrary-precision-integer-division/
// https://www.youtube.com/watch?v=6bpLYxk9TUQ

void APNUM_intDivSimple(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
{
    APNUM_int* Q = outQ;
    APNUM_int* R = outR;
    vec_resize(Q->digits, 0);
    vec_resize(R->digits, 0);
    int rel = APNUM_intCmpAbs(N, D);
    if (0 == rel)
    {
        vec_push(Q->digits, 1);
        goto out;
    }
    if (rel < 0)
    {
        APNUM_intDup(R, N);
        goto out;
    }

    assert(D->digits->length > 0);
    APNUM_Wigit d = vec_last(D->digits);

    APNUM_int N_abs[1] = { N->digits[0] };
    APNUM_int D_abs[1] = { D->digits[0] };

    APNUM_intDup(R, N_abs);
    APNUM_int* Q1 = APNUM_intZero();
    for (;;)
    {
        Q1->neg = R->neg;
        vec_resize(Q1->digits, 0);

        assert(R->digits->length >= D->digits->length);
        u32 l = R->digits->length - D->digits->length + 1;
        APNUM_Wigit r = 0;
        for (u32 i = 0; i < l; ++i)
        {
            u32 j = l - 1 - i;
            j = D->digits->length - 1 + j;
            r *= APNUM_Digit_Base;
            r += R->digits->data[j];
            if (r >= d)
            {
                APNUM_Wigit e = r / d;
                assert(e < APNUM_Digit_Base);
                APNUM_intDightsInsertAt0(Q1, (APNUM_Digit)e);
                r = r - e * d;
            }
            else
            {
                APNUM_intDightsInsertAt0(Q1, 0);
            }
        }
        APNUM_intAddInP(Q, Q1);

        APNUM_intMul(R, Q, D_abs);
        R->neg = true;

        APNUM_intAddInP(R, N_abs);
        if (APNUM_intCmpAbs(R, D) < 0)
        {
            if (R->neg)
            {
                Q1->neg = R->neg;
                vec_resize(Q1->digits, 1);
                Q1->digits->data[0] = 1;
                APNUM_intAddInP(Q, Q1);
                APNUM_intAddInP(R, D_abs);
            }
            break;
        }
    }
    APNUM_intFree(Q1);
out:
    Q->neg = N->neg ^ D->neg;
    R->neg = N->neg;
}












void APNUM_intDiv(APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
{
    assert(outQ != N);
    assert(outQ != D);
    assert(outR != N);
    assert(outR != D);
    APNUM_intDivSimple(outQ, outR, N, D);
}












int APNUM_intCmp(const APNUM_int* a, const APNUM_int* b)
{
    if (!a->neg && b->neg)
    {
        return 1;
    }
    else if (a->neg && !b->neg)
    {
        return -1;
    }
    int r = 1;
    if (a->neg && b->neg)
    {
        r = -1;
    }
    return APNUM_intCmpAbs(a, b) * r;
}













































































