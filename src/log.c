/* 
   Copyright (c) 2012,2013   Axel Kohlmeyer <akohlmey@gmail.com> 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of the <organization> nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

double fm_log2(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.s.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.s.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.s.i0 = 0;
    val.s.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
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

double fm_log(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.s.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.s.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.s.i0 = 0;
    val.s.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double)ipart)*FM_DOUBLE_LOGEOF2 + y;
}


/* optimizer friendly implementation of log10(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log(fpart) from a spline table in [1.0:2.0[
 */

double fm_log10(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.s.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.s.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.s.i0 = 0;
    val.s.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double) ipart)*FM_DOUBLE_LOG10OF2 + (y * FM_DOUBLE_LOG10OFE);
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

float fm_log2f(float x) 
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
    a = 1.0f - b;

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

float fm_logf(float x) 
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
    a = 1.0f - b;

    /* evaluate spline */
    y = a * fm_logf_q1[hx] + b * fm_logf_q1[hx+1];
    a = (a*a*a-a) * fm_logf_q2[hx];
    b = (b*b*b-b) * fm_logf_q2[hx+1];
    y += (a + b) * fm_logf_dsq6;

    return ((float)ipart)*FM_FLOAT_LOGEOF2 + y;
}


/* optimizer friendly implementation of log10f(x).
 *
 * strategy:
 *
 * split the argument into a product, 2**ipart * fpart
 * by using bitmasks and integer math and then approximate
 * log(fpart) from a spline table in [1.0:2.0[
 */

float fm_log10f(float x) 
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
    a = 1.0f - b;

    /* evaluate spline */
    y = a * fm_logf_q1[hx] + b * fm_logf_q1[hx+1];
    a = (a*a*a-a) * fm_logf_q2[hx];
    b = (b*b*b-b) * fm_logf_q2[hx+1];
    y += (a + b) * fm_logf_dsq6;

    return ((float)ipart)*FM_FLOAT_LOG10OF2 + y*FM_FLOAT_LOG10OFE;
}

#if defined(LIBM_ALIAS) && defined(USE_LOG_SPLINE)
/* include aliases to the equivalent libm functions for use with LD_PRELOAD. */
double log(double x) __attribute__ ((alias("fm_log")));
double log2(double x) __attribute__ ((alias("fm_log2")));
double log10(double x) __attribute__ ((alias("fm_log10")));
float logf(float x) __attribute__ ((alias("fm_logf")));
float log2f(float x) __attribute__ ((alias("fm_log2f")));
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
