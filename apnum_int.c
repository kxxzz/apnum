#include "apnum_a.h"





#define APNUM_StrChar_Base_MAX 35u



typedef u32 APNUM_Digit;
typedef u64 APNUM_Wigit;
static_assert(sizeof(APNUM_Digit) * 2 <= sizeof(APNUM_Wigit), "");


#define APNUM_Digit_Base (APNUM_Digit)-1
static_assert(APNUM_Digit_Base <= (APNUM_Digit)-1, "");


typedef vec_t(APNUM_Digit) APNUM_DigitVec;






typedef struct APNUM_int
{
    APNUM_DigitVec digits[1];
    bool neg;
} APNUM_int;

void APNUM_intFreeMem(APNUM_int* a)
{
    vec_free(a->digits);
    free(a);
}





APNUM_int* APNUM_intZero(APNUM_pool_t pool)
{
    if (pool->freeIntegers->length > 0)
    {
        APNUM_int* a = vec_last(pool->freeIntegers);
        vec_pop(pool->freeIntegers);

        vec_resize(a->digits, 0);
        a->neg = false;
        return a;
    }
    else
    {
        APNUM_int* a = zalloc(sizeof(*a));
        return a;
    }
}

void APNUM_intFree(APNUM_pool_t pool, APNUM_int* a)
{
    vec_push(pool->freeIntegers, a);
}















static void APNUM_intSwap(APNUM_int* a, APNUM_int* b)
{
    APNUM_int t = *a;
    *a = *b;
    *b = t;
}


static void APNUM_intDightsInsertAt0(APNUM_int* a, APNUM_Digit d)
{
    assert(d < APNUM_Digit_Base);
    if ((a->digits->length > 0) || d)
    {
        vec_insert(a->digits, 0, d);
    }
}


static APNUM_Digit APNUM_intDigitsDivDigit(APNUM_int* Q, u32 N_digitsLen, const APNUM_Digit* N_digits, APNUM_Digit D)
{
    vec_resize(Q->digits, 0);
    APNUM_Wigit R = 0;
    for (u32 i = 0; i < N_digitsLen; ++i)
    {
        u32 j = N_digitsLen - 1 - i;
        R *= APNUM_Digit_Base;
        R += N_digits[j];
        if (R >= D)
        {
            APNUM_Wigit e = R / D;
            assert(e < APNUM_Digit_Base);
            APNUM_intDightsInsertAt0(Q, (APNUM_Digit)e);
            R = R - e * D;
        }
        else
        {
            APNUM_intDightsInsertAt0(Q, 0);
        }
        assert(R < APNUM_Digit_Base);
    }
    return (APNUM_Digit)R;
}





