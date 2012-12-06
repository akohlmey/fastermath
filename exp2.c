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

static const unsigned char fm_exp2_c[48] __attribute__ ((aligned(16))) = {
    0xd3, 0xea, 0x9a, 0x54, 0xc8, 0xa5, 0x97, 0x3f, /* 2.30933477057345225087e-2, */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f, /* 1.00000000000000000000e0,  */
    0xde, 0x5b, 0x61, 0x93, 0xba, 0x33, 0x34, 0x40, /* 2.02020656693165307700e1,  */
    0x3c, 0x5c, 0xfb, 0x0f, 0xe5, 0x25, 0x6d, 0x40, /* 2.33184211722314911771e2,  */
    0x93, 0x76, 0x7b, 0x90, 0xa0, 0xa7, 0x97, 0x40, /* 1.51390680115615096133e3,  */
    0xae, 0x0b, 0xed, 0x2f, 0x36, 0x10, 0xb1, 0x40  /* 4.36821166879210612817e3   */
};
static const double fm_exp2_f[6] __attribute__ ((alias("fm_exp2_c")));

double fm_exp2(const double x)
{
    double   ipart, fpart, xx, px, qx;
    udi_t    epart;

    ipart = __builtin_floor(x + 0.5);
    fpart = x - ipart;
    epart.f = 0.0;
    epart.b.e = ((unsigned int) ipart) + FM_DOUBLE_BIAS;

    xx = fpart*fpart;

    px =         fm_exp2_f[0];
    qx =         fm_exp2_f[1];
    px = px*xx + fm_exp2_f[2];
    qx = qx*xx + fm_exp2_f[3];
    px = px*xx + fm_exp2_f[4];
    qx = qx*xx + fm_exp2_f[5];

    px = px * fpart;

    xx = 1.0 + 2.0*(px/(qx-px));
    return epart.f*xx;
}


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

static const unsigned char fm_exp2f_c[24] __attribute__ ((aligned(16))) = {
    0xde, 0xfd, 0x20, 0x39, /* 1.535336188319500e-004, */
    0x29, 0x9f, 0xaf, 0x3a, /* 1.339887440266574e-003, */
    0xa6, 0x96, 0x1d, 0x3c, /* 9.618437357674640e-003, */
    0x74, 0x57, 0x63, 0x3d, /* 5.550332471162809e-002, */
    0xee, 0xfd, 0x75, 0x3e, /* 2.402264791363012e-001, */
    0x18, 0x72, 0x31, 0x3f  /* 6.931472028550421e-001  */
};
static const float fm_exp2f_f[6] __attribute__ ((alias("fm_exp2f_c")));

float fm_exp2f(const float x)
{
    float ipart, fpart, xx, px;
    ufi_t epart;

    ipart = __builtin_floorf(x + 0.5f);
    fpart = x - ipart;
    epart.u = 0U;
    epart.b.e = ((unsigned int) ipart) + FM_FLOAT_BIAS;

    px =            fm_exp2f_f[0];
    px = px*fpart + fm_exp2f_f[1];
    px = px*fpart + fm_exp2f_f[2];
    px = px*fpart + fm_exp2f_f[3];
    px = px*fpart + fm_exp2f_f[4];
    px = px*fpart + fm_exp2f_f[5];
    xx = px*fpart + 1.0f;

    return epart.f*xx;
}

#if defined(LIBM_ALIAS) || 1
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
double exp2(const double x) __attribute__ ((alias("fm_exp2")));
float exp2f(const float x) __attribute__ ((alias("fm_exp2f")));
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
