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

static const double fm_log2_p[] __attribute__ ((aligned(16))) = {
    1.01875663804580931796e-4,
    4.97494994976747001425e-1,
    4.70579119878881725854e0,
    1.44989225341610930846e1,
    1.79368678507819816313e1,
    7.70838733755885391666e0
};

static const double fm_log2_q[] __attribute__ ((aligned(16))) = {
/*  1.00000000000000000000e0, */
    1.12873587189167450590e1,
    4.52279145837532221105e1,
    8.29875266912776603211e1,
    7.11544750618563894466e1,
    2.31251620126765340583e1
};

static const double fm_log2_r[] __attribute__ ((aligned(16))) = {
    -7.89580278884799154124e-1,
     1.63866645699558079767e1,
    -6.41409952958715622951e1
};

static const double fm_log2_s[] __attribute__ ((aligned(16))) = {
/*   1.00000000000000000000e0, */
    -3.56722798256324312549e1,
     3.12093766372244180303e2,
    -7.69691943550460008604e2
};

double fm_log2(double x) 
{
    udi_t val;
    double z,px,qx;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and part of the mantissa */
    fpart = val.i[1] & FM_DOUBLE_MMASK;
    ipart = val.i[1] & FM_DOUBLE_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_DOUBLE_EZERO;
    val.i[1] = fpart;
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
    fpart = val.i[1] & FM_DOUBLE_MMASK;
    ipart = val.i[1] & FM_DOUBLE_EMASK;

    /* set exponent to 0 to get the prefactor to 2**ipart */
    fpart |= FM_DOUBLE_EZERO;
    val.i[1] = fpart;
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
    z += ((double)ipart)*FM_DOUBLE_LOGEOF2;
    return z;
}

#if defined(LIBM_ALIAS)
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
double log(double x) __attribute__ ((alias("fm_log")));
double log2(double x) __attribute__ ((alias("fm_log2")));
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
