/* faster versions of logarithms in single and double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

/* optimizer friendly implementation of log2(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log_2(fpart) with a rational function in [1.0:2.0[
 */

static const double fm_log2_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
    1.01875663804580931796e-4,
    4.97494994976747001425e-1,
    4.70579119878881725854e0,
    1.44989225341610930846e1,
    1.79368678507819816313e1,
    7.70838733755885391666e0
};

static const double fm_log2_q[] __attribute__ ((aligned(_FM_ALIGN))) = {
/*  1.00000000000000000000e0, */
    1.12873587189167450590e1,
    4.52279145837532221105e1,
    8.29875266912776603211e1,
    7.11544750618563894466e1,
    2.31251620126765340583e1
};

double fm_log2(double x) 
{
    udi_t val;
    double z,px,qx;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i1 & FM_DOUBLE_MMASK;
    ipart = val.i1 & FM_DOUBLE_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_DOUBLE_EZERO;
    val.i1 = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= 20;
    ipart -= FM_DOUBLE_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_DOUBLE_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2_p[0];
    px = px*x + fm_log2_p[1];
    px = px*x + fm_log2_p[2];
    px = px*x + fm_log2_p[3];
    px = px*x + fm_log2_p[4];
    px = px*x + fm_log2_p[5];

    qx =    x + fm_log2_q[0];
    qx = qx*x + fm_log2_q[1];
    qx = qx*x + fm_log2_q[2];
    qx = qx*x + fm_log2_q[3];
    qx = qx*x + fm_log2_q[4];

    z = x*x;
    z = x*(z*px/qx) - 0.5*z + x;
    z *= FM_DOUBLE_LOG2OFE;
    return ((double)ipart) + z;
}

double fm_log(double x) 
{
    udi_t val;
    double z,px,qx;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i1 & FM_DOUBLE_MMASK;
    ipart = val.i1 & FM_DOUBLE_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_DOUBLE_EZERO;
    val.i1 = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= FM_DOUBLE_MBITS;
    ipart -= FM_DOUBLE_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_DOUBLE_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2_p[0];
    px = px*x + fm_log2_p[1];
    px = px*x + fm_log2_p[2];
    px = px*x + fm_log2_p[3];
    px = px*x + fm_log2_p[4];
    px = px*x + fm_log2_p[5];

    qx =    x + fm_log2_q[0];
    qx = qx*x + fm_log2_q[1];
    qx = qx*x + fm_log2_q[2];
    qx = qx*x + fm_log2_q[3];
    qx = qx*x + fm_log2_q[4];

    z = x*x;
    z = x*(z*px/qx) - 0.5*z + x;
    z += ((double)ipart)*FM_DOUBLE_LOGEOF2;
    return z;
}


double fm_log10(double x) 
{
    udi_t val;
    double z,px,qx;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i1 & FM_DOUBLE_MMASK;
    ipart = val.i1 & FM_DOUBLE_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_DOUBLE_EZERO;
    val.i1 = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= FM_DOUBLE_MBITS;
    ipart -= FM_DOUBLE_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_DOUBLE_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2_p[0];
    px = px*x + fm_log2_p[1];
    px = px*x + fm_log2_p[2];
    px = px*x + fm_log2_p[3];
    px = px*x + fm_log2_p[4];
    px = px*x + fm_log2_p[5];

    qx =    x + fm_log2_q[0];
    qx = qx*x + fm_log2_q[1];
    qx = qx*x + fm_log2_q[2];
    qx = qx*x + fm_log2_q[3];
    qx = qx*x + fm_log2_q[4];

    z = x*x;
    z = x*(z*px/qx) - 0.5*z + x;
    z *= FM_DOUBLE_LOG10OFE;
    z += ((double)ipart)*FM_DOUBLE_LOG10OF2;
    return z;
}


/* optimizer friendly implementation of log2f(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log_2(fpart) with a rational function in [1.0:2.0[
 */

