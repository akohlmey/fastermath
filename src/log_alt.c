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
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.i0 = 0;
    val.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
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
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.i0 = 0;
    val.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double)ipart)*FM_DOUBLE_LOGEOF2 + y;
}


/* optimizer friendly implementation of log2f(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log_2(fpart) from a spline table of logf(x) in [1.0:2.0[
 */

#include "logf_spline_tbl.c"

float fm_log2f_alt(float x) 
{
    ufi_t val;
    float a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_FLOAT_EMASK) >> FM_FLOAT_MBITS) - FM_FLOAT_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_FLOAT_MMASK;
    val.i = hx | FM_FLOAT_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINEF_SHIFT;

    /* compute x value matching table index */
    val.i = FM_FLOAT_EZERO | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_logf_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_logf_q1[hx] + b * fm_logf_q1[hx+1];
    a = (a*a*a-a) * fm_logf_q2[hx];
    b = (b*b*b-b) * fm_logf_q2[hx+1];
    y += (a + b) * fm_logf_dsq6;

    return ((float) ipart) + (y * FM_FLOAT_LOG2OFE);
}


/* optimizer friendly implementation of logf(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log(fpart) from a spline table in [1.0:2.0[
 */

float fm_logf_alt(float x) 
{
    ufi_t val;
    float a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_FLOAT_EMASK) >> FM_FLOAT_MBITS) - FM_FLOAT_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_FLOAT_MMASK;
    val.i = hx | FM_FLOAT_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINEF_SHIFT;

    /* compute x value matching table index */
    val.i = FM_FLOAT_EZERO | (hx << FM_SPLINEF_SHIFT);
    b = (x - val.f) * fm_logf_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_logf_q1[hx] + b * fm_logf_q1[hx+1];
    a = (a*a*a-a) * fm_logf_q2[hx];
    b = (b*b*b-b) * fm_logf_q2[hx+1];
    y += (a + b) * fm_logf_dsq6;

    return ((float)ipart)*FM_FLOAT_LOGEOF2 + y;
}

#if defined(LIBM_ALIAS)
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
double log(double x) __attribute__ ((alias("fm_log_alt")));
/* double log2(double x) __attribute__ ((alias("fm_log2"))); */
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
