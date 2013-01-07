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

static const double fm_log2_r[] __attribute__ ((aligned(32))) = {
    1.01875663804580931796e-4,  /* p[0] */
    1.00000000000000000000e0,   /* q[-1] */
    4.97494994976747001425e-1,  /* p[1] */
    1.12873587189167450590e1,   /* q[0] */
    4.70579119878881725854e0,   /* p[2] */
    4.52279145837532221105e1,   /* q[1] */
    1.44989225341610930846e1,   /* p[3] */
    8.29875266912776603211e1,   /* q[2] */
    1.79368678507819816313e1,   /* p[4] */
    7.11544750618563894466e1,   /* q[3] */
    7.70838733755885391666e0,   /* p[5] */
    2.31251620126765340583e1    /* q[4] */
};


double fm_log2(double x) 
{
    udi_t val;
    double z,px,qx;
    int32_t ipart,fpart;

    val.f = x;
    /* extract exponent and subtract bias */
    ipart = (((val.i[1] & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);
    /* mask out exponent to get the prefactor to 2**ipart */
    val.i[1] &= 0x000fffff;
    val.i[1] |= 0x3ff00000;
    x = val.f;

    if (x > FM_DOUBLE_SQRTH) {
        x *= 0.5;
        ++ipart;
    }
    x -= 1.0;

    px =        fm_log2_r[0];
    qx =        fm_log2_r[1];
    px = px*x + fm_log2_r[2];
    qx = qx*x + fm_log2_r[3];
    px = px*x + fm_log2_r[4];
    qx = qx*x + fm_log2_r[5];
    px = px*x + fm_log2_r[6];
    qx = qx*x + fm_log2_r[7];
    px = px*x + fm_log2_r[8];
    qx = qx*x + fm_log2_r[9];
    px = px*x + fm_log2_r[10];
    qx = qx*x + fm_log2_r[11];

    z = x*x;
    z = x*(z*px/qx) - 0.5*z + x;
    z *= FM_DOUBLE_LOG2OFE;

    return ((double)ipart) + z;
}

#if defined(LIBM_ALIAS)
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
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
