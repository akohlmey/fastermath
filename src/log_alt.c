/* faster versions of logarithms in single and double precision.
 *
 * Based on the Cephes math library 2.8
 */

#include "fastermath.h"
#include "fm_internal.h"

/* optimizer friendly implementation of log2(x). */
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
    7.70838733755885391666e0,
};

static const double fm_log2_q[] __attribute__ ((aligned(_FM_ALIGN))) = {
/*  1.00000000000000000000e0, */
    1.12873587189167450590e1,
    4.52279145837532221105e1,
    8.29875266912776603211e1,
    7.11544750618563894466e1,
    2.31251620126765340583e1,
};

static const double fm_log2_r[] __attribute__ ((aligned(_FM_ALIGN))) = {
    -7.89580278884799154124e-1,
     1.63866645699558079767e1,
    -6.41409952958715622951e1
};

static const double fm_log2_s[] __attribute__ ((aligned(_FM_ALIGN))) = {
/*   1.00000000000000000000e0, */
    -3.56722798256324312549e1,
     3.12093766372244180303e2,
    -7.69691943550460008604e2
};

static const double fm_log2ofe_1 = 0.44269504088896340735992;

double fm_log2_alt(double x) 
{
    udi_t val;
    double z,y,px,qx;
    int32_t ipart;

    val.f = x;
    /* extract exponent and subtract bias */
    ipart = (((val.i[1] & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);
    /* mask out exponent to get the prefactor to 2**ipart */
    val.i[1] &= 0x000fffff;
    val.i[1] |= 0x3ff00000;
    x = val.f;

    if (((ipart > 2) || (ipart < -2))) {

        if (x > FM_DOUBLE_SQRT2) {
            x *= 0.5;
            ++ipart;
        }
        
        z  = x - 0.5;
        z -= 0.5;
        y = 0.5 * x  + 0.5;

        x = z / y;
        z = x*x;

        px =        fm_log2_r[0];
        qx =    z + fm_log2_s[0];
        px = px*z + fm_log2_r[1];
        qx = qx*z + fm_log2_s[1];
        px = px*z + fm_log2_r[2];
        qx = qx*z + fm_log2_s[2];

        y = x * (z*px/qx);

    } else {
        
        if (x > FM_DOUBLE_SQRT2) {
            x *= 0.5;
            ++ipart;
        }

        x -= 1.0;
        z = x*x;

        px =        fm_log2_p[0];
        qx =    x + fm_log2_q[0];
        px = px*x + fm_log2_p[1];
        qx = qx*x + fm_log2_q[1];
        px = px*x + fm_log2_p[2];
        qx = qx*x + fm_log2_q[2];
        px = px*x + fm_log2_p[3];
        qx = qx*x + fm_log2_q[3];
        px = px*x + fm_log2_p[4];
        qx = qx*x + fm_log2_q[4];
        px = px*x + fm_log2_p[5];

        y = x*(z*px/qx) - 0.5*z;
    }

    z = y * fm_log2ofe_1;
    z += x * fm_log2ofe_1;
    z += y;
    z += x;
    return ((double)ipart) + z;
}

double fm_log_alt(double x) 
{
    udi_t val;
    double z,y,px,qx;
    int32_t ipart;

    val.f = x;
    /* extract exponent and subtract bias */
    ipart = (((val.i[1] & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);
    /* mask out exponent to get the prefactor to 2**ipart */
    val.i[1] &= 0x000fffff;
    val.i[1] |= 0x3ff00000;
    x = val.f;

    if (((ipart > 2) || (ipart < -2))) {

        if (x > FM_DOUBLE_SQRT2) {
            x *= 0.5;
            ++ipart;
        }
        
        z  = x - 0.5;
        z -= 0.5;
        y = 0.5 * x  + 0.5;

        x = z / y;
        z = x*x;

        px =        fm_log2_r[0];
        qx =    z + fm_log2_s[0];
        px = px*z + fm_log2_r[1];
        qx = qx*z + fm_log2_s[1];
        px = px*z + fm_log2_r[2];
        qx = qx*z + fm_log2_s[2];

        y = x * (z*px/qx);

    } else {
        
        if (x > FM_DOUBLE_SQRT2) {
            x *= 0.5;
            ++ipart;
        }

        x -= 1.0;
        z = x*x;

        px =        fm_log2_p[0];
        qx =    x + fm_log2_q[0];
        px = px*x + fm_log2_p[1];
        qx = qx*x + fm_log2_q[1];
        px = px*x + fm_log2_p[2];
        qx = qx*x + fm_log2_q[2];
        px = px*x + fm_log2_p[3];
        qx = qx*x + fm_log2_q[3];
        px = px*x + fm_log2_p[4];
        qx = qx*x + fm_log2_q[4];
        px = px*x + fm_log2_p[5];

        y = x*(z*px/qx) - 0.5*z;
    }

    z = y + x;
    z += ((double)ipart) * FM_DOUBLE_LOGEOF2;
    return z;
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