static const float fm_log2f_p[] __attribute__ ((aligned(_FM_ALIGN))) = {
    7.0376836292e-2,
   -1.1514610310e-1,
    1.1676998740e-1,
   -1.2420140846e-1,
    1.4249322787e-1,
   -1.6668057665e-1,
    2.0000714765e-1,
   -2.4999993993e-1,
    3.3333331174e-1
};

float fm_log2f(float x) 
{
    ufi_t val;
    float z,px;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i & FM_FLOAT_MMASK;
    ipart = val.i & FM_FLOAT_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_FLOAT_EZERO;
    val.i = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= FM_FLOAT_MBITS;
    ipart -= FM_FLOAT_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_FLOAT_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2f_p[0];
    px = px*x + fm_log2f_p[1];
    px = px*x + fm_log2f_p[2];
    px = px*x + fm_log2f_p[3];
    px = px*x + fm_log2f_p[4];
    px = px*x + fm_log2f_p[5];
    px = px*x + fm_log2f_p[6];
    px = px*x + fm_log2f_p[7];
    px = px*x + fm_log2f_p[8];

    z = x*x;
    z = x*z*px - 0.5*z + x;
    z *= FM_FLOAT_LOG2OFE;
    return ((float)ipart) + z;
}

float fm_logf(float x) 
{
    ufi_t val;
    float z,px;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i & FM_FLOAT_MMASK;
    ipart = val.i & FM_FLOAT_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_FLOAT_EZERO;
    val.i = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= FM_FLOAT_MBITS;
    ipart -= FM_FLOAT_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_FLOAT_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2f_p[0];
    px = px*x + fm_log2f_p[1];
    px = px*x + fm_log2f_p[2];
    px = px*x + fm_log2f_p[3];
    px = px*x + fm_log2f_p[4];
    px = px*x + fm_log2f_p[5];
    px = px*x + fm_log2f_p[6];
    px = px*x + fm_log2f_p[7];
    px = px*x + fm_log2f_p[8];

    z = x*x;
    z = x*z*px - 0.5*z + x;
    z += ((float)ipart)*FM_FLOAT_LOGEOF2;
    return z;
}

float fm_log10f(float x) 
{
    ufi_t val;
    float z,px;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i & FM_FLOAT_MMASK;
    ipart = val.i & FM_FLOAT_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_FLOAT_EZERO;
    val.i = fpart;
    x = val.f;

    /* extract exponent */
    ipart >>= FM_FLOAT_MBITS;
    ipart -= FM_FLOAT_BIAS;

    /* the polynomial is computed for sqrt(0.5) < x < sqrt(2),
       but we have the mantissa in the interval 1 < x < 2.
       adjust by dividing x by 2 and incrementing ipart, if needed. */
    if (x > FM_FLOAT_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    /* use polynomial approximation for log(1+x) */
    x -= 1.0;

    px =        fm_log2f_p[0];
    px = px*x + fm_log2f_p[1];
    px = px*x + fm_log2f_p[2];
    px = px*x + fm_log2f_p[3];
    px = px*x + fm_log2f_p[4];
    px = px*x + fm_log2f_p[5];
    px = px*x + fm_log2f_p[6];
    px = px*x + fm_log2f_p[7];
    px = px*x + fm_log2f_p[8];

    z = x*x;
    z = x*z*px - 0.5*z + x;
    z *= FM_FLOAT_LOG10OFE;
    z += ((float)ipart)*FM_FLOAT_LOG10OF2;
    return z;
}

#if defined(LIBM_ALIAS) && !defined(USE_LOG_SPLINE)
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
double log(double x) __attribute__ ((alias("fm_log")));
double log2(double x) __attribute__ ((alias("fm_log2")));
double log10(double x) __attribute__ ((alias("fm_log10")));
float log2f(float x) __attribute__ ((alias("fm_log2f")));
float logf(float x) __attribute__ ((alias("fm_logf")));
float log10f(float x) __attribute__ ((alias("fm_log10f")));
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