static APNUM_Digit APNUM_intDivDigit(APNUM_int* Q, const APNUM_int* N, APNUM_Digit D)
{
    return APNUM_intDigitsDivDigit(Q, N->digits->length, N->digits->data, D);
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





















void APNUM_intDup(APNUM_int* out, const APNUM_int* a)
{
    vec_dup(out->digits, a->digits);
    out->neg = a->neg;
}

void APNUM_intNegation(APNUM_int* a)
{
    a->neg = !a->neg;
}

bool APNUM_intIsZero(APNUM_int* a)
{
    if (a->neg)
    {
        assert(a->digits->length > 0);
    }
    return 0 == a->digits->length;
}

bool APNUM_intIsNeg(APNUM_int* a)
{
    return a->neg;
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




















static void APNUM_intDigitsFromU32(APNUM_int* a, u32 u)
{
    vec_resize(a->digits, 0);
    while (u)
    {
        u32 r = u % APNUM_Digit_Base;
        vec_push(a->digits, r);
        u /= APNUM_Digit_Base;
    }
}


void APNUM_intFromU32(APNUM_int* out, u32 u)
{
    APNUM_intDigitsFromU32(out, u);
    out->neg = false;
}

void APNUM_intFromS32(APNUM_int* out, s32 i)
{
    u32 u = (i >= 0) ? i : -i;
    APNUM_intDigitsFromU32(out, u);
    out->neg = i < 0;
}























static u32 APNUM_intFromStrWithHead(APNUM_pool_t pool, APNUM_int* out, u32 base, const char* str, u32 headLen)
{
    assert(base <= APNUM_StrChar_Base_MAX);
    APNUM_int* a = out;
    vec_resize(a->digits, 0);

    bool neg = '-' == str[0];
    u32 sp = (('-' == str[0]) || ('+' == str[0])) ? 1 : 0;
    sp += headLen;
    u32 len = 0;
    for (u32 i = sp;; ++i)
    {
        APNUM_Digit d = APNUM_digitFromChar(str[i]);
        if (d > base)
        {
            len = i;
            break;
        }
    }
    if (sp == len)
    {
        return 0;
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

    APNUM_int* b = APNUM_intZero(pool);
    APNUM_int* a1 = APNUM_intZero(pool);

    b->neg = a->neg;
    APNUM_intDigitsFromU32(b, APNUM_digitFromChar(str[sp]));
    APNUM_intAddInP(pool, a, b);

    for (u32 i = sp + 1; i < len; ++i)
    {
        b->neg = false;
        APNUM_intDigitsFromU32(b, base);
        APNUM_intMul(pool, a1, a, b);
        APNUM_intSwap(a, a1);

        b->neg = a->neg;
        APNUM_intDigitsFromU32(b, APNUM_digitFromChar(str[i]));
        APNUM_intAddInP(pool, a, b);
    }

    APNUM_intFree(pool, a1);
    APNUM_intFree(pool, b);
    return len;
}







static u32 APNUM_intToStrWithHead
(
    APNUM_pool_t pool, const APNUM_int* a, u32 base, char* strBuf, u32 strBufSize, u32 headLen, const char* head
)
{
    static const char charTable[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    assert(base <= APNUM_StrChar_Base_MAX);
    if (0 == a->digits->length)
    {
        strBuf[0] = '0';
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

    APNUM_int* q = APNUM_intZero(pool);
    APNUM_intDup(q, a);
    q->neg = false;

    APNUM_int* q1 = APNUM_intZero(pool);
    APNUM_int* r = APNUM_intZero(pool);
    APNUM_int* ibase = APNUM_intZero(pool);
    APNUM_intDigitsFromU32(ibase, base);

    vec_char buf[1] = { 0 };
    do 
    {
        APNUM_intDiv(pool, q1, r, q, ibase);
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

    APNUM_intFree(pool, ibase);
    APNUM_intFree(pool, r);
    APNUM_intFree(pool, q1);
    APNUM_intFree(pool, q);

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
out:
    vec_free(buf);
    return len;
}









u32 APNUM_intFromStr(APNUM_pool_t pool, APNUM_int* out, u32 base, const char* str)
{
    return APNUM_intFromStrWithHead(pool, out, base, str, 0);
}


u32 APNUM_intToStr(APNUM_pool_t pool, const APNUM_int* a, u32 base, char* strBuf, u32 strBufSize)
{
    return APNUM_intToStrWithHead(pool, a, base, strBuf, strBufSize, 0, NULL);
}


u32 APNUM_intFromStrWithBaseFmt(APNUM_pool_t pool, APNUM_int* out, const char* str)
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
    return APNUM_intFromStrWithHead(pool, out, base, str, headLen);
}


u32 APNUM_intToStrWithBaseFmt
(
    APNUM_pool_t pool, const APNUM_int* a, APNUM_int_StrBaseFmtType baseFmt, char* strBuf, u32 strBufSize
)
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
    return APNUM_intToStrWithHead(pool, a, base, strBuf, strBufSize, headLen, head);
}





























void APNUM_intAddInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b)
{
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

    u32 len = max(a->digits->length, b->digits->length);

    if (a->neg == b->neg)
    {
        vec_reserve(a->digits, len + 1);

        bool carry = false;
        for (u32 i = 0; i < len; ++i)
        {
            APNUM_Wigit ea = (i < a->digits->length) ? a->digits->data[i] : 0;
            APNUM_Wigit eb = (i < b->digits->length) ? b->digits->data[i] : 0;
            APNUM_Wigit e = ea + eb + (carry ? 1 : 0);
            if (e >= APNUM_Digit_Base)
            {
                e -= APNUM_Digit_Base;
                carry = true;
            }
            else
            {
                carry = false;
            }
            assert(e < APNUM_Digit_Base);
            a->digits->data[i] = (APNUM_Digit)e;
        }
        if (carry)
        {
            vec_resize(a->digits, len + 1);
            a->digits->data[len] = 1;
        }
        else
        {
            vec_resize(a->digits, len);
        }
    }
    else
    {
        vec_reserve(a->digits, len);

        const APNUM_int *m, *s;
        if (APNUM_intCmpAbs(a, b) >= 0)
        {
            m = a;
            s = b;
        }
        else
        {
            m = b;
            s = a;
        }
        bool carry = false;
        for (u32 i = 0; i < len; ++i)
        {
            APNUM_Wigit em = (i < m->digits->length) ? m->digits->data[i] : 0;
            APNUM_Wigit es = (i < s->digits->length) ? s->digits->data[i] : 0;
            es += carry ? 1 : 0;
            if (em < es)
            {
                em += APNUM_Digit_Base;
                carry = true;
            }
            else
            {
                carry = false;
            }
            assert(em >= es);
            APNUM_Wigit ed = em - es;
            assert(ed < APNUM_Digit_Base);
            a->digits->data[i] = (APNUM_Digit)ed;
        }
        assert(!carry);
        vec_resize(a->digits, len);
    }
    a->neg = outNeg;
    APNUM_intTruncate(a);
}


void APNUM_intSubInP(APNUM_pool_t pool, APNUM_int* a, const APNUM_int* b)
{
    APNUM_int negb[1] = { 0 };
    negb->digits[0] = b->digits[0];
    negb->neg = !b->neg;
    APNUM_intAddInP(pool, a, negb);
}













void APNUM_intAdd(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intAddInP(pool, out, b);
}


void APNUM_intSub(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intDup(out, a);
    APNUM_intSubInP(pool, out, b);
}









// https://en.wikipedia.org/wiki/Multiplication_algorithm



// Long multiplication

static void APNUM_intMulLong(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
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











void APNUM_intMul(APNUM_pool_t pool, APNUM_int* out, const APNUM_int* a, const APNUM_int* b)
{
    assert(out != a);
    assert(out != b);
    APNUM_intMulLong(pool, out, a, b);
}









// https://en.wikipedia.org/wiki/Division_algorithm




// https://en.wikipedia.org/wiki/Long_division

void APNUM_intDivLong(APNUM_pool_t pool, APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
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
            APNUM_intSubInP(pool, R, eb);
        }
        APNUM_intDightsInsertAt0(Q, er);
    }
out:
    Q->neg = N->neg ^ D->neg;
    R->neg = N->neg;
}







// http://justinparrtech.com/JustinParr-Tech/an-algorithm-for-arbitrary-precision-integer-division/
// https://www.youtube.com/watch?v=6bpLYxk9TUQ

void APNUM_intDivSimple(APNUM_pool_t pool, APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
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
    APNUM_Digit A = vec_last(D->digits);

    APNUM_int N_abs[1] = { N->digits[0] };
    APNUM_int D_abs[1] = { D->digits[0] };

    APNUM_intDup(R, N_abs);
    APNUM_int* RA = APNUM_intZero(pool);
    for (;;)
    {
        RA->neg = R->neg;
        assert(R->digits->length >= D->digits->length);
        u32 R_digitsLen = R->digits->length - D->digits->length + 1;
        const APNUM_Digit* R_digits = R->digits->data + D->digits->length - 1;
        APNUM_intDigitsDivDigit(RA, R_digitsLen, R_digits, A);

        APNUM_intAddInP(pool, Q, RA);

        APNUM_intMul(pool, R, Q, D_abs);
        R->neg = true;

        APNUM_intAddInP(pool, R, N_abs);
        if (APNUM_intCmpAbs(R, D) < 0)
        {
            if (R->neg)
            {
                RA->neg = true;
                APNUM_intDigitsFromU32(RA, 1);
                APNUM_intAddInP(pool, Q, RA);
                APNUM_intAddInP(pool, R, D_abs);
            }
            break;
        }
    }
    APNUM_intFree(pool, RA);
out:
    Q->neg = N->neg ^ D->neg;
    R->neg = N->neg;
}












void APNUM_intDiv(APNUM_pool_t pool, APNUM_int* outQ, APNUM_int* outR, const APNUM_int* N, const APNUM_int* D)
{
    assert(outQ != N);
    assert(outQ != D);
    assert(outR != N);
    assert(outR != D);
    APNUM_intDivSimple(pool, outQ, outR, N, D);
}



























































































































































































