/* build static spline tables in double and single precision */

#include "fastermath.h"
#include "fm_internal.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/* code to generate double precision spline tables */

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


/* build 12-bit spline table */
#define FM_SPLINE_SHIFT 8

static void fm_init_log_spl() 
{
    udi_t val;
    double x,delta;
    int i, max;

    FILE *fp;

    max = 1<<(FM_DOUBLE_MBITS-FM_SPLINE_SHIFT);

    posix_memalign((void **)&fm_log_q1, _FM_ALIGN, (max+4)*sizeof(double));
    posix_memalign((void **)&fm_log_q2, _FM_ALIGN, (max+4)*sizeof(double));

    /* determine grid spacing and compute derived properties */
    val.i1 = 0x3ff00000 | (1 << FM_SPLINE_SHIFT);
    delta = val.f - 1.0;
    fm_log_dinv  = 1.0/delta;
    fm_log_dsq6  = delta*delta/6.0;

    printf("init spline table for log() with %d bits. "
           "delta=%.15g  mem=%.3f kB\n",
           FM_DOUBLE_MBITS-FM_SPLINE_SHIFT,
           delta, (max+2)*sizeof(double)/512.0);

    val.i0 = 0;
    for (i=0; i < max; ++i) {
        val.i1 = 0x3ff00000 | (i << FM_SPLINE_SHIFT);
        x = val.f;
        fm_log_q1[i] = log(x);
    }
    fm_log_q1[max] = FM_DOUBLE_LOGEOF2;
    fm_spline(delta,fm_log_q1,max+1,1.0,0.5,fm_log_q2);

    fp = fopen("log_spline_tbl.c","w");
    fprintf(fp,"\n#define FM_SPLINE_SHIFT %d\n",FM_SPLINE_SHIFT);
    fprintf(fp,"static const double fm_log_dinv = % 025.20e;\n", fm_log_dinv);
    fprintf(fp,"static const double fm_log_dsq6 = % 025.20e;\n", fm_log_dsq6);
    fprintf(fp,"static const double fm_log_q1[] "
            "__attribute__ ((aligned(_FM_ALIGN))) = {\n");
    for (i=0; i < max; i += 2) {
        fprintf(fp,"% 025.20e, ",  fm_log_q1[i]);
        fprintf(fp,"% 025.20e,\n", fm_log_q1[i+1]);
    }
    fprintf(fp,"% 025.20e\n};\n\n", fm_log_q1[max]);

    fprintf(fp,"static const double fm_log_q2[] __attribute__ ((aligned(_FM_ALIGN))) = {\n");
    for (i=0; i < max; i += 2) {
        fprintf(fp,"% 025.20e, ",  fm_log_q2[i]);
        fprintf(fp,"% 025.20e,\n", fm_log_q2[i+1]);
    }
    fprintf(fp,"% 025.20e\n};\n", fm_log_q2[max]);
    fclose(fp);

    free(fm_log_q1);
    free(fm_log_q2);
}

/* code to generate single precision spline tables */

static void fm_splinef(float delta, float *y, int n,
                      float yp1, float ypn, float *y2)
{
  int i,k;
  float p,qn,un;
  float *u = (float *) malloc(n*sizeof(float));

  y2[0] = -0.5f;
  u[0] = (3.0f/(delta)) * ((y[1]-y[0]) / (delta) - yp1);

  for (i = 1; i < n-1; i++) {
    p = 0.5f*y2[i-1] + 2.0f;
    y2[i] = -0.5f / p;
    u[i] = (y[i+1]-2.0f*y[i]+y[i-1]) / delta;
    u[i] = (3.0f*u[i] / delta - 0.5f*u[i-1]) / p;
  }

  qn = 0.5f;
  un = (3.0f/delta) * (ypn - (y[n-1]-y[n-2]) / delta);

  y2[n-1] = (un-qn*u[n-2]) / (qn*y2[n-2] + 1.0f);
  for (k = n-2; k >= 0; k--) y2[k] = y2[k]*y2[k+1] + u[k];

  free(u);
}

static float *fm_logf_q1    = NULL;
static float *fm_logf_q2    = NULL;
static float  fm_logf_dsq6  = 0.0f;
static float  fm_logf_dinv  = 0.0f;


/* build 12-bit spline table */
#define FM_SPLINEF_SHIFT 11

static void fm_init_logf_spl() 
{
    ufi_t val;
    float x,delta;
    int i, max;

    FILE *fp;

    max = 1<<(FM_FLOAT_MBITS-FM_SPLINEF_SHIFT);

    posix_memalign((void **)&fm_logf_q1, _FM_ALIGN, (max+4)*sizeof(float));
    posix_memalign((void **)&fm_logf_q2, _FM_ALIGN, (max+4)*sizeof(float));

    /* determine grid spacing and compute derived properties */
    val.i = FM_FLOAT_EZERO | (1 << FM_SPLINEF_SHIFT);
    delta = val.f - 1.0f;
    fm_logf_dinv  = 1.0f/delta;
    fm_logf_dsq6  = delta*delta/6.0f;

    printf("init spline table for logf() with %d bits. "
           "delta=%.15g  mem=%.3f kB\n",
           FM_FLOAT_MBITS-FM_SPLINEF_SHIFT,
           delta, (max+2)*sizeof(float)/512.0f);

    for (i=0; i < max; ++i) {
        val.i = 0x3ff00000 | (i << FM_SPLINEF_SHIFT);
        x = val.f;
        fm_logf_q1[i] = logf(x);
    }
    fm_logf_q1[max] = FM_FLOAT_LOGEOF2;
    fm_splinef(delta,fm_logf_q1,max+1,1.0f,0.5f,fm_logf_q2);

    fp = fopen("logf_spline_tbl.c","w");
    fprintf(fp,"\n#define FM_SPLINEF_SHIFT %d\n",FM_SPLINEF_SHIFT);
    fprintf(fp,"static const float fm_logf_dinv = % 015.10e;\n", fm_logf_dinv);
    fprintf(fp,"static const float fm_logf_dsq6 = % 015.10e;\n", fm_logf_dsq6);
    fprintf(fp,"static const float fm_logf_q1[] "
            "__attribute__ ((aligned(_FM_ALIGN))) = {\n");
    for (i=0; i < max; i += 2) {
        fprintf(fp,"% 015.10e, ",  fm_logf_q1[i]);
        fprintf(fp,"% 015.10e,\n", fm_logf_q1[i+1]);
    }
    fprintf(fp,"% 015.10e\n};\n\n", fm_logf_q1[max]);

    fprintf(fp,"static const float fm_logf_q2[] __attribute__ ((aligned(_FM_ALIGN))) = {\n");
    for (i=0; i < max; i += 2) {
        fprintf(fp,"% 015.10e, ",  fm_logf_q2[i]);
        fprintf(fp,"% 015.10e,\n", fm_logf_q2[i+1]);
    }
    fprintf(fp,"% 015.10e\n};\n", fm_logf_q2[max]);
    fclose(fp);

    free(fm_logf_q1);
    free(fm_logf_q2);
}

int main(int argc, char **argv)
{
    fm_init_log_spl();
    fm_init_logf_spl();

    return 0;
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
