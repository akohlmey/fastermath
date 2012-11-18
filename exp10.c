/* faster versions of 10**x in single, double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

#include <stdio.h>

/* optimizer friendly implementation of exp10(x).
 *
 * strategy:
 *
 * transform 10**x into an expression 2**(ipart) * 10**fpart
 * compute 2**ipart by setting the exponent and approximate
 * 10**fpart from a pade style rational function.
 */

static const double fm_exp10_p[] = {
    4.09962519798587023075e-2,
    1.17452732554344059015e1,
    4.06717289936872725516e2,
    2.39423741207388267439e3,
};

static const double fm_exp10_q[] = {
 /* 1.00000000000000000000E0,*/
    8.50936160849306532625e1,
    1.27209271178345121210e3,
    2.07960819286001865907e3,
};

static const double fm_exp10_max = 308.2547155599167;

double fm_exp10(double x)
{
    double   ipart, fpart, xx, px, qx;
    udi_t    epart;

    /* handle exceptional arguments */
    if (x == 0.0) 
        return 1.0;
    else if (x <= -fm_exp10_max)
        return 0.0;
    else if (x >= fm_exp10_max)
        return FM_INFINITY;
    else if (__builtin_isnan(x))
        return x;

    ipart  = __builtin_floor(FM_LOG2_10*x + 0.5);
    fpart  = x - ipart*FM_LOG10_2;

    epart.f = 0.0;
    epart.b.e = (unsigned int) (ipart + FM_DOUBLE_BIAS);

    xx = fpart*fpart;

    px =         fm_exp10_p[0];
    qx =    xx + fm_exp10_q[0];
    px = px*xx + fm_exp10_p[1];
    qx = qx*xx + fm_exp10_q[1];
    px = px*xx + fm_exp10_p[2];
    qx = qx*xx + fm_exp10_q[2];
    px = px*xx + fm_exp10_p[3];
    px = px*fpart;
    
    xx = 1.0 + 2.0*px/(qx-px);
    return epart.f*xx;
}


/* optimizer friendly implementation of exp10f(x).
 *
 * strategy:
 *
 * transform 10**x into an expression 2**(ipart) * 10**fpart
 * compute 2**ipart by setting the exponent and approximate
 * 10**fpart from a pade style rational function.
 */

static const float fm_exp10f_p[] = {
    4.09962519798587023075e-2f,
    1.17452732554344059015e1f,
    4.06717289936872725516e2f,
    2.39423741207388267439e3f,
};

static const float fm_exp10f_q[] = {
 /* 1.00000000000000000000e0f,*/
    8.50936160849306532625e1f,
    1.27209271178345121210e3f,
    2.07960819286001865907e3f,
};

static const float fm_exp10f_max = 38.230809449325611792f;

float fm_exp10f(float x)
{
    float   ipart, fpart, xx, px, qx;
    ufi_t    epart;

    /* handle exceptional arguments */
    if (x == 0.0f) 
        return 1.0f;
    else if (x <= -fm_exp10f_max)
        return 0.0f;
    else if (x >= fm_exp10f_max)
        return FM_INFINITY;
    else if (__builtin_isnanf(x))
        return x;

    ipart  = __builtin_floorf(FM_LOG2_10F*x + 0.5f);
    fpart  = x - ipart*FM_LOG10_2F;

    epart.f = 0.0f;
    epart.b.e = (unsigned int) (ipart + FM_FLOAT_BIAS);

    xx = fpart*fpart;

    px =         fm_exp10f_p[0];
    qx =    xx + fm_exp10f_q[0];
    px = px*xx + fm_exp10f_p[1];
    qx = qx*xx + fm_exp10f_q[1];
    px = px*xx + fm_exp10f_p[2];
    qx = qx*xx + fm_exp10f_q[2];
    px = px*xx + fm_exp10f_p[3];
    px = px*fpart;
    
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
