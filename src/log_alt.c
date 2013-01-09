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

#if 0
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

    FILE *fp;

    max = 1<<(FM_DOUBLE_EBITS-FM_SPLINE_SHIFT);

    posix_memalign((void **)&fm_log_q1, _FM_ALIGN, (max+4)*sizeof(double));
    posix_memalign((void **)&fm_log_q2, _FM_ALIGN, (max+4)*sizeof(double));

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

    fp = fopen("log_spline.c","w");
    fprintf(fp,"static const double fm_log_dinv = % 025.20e;\n", fm_log_dinv);
    fprintf(fp,"static const double fm_log_dsq6 = % 025.20e;\n", fm_log_dsq6);
    fprintf(fp,"static const double fm_log_q1[] = {\n");
    for (i=0; i < max+2; i += 2) {
        fprintf(fp,"% 025.20e, ",  fm_log_q1[i]);
        fprintf(fp,"% 025.20e,\n", fm_log_q1[i+1]);
    }
    fprintf(fp,"};\n");
    fprintf(fp,"static const double fm_log_q2[] = {\n");
    for (i=0; i < max+2; i += 2) {
        fprintf(fp,"% 025.20e, ",  fm_log_q2[i]);
        fprintf(fp,"% 025.20e,\n", fm_log_q2[i+1]);
    }
    fprintf(fp,"};\n");

    fclose(fp);
}

#endif

#include "log_spline_tbl.c"
#define FM_SPLINE_SHIFT 8

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
