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
 * log_2(fpart) from a spline table of log(x) in [1.0:2.0[
 */

#include "log_spline_tbl.c"

double fm_log2_alt(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> FM_DOUBLE_EBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= 0x000fffff;
    val.i1 = hx | 0x3ff00000;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.i0 = 0;
    val.i1 = 0x3ff00000 | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double) ipart) + (y * FM_DOUBLE_LOG2OFE);
}


/* optimizer friendly implementation of log(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log(fpart) from a spline table in [1.0:2.0[
 */

double fm_log_alt(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> FM_DOUBLE_EBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= 0x000fffff;
    val.i1 = hx | 0x3ff00000;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.i0 = 0;
    val.i1 = 0x3ff00000 | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double)ipart)*FM_DOUBLE_LOGEOF2 + y;
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
