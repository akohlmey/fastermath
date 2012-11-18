/* faster versions of e**x in single, double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

#include <stdio.h>

/* optimizer friendly implementation of exp(x).
 *
 * strategy:
 *
 * transform e**x into an expression 2**(ipart) * e**fpart
 * compute 2**ipart by setting the exponent and approximate
 * e**fpart from a pade style rational function.
 */

static const double fm_exp_p[] = {
    1.26177193074810590878e-4,
    3.02994407707441961300e-2,
    9.99999999999999999910e-1,
};

static const double fm_exp_q[] = {
    3.00198505138664455042e-6,
    2.52448340349684104192e-3,
    2.27265548208155028766e-1,
    2.00000000000000000009e0,
};

static const double fm_exp_max =  7.08396418532264106224E2;  /* log 2**1022 */
static const double fm_exp_min = -7.08396418532264106224E2;  /* log 2**-1022 */

double fm_exp(double x)
{
    double   ipart, fpart, xx, px, qx;
    udi_t    epart;

    /* handle exceptional arguments */
    if (x == 0.0) 
        return 1.0;
    else if (x <= fm_exp_min)
        return 0.0;
    else if (x >= fm_exp_max)
        return FM_INFINITY;
    else if (__builtin_isnan(x))
        return x;

    ipart  = __builtin_floor(FM_1BYLN2*x + 0.5);
    fpart  = x - ipart*FM_LN2;

    epart.f = 0.0;
    epart.b.e = (unsigned int) (ipart + FM_DOUBLE_BIAS);

    xx = fpart*fpart;

    px =         fm_exp_p[0];
    qx =         fm_exp_q[0];
    px = px*xx + fm_exp_p[1];
    qx = qx*xx + fm_exp_q[1];
    px = px*xx + fm_exp_p[2];
    qx = qx*xx + fm_exp_q[2];
    px = px*fpart;
    qx = qx*xx + fm_exp_q[3];
    
    xx = 1.0 + 2.0*px/(qx-px);
    return epart.f*xx;
}

/* optimizer friendly implementation of exp(x).
 *
 * strategy:
 *
 * transform e**x into an expression 2**(ipart) * e**fpart
 * compute 2**ipart by setting the exponent and approximate
 * e**fpart from a pade style rational function.
 */

static const float fm_expf_p[] = {
    1.26177193074810590878e-4f,
    3.02994407707441961300e-2f,
    9.99999999999999999910e-1f,
};

static const float fm_expf_q[] = {
    3.00198505138664455042e-6f,
    2.52448340349684104192e-3f,
    2.27265548208155028766e-1f,
    2.00000000000000000009e0f,
};
static const float fm_expf_max =  88.02969187150841f;  /* ln(2^127) */
static const float fm_expf_min = -88.7228391116729996; /* log(2^-128) */

float fm_expf(float x)
{
    float   ipart, fpart, xx, px, qx;
    ufi_t    epart;

    /* handle exceptional arguments */
    if (x == 0.0f) 
        return 1.0f;
    else if (x <= fm_expf_min)
        return 0.0f;
    else if (x >= fm_expf_max)
        return FM_INFINITYF;
    else if (__builtin_isnanf(x))
        return x;

    ipart  = __builtin_floorf(FM_1BYLN2F*x + 0.5f);
    fpart  = x - ipart*FM_LN2F;

    epart.f = 0.0f;
    epart.b.e = (unsigned int) (ipart + FM_FLOAT_BIAS);

    xx = fpart*fpart;

    px =         fm_expf_p[0];
    qx =         fm_expf_q[0];
    px = px*xx + fm_expf_p[1];
    qx = qx*xx + fm_expf_q[1];
    px = px*xx + fm_expf_p[2];
    qx = qx*xx + fm_expf_q[2];
    px = px*fpart;
    qx = qx*xx + fm_expf_q[3];
    
    xx = 1.0f + 2.0f*px/(qx-px);
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
