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

#if 0

double fm_log2_alt(double x) 
{
    udi_t val;
    double z,y,px,qx;
    int32_t ipart;

    val.f = x;
    /* extract exponent and subtract bias */
    ipart = (((val.i1 & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);
    /* mask out exponent to get the prefactor to 2**ipart */
    val.i1 &= 0x000fffff;
    val.i1 |= 0x3ff00000;
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
#else

static const double Lg1 = 6.666666666666735130e-01;     /* 3FE55555 55555593 */
static const double Lg2 = 3.999999999940941908e-01;     /* 3FD99999 9997FA04 */
static const double Lg3 = 2.857142874366239149e-01;     /* 3FD24924 94229359 */
static const double Lg4 = 2.222219843214978396e-01;     /* 3FCC71C5 1D8E78AF */
static const double Lg5 = 1.818357216161805012e-01;     /* 3FC74664 96CB03DE */
static const double Lg6 = 1.531383769920937332e-01;     /* 3FC39A09 D078C69F */
static const double Lg7 = 1.479819860511658591e-01;     /* 3FC2F112 DF3E5244 */

double fm_log2_alt(double x)
{
    udi_t val;
    double z,f,w,s,t1,t2,R,dk;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= 0x000fffff;
    val.i1 = hx | 0x3ff00000;
    x = val.f;

    if (x > FM_DOUBLE_SQRT2) {
        x *= 0.5;
        ++ipart;
    }

    f = x - 1.0;

    s = f / (2.0 + f);
    z = s * s;
    w = z*z;
    dk = (double) ipart;
    t1 = w*(Lg2 + w*(Lg4 + w*Lg6));
    t2 = z*(Lg1 + w*(Lg3 + w*(Lg5 + w *Lg7)));
    R = t1+t2;
    return dk - ((s * (f - R)) -f ) * FM_DOUBLE_LOG2OFE;
}

#endif

double fm_log_alt(double x) 
{
    udi_t val;
    double z,y,px,qx;
    int32_t ipart;

    val.f = x;
    /* extract exponent and subtract bias */
    ipart = (((val.i1 & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);
    /* mask out exponent to get the prefactor to 2**ipart */
    val.i1 &= 0x000fffff;
    val.i1 |= 0x3ff00000;
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

#if 0

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
static double *fm_log_tbl   = NULL;
static double  fm_log_frac  = 0.0;
static int     fm_log_shift = 0;

void fm_init_log_tbl(int bits) 
{
    udi_t val;
    int i, max, nshift;

    if (bits > 20) bits=20;
    nshift = 20 - bits;
    max = 1<<bits;
    fm_log_shift = nshift;

    printf("init log table with %d bits. nshift=%d mem=%.3f MB ",
           bits, nshift, max*sizeof(double)/1024.0/1024.0);

    posix_memalign((void **)&fm_log_tbl, _FM_ALIGN, max*sizeof(double));

    val.i0 = 0;
    for (i=0; i < max; ++i) {
        val.i1 = 0x3ff00000 | (i << nshift);
        fm_log_tbl[i] = log(val.f);
    }

    val.i1 = 0x3ff00000 | (1 << nshift);
    fm_log_frac = 1.0/(val.f - 1.0);
    printf("fm_log_frac=%.15g\n",fm_log_frac);

}

double fm_log2_tbl(double x)
{
    udi_t val;
    int32_t hx, ipart;
    const double *tbl;
    double f1, f2;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= 0x000fffff;
    val.i1 = hx | 0x3ff00000;
    hx >>= fm_log_shift;
    x = val.f;

    val.i0 = 0;
    val.i1 = 0x3ff00000 | (hx <<fm_log_shift);

    tbl = fm_log_tbl + hx;
    f1 = *tbl;
    ++tbl;
    f2 = *tbl;
    f1 += (f2-f1) * fm_log_frac * (x-val.f);

    return ((double)ipart) + (f1* FM_DOUBLE_LOG2OFE);
}

#else


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
static double *fm_log_tbl   = NULL;
static double  fm_log_frac  = 0.0;
static int     fm_log_shift = 0;

void fm_init_log_tbl(int bits) 
{
    udi_t val;
    double x,y;
    int i, max, nshift;

    if (bits > 20) bits=20;
    nshift = 20 - bits;
    max = 1<<bits;
    fm_log_shift = nshift;

    printf("init log table with %d bits. nshift=%d mem=%.3f MB ",
           bits, nshift, max*sizeof(double)/1024.0/1024.0);

    posix_memalign((void **)&fm_log_tbl, _FM_ALIGN, max*sizeof(double));

    val.i1 = 0x3ff00000 | (1 << nshift);
    fm_log_frac = 1.0/(val.f - 1.0);
    printf("fm_log_frac=%.15g\n",fm_log_frac);

    val.i0 = 0;
    for (i=0; i < max; ++i) {
        val.i1 = 0x3ff00000 | (i << nshift);
        x = val.f;
        fm_log_tbl[i] = log(x);
    }
}

double fm_log2_tbl(double x)
{
    udi_t val;
    int32_t hx, ipart;
    const double *tbl;
    double f1, f2;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= 0x000fffff;
    val.i1 = hx | 0x3ff00000;
    hx >>= fm_log_shift;
    x = val.f;

#if 1
    val.i0 = 0;
    val.i1 = 0x3ff00000 | (hx <<fm_log_shift);

    tbl = fm_log_tbl + hx;
    f1 = *tbl;
    ++tbl;
    f2 = *tbl;
    f1 += (f2-f1) * fm_log_frac * (x-val.f);
#else
    /* compute approximation */
    f1 = (x-3.0)/(x+1.0) + 1.0;
#endif

    return ((double)ipart) + (f1* FM_DOUBLE_LOG2OFE);
}
#endif


static void fm_spline(double delta, double *y, int n,
                      double yp1, double ypn, double *y2)
{
  int i,k;
  double p,qn,un;
  double *u = (double *) malloc(n*sizeof(double));

  y2[0] = -0.5;
  u[0] = (3.0/(delta)) * ((y[1]-y[0]) / (delta) - yp1);

  for (i = 1; i < n-1; i++) {
    p = 0.5*y2[i-1] + 2.0;
    y2[i] = -0.5 / p;
    u[i] = (y[i+1]-2.0*y[i]+y[i-1]) / delta;
    u[i] = (3.0*u[i] / delta - 0.5*u[i-1]) / p;
  }

  qn = 0.5;
  un = (3.0/delta) * (ypn - (y[n-1]-y[n-2]) / delta);

  y2[n-1] = (un-qn*u[n-2]) / (qn*y2[n-2] + 1.0);
  for (k = n-2; k >= 0; k--) y2[k] = y2[k]*y2[k+1] + u[k];

  free(u);
}

static double *fm_log_q1    = NULL;
static double *fm_log_q2    = NULL;
static double  fm_log_dsq6  = 0.0;
static double  fm_log_dinv  = 0.0;

#define FM_SPLINE_SHIFT 8

/* build 12-bit spline table */
void fm_init_log_spl() 
{
    udi_t val;
    double x,delta;
    int i, max;

    max = 1<<(FM_DOUBLE_EBITS-FM_SPLINE_SHIFT);

    posix_memalign((void **)&fm_log_q1, _FM_ALIGN, (max+2)*sizeof(double));
    posix_memalign((void **)&fm_log_q2, _FM_ALIGN, (max+2)*sizeof(double));

    /* determine grid spacing and compute derived properties */
    val.i1 = 0x3ff00000 | (1 << FM_SPLINE_SHIFT);
    delta = val.f - 1.0;
    fm_log_dinv  = 1.0/delta;
    fm_log_dsq6  = delta*delta/6.0;

    printf("init log table with %d bits. delta=%.15g  mem=%.3f kB\n",
           FM_DOUBLE_EBITS-FM_SPLINE_SHIFT,
           delta, (max+2)*sizeof(double)/512.0);

    val.i0 = 0;
    for (i=0; i < max; ++i) {
        val.i1 = 0x3ff00000 | (i << FM_SPLINE_SHIFT);
        x = val.f;
        fm_log_q1[i] = log(x);
    }
    fm_log_q1[max] = FM_DOUBLE_LOGEOF2;
    fm_spline(delta,fm_log_q1,max+1,1.0,0.5,fm_log_q2);
}

double fm_log2_spl(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & 0x7ff00000) >> 20) - FM_DOUBLE_BIAS);

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

    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];

    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double) ipart) + (y * FM_DOUBLE_LOG2OFE);
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
