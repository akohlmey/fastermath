/* faster versions of 2**x in single, double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

/* optimizer friendly implementation of exp2(x).
 *
 * strategy:
 *
 * split argument into an integer part and a fraction:
 * ipart = floor(x)+0.5;
 * fpart = x - ipart;
 *
 * compute exp2(ipart) from setting the ieee754 exponent
 * compute exp2(fpart) using a pade' approximation for x in [-0.5;0.5[
 *
 * the result becomes: exp2(x) = exp2(ipart) * exp2(fpart)
 */
static const double fm_exp2_p[] = {
    2.30933477057345225087e-2,
    2.02020656693165307700e1,
    1.51390680115615096133e3
};
static const double fm_exp2_q[] = {
/*  1.00000000000000000000e0, */
    2.33184211722314911771e2,
    4.36821166879210612817e3
};

double fm_exp2(double x)
{
    double   ipart, fpart, xx, px, qx;
    udi_t    epart;

    /* handle exceptional arguments */
    if (x == 0.0) 
        return 1.0;
    else if (x <= -1023.0)
        return 0.0;
    else if (x >= 1024.0)
        return FM_INFINITY;

    ipart = __builtin_floor(x + 0.5);
    fpart = x - ipart;
    epart.f = 0.0;
    epart.b.e = (unsigned int) (ipart + FM_DOUBLE_BIAS);

    xx = fpart*fpart;

    px =         fm_exp2_p[0];
    px = px*xx + fm_exp2_p[1];
    qx =    xx + fm_exp2_q[0];
    px = px*xx + fm_exp2_p[2];
    qx = qx*xx + fm_exp2_q[1];
    px = px * fpart;

    xx = 1.0 + 2.0*(px/(qx-px));
    return epart.f*xx;
}

/* optimizer friendly implementation of exp2f(x).
 *
 * strategy:
 *
 * split argument into an integer part and a fraction:
 * ipart = floor(x)+0.5;
 * fpart = x - ipart;
 *
 * compute exp2(ipart) from setting the ieee754 exponent
 * compute exp2(fpart) using a pade' approximation for x in [-0.5;0.5[
 *
 * the result becomes: exp2(x) = exp2(ipart) * exp2(fpart)
 */
static const float fm_exp2f_p[] = {
    2.30933477057345225087e-2f,
    2.02020656693165307700e1f,
    1.51390680115615096133e3f
};
static const float fm_exp2f_q[] = {
/*  1.00000000000000000000e0, */
    2.33184211722314911771e2,
    4.36821166879210612817e3
};

float fm_exp2f(float x)
{
    float ipart, fpart, xx, px, qx;
    ufi_t epart;

    /* handle exceptional arguments */
    if (x == 0.0f) 
        return 1.0f;
    else if (x <= -126.0f)
        return 0.0;
    else if (x >= 127.0)
        return FM_INFINITYF;

    ipart = __builtin_floorf(x + 0.5f);
    fpart = x - ipart;
    epart.f = 0.0f;
    epart.b.e = (unsigned int) (ipart + FM_FLOAT_BIAS);

    xx = fpart*fpart;

    px =         fm_exp2f_p[0];
    px = px*xx + fm_exp2f_p[1];
    qx =    xx + fm_exp2f_q[0];
    px = px*xx + fm_exp2f_p[2];
    qx = qx*xx + fm_exp2f_q[1];
    px = px * fpart;

    xx = 1.0f + 2.0f*(px/(qx-px));
    return epart.f*xx;
}

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make"
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
