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
static const double fm_exp2_r[] __attribute__ ((aligned(16))) = {
    2.30933477057345225087e-2,
    1.00000000000000000000e0,
    2.02020656693165307700e1,
    2.33184211722314911771e2,
    1.51390680115615096133e3,
    4.36821166879210612817e3
};

double fm_exp2(const double x)
{
    double   ipart, fpart, xx, px, qx;
    udi_t    epart;

    ipart = __builtin_floor(x + 0.5);
    fpart = x - ipart;
    epart.f = 0.0;
    epart.b.e = ((unsigned int) ipart) + FM_DOUBLE_BIAS;

    xx = fpart*fpart;

    px =         fm_exp2_r[0];
    qx =         fm_exp2_r[1];
    px = px*xx + fm_exp2_r[2];
    qx = qx*xx + fm_exp2_r[3];
    px = px*xx + fm_exp2_r[4];
    qx = qx*xx + fm_exp2_r[5];

    px = px * fpart;

    xx = 1.0 + 2.0*(px/(qx-px));
    return epart.f*xx;
}


#if 0
/* optimizer friendly implementation of exp2f(x).
 *
 * strategy:
 *
 * split argument into an integer part and a fraction:
 * ipart = floorf(x)+0.5;
 * fpart = x - ipart;
 *
 * compute exp2f(ipart) from setting the ieee754 exponent
 * compute exp2f(fpart) using a pade' approximation for x in [-0.5;0.5[
 *
 * the result becomes: exp2f(x) = exp2f(ipart) * exp2f(fpart)
 */
static const float fm_exp2f_r[] __attribute__ ((aligned(16))) = {
    2.30933477057345225087e-2,
    1.00000000000000000000e0,
    2.02020656693165307700e1,
    2.33184211722314911771e2,
    1.51390680115615096133e3,
    4.36821166879210612817e3
};


float fm_exp2f(const float x)
{
    float ipart, fpart, xx, px, qx;
    ufi_t epart;

    ipart = __builtin_floorf(x + 0.5f);
    fpart = x - ipart;
    epart.u = 0U;
    epart.b.e = ((unsigned int) ipart) + FM_FLOAT_BIAS;

    xx = fpart*fpart;

    px =         fm_exp2f_r[0];
    qx =         fm_exp2f_r[1];
    px = px*xx + fm_exp2f_r[2];
    qx = qx*xx + fm_exp2f_r[3];
    px = px*xx + fm_exp2f_r[4];
    qx = qx*xx + fm_exp2f_r[5];

    px = px * fpart;

    xx = 1.0f + 2.0f*(px/(qx-px));
    return epart.f*xx;
}

#else

/* optimizer friendly implementation of exp2f(x).
 *
 * strategy:
 *
 * split argument into an integer part and a fraction:
 * ipart = floorf(x)+0.5;
 * fpart = x - ipart;
 *
 * compute exp2f(ipart) from setting the ieee754 exponent
 * compute exp2f(fpart) using a polynomial approximation for x in [-0.5;0.5[
 *
 * the result becomes: exp2f(x) = exp2f(ipart) * exp2f(fpart)
 */

static const float fm_exp2f_p[] __attribute__ ((aligned(16))) = {
    1.535336188319500e-004,
    1.339887440266574e-003,
    9.618437357674640e-003,
    5.550332471162809e-002,
    2.402264791363012e-001,
    6.931472028550421e-001
};

float fm_exp2f(const float x)
{
    float ipart, fpart, xx, px, qx;
    ufi_t epart;

    ipart = __builtin_floorf(x + 0.5f);
    fpart = x - ipart;
    epart.u = 0U;
    epart.b.e = ((unsigned int) ipart) + FM_FLOAT_BIAS;

    px =            fm_exp2f_p[0];
    px = px*fpart + fm_exp2f_p[1];
    px = px*fpart + fm_exp2f_p[2];
    px = px*fpart + fm_exp2f_p[3];
    px = px*fpart + fm_exp2f_p[4];
    px = px*fpart + fm_exp2f_p[5];
    xx = px*fpart + 1.0f;

    return epart.f*xx;
}
#endif

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make"
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
