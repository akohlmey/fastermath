/* alternate faster versions of e**x and 10**x in single, double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

/* optimizer friendly implementation of exp(x).
 *
 * strategy:
 *
 * transform e**x into an expression 2**(ipart) * e**fpart
 * compute 2**ipart by setting the exponent and approximate
 * e**fpart from a pade style rational function.
 */

static const double fm_exp_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
    1.26177193074810590878e-4,
    3.02994407707441961300e-2,
    9.99999999999999999910e-1,
};

static const double fm_exp_q[] __attribute__ ((aligned(_FM_ALIGN))) = {
    3.00198505138664455042e-6,
    2.52448340349684104192e-3,
    2.27265548208155028766e-1,
    2.00000000000000000009e0,
};

static const double fm_exp_c1 = 6.93145751953125e-1;
static const double fm_exp_c2 = 1.42860682030941723212e-6;

double fm_exp_alt(double x)
{
    double   ipart, xx, px, qx;
    udi_t    epart;

    ipart  = __builtin_floor(FM_DOUBLE_LOG2OFE*x + 0.5);
    x -= ipart*fm_exp_c1;
    x -= ipart*fm_exp_c2;
    FM_DOUBLE_INIT_EXP(epart,ipart);

    xx = x*x;

    px =         fm_exp_p[0];
    qx =         fm_exp_q[0];
    px = px*xx + fm_exp_p[1];
    qx = qx*xx + fm_exp_q[1];
    px = px*xx + fm_exp_p[2];
    qx = qx*xx + fm_exp_q[2];
    px = px*x;
    qx = qx*xx + fm_exp_q[3];
    
    x = px/(qx-px);
    x = 1.0 + 2.0*x;

    return epart.f*x;
}

/* optimizer friendly implementation of expf(x).
 *
 * strategy:
 *
 * transform e**x into an expression 2**(ipart) * e**fpart
 * compute 2**ipart by setting the exponent and approximate
 * e**fpart from a polynomial expansion.
 */

static const float fm_expf_p[]  __attribute__ ((aligned(_FM_ALIGN))) = {
    1.9875691500e-4f,
    1.3981999507e-3f,
    8.3334519073e-3f,
    4.1665795894e-2f,
    1.6666665459e-1f,
    5.0000001201e-1f
};

static const float fm_expf_c1 =  0.693359375f;
static const float fm_expf_c2 = -2.12194440e-4f;

float fm_expf_alt(float x)
{
    float ipart, px;
    ufi_t epart;

    ipart  = __builtin_floorf(FM_FLOAT_LOG2OFE*x + 0.5f);
    x -= ipart*fm_expf_c1;
    x -= ipart*fm_expf_c2;
    FM_FLOAT_INIT_EXP(epart,ipart);

    px =         fm_expf_p[0];
    px = px*x  + fm_expf_p[1];
    px = px*x  + fm_expf_p[2];
    px = px*x  + fm_expf_p[3];
    px = px*x  + fm_expf_p[4];
    px = px*x  + fm_expf_p[5];
    px = px*x*x + x +1.0f;

    return epart.f*px;
}

/* optimizer friendly implementation of exp10(x).
 *
 * strategy:
 *
 * transform 10**x into an expression 2**(ipart) * 10**fpart
 * compute 2**ipart by setting the exponent and approximate
 * 10**fpart from a pade style rational function.
 */

static const double fm_exp10_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
    4.09962519798587023075e-2,
    1.17452732554344059015e1,
    4.06717289936872725516e2,
    2.39423741207388267439e3
};

static const double fm_exp10_q[] __attribute__ ((aligned(_FM_ALIGN))) = {
    1.00000000000000000000e0,
    8.50936160849306532625e1,
    1.27209271178345121210e3,
    2.07960819286001865907e3
};

static const double fm_exp10_c1 = 3.01025390625000000000e-1;
static const double fm_exp10_c2 = 4.60503898119521373889e-6;

double fm_exp10_alt(double x)
{
    double   ipart, xx, px, qx;
    udi_t    epart;

    ipart  = __builtin_floor(FM_DOUBLE_LOG2OF10*x + 0.5);
    x -= ipart*fm_exp10_c1;
    x -= ipart*fm_exp10_c2;
    FM_DOUBLE_INIT_EXP(epart,ipart);

    xx = x*x;

    px =         fm_exp10_p[0];
    qx =         fm_exp10_q[0];
    px = px*xx + fm_exp10_p[1];
    qx = qx*xx + fm_exp10_q[1];
    px = px*xx + fm_exp10_p[2];
    qx = qx*xx + fm_exp10_q[2];
    px = px*xx + fm_exp10_p[3];
    qx = qx*xx + fm_exp10_q[3];
    px *= x;
    
    x = px/(qx-px);
    x = 1.0 + 2.0*x;

    return epart.f*x;
}

/* optimizer friendly implementation of exp10f(x).
 *
 * strategy:
 *
 * transform 10**x into an expression 2**(ipart) * 10**fpart
 * compute 2**ipart by setting the exponent and approximate
 * 10**fpart from a polynomial expansion.
 */

static const float fm_exp10f_p[]  __attribute__ ((aligned(_FM_ALIGN))) = {
 0.2063216740311022f,
 0.5420251702225484f,
 1.171292686296281f,
 2.034649854009453f,
 2.650948748208892f,
 2.302585167056758f
};

static const float fm_exp10f_c1 = 0.30078125000000000000f;
static const float fm_exp10f_c2 = 2.48745663981195213739e-4f;

float fm_exp10f_alt(float x)
{
    float ipart, px;
    ufi_t epart;

    ipart  = __builtin_floorf(FM_FLOAT_LOG2OF10*x + 0.5f);
    x -= ipart*fm_exp10f_c1;
    x -= ipart*fm_exp10f_c2;
    FM_FLOAT_INIT_EXP(epart,ipart);

    px =         fm_exp10f_p[0];
    px = px*x  + fm_exp10f_p[1];
    px = px*x  + fm_exp10f_p[2];
    px = px*x  + fm_exp10f_p[3];
    px = px*x  + fm_exp10f_p[4];
    px = px*x  + fm_exp10f_p[5];
    px = px*x  + 1.0f;

    return epart.f*px;
}

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make -C .."
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
