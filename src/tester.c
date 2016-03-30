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

#define _GNU_SOURCE 1
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

extern double exp10(double);
extern float exp10f(float);

#include "fastermath.h"

#include "config.c"

/* compute high precision walltime and walltime difference */
static double wallclock(const double * __restrict ref)
{
    struct timespec t;
    double ret;

    clock_gettime(CLOCK_REALTIME, &t);
    ret = ((double) t.tv_sec)*1.0e6 + 1.0e-3*((double) t.tv_nsec);

    return ref ? (ret - *ref) : ret;
}


int main(int argc, char **argv)
{
    double *xval, *res0, *res1, *res2, *res3;
    float  *xvalf, *res0f, *res1f, *res2f, *res3f;
    double xscale, err, sumerr, start;
    int num, rep, i, j;
    unsigned int seed;

    puts("\n============================="
         "\nfastermath test and benchmark"
         "\n=============================");
    puts(config);

    if (argc < 3) {
        printf("usage %s <num tests> <num reps> [<rng seed>]\n",argv[0]);
        return -1;
    }

    num = atoi(argv[1]);
    rep = atoi(argv[2]);
    if (argc < 4) {
        seed = 1966;
    } else {
        seed = atoi(argv[3]);
    }
    srand(seed);

#define GRABMEM(var,type)                                          \
    posix_memalign((void **)&var, _FM_ALIGN, num*sizeof(type)); \
    memset(var, 0, num*sizeof(type))

    GRABMEM(xval,double);
    GRABMEM(res0,double);
    GRABMEM(res1,double);
    GRABMEM(res2,double);
    GRABMEM(res3,double);

    GRABMEM(xvalf,float);
    GRABMEM(res0f,float);
    GRABMEM(res1f,float);
    GRABMEM(res2f,float);
    GRABMEM(res3f,float);

    puts("-------------------------\ntesting exponentiation functions");
    err = sumerr = 0.0;
    xscale = 1.0 / ((double) RAND_MAX);
    start = wallclock(NULL);
    /* random numbers distributed between -10.0 and 10.0 */ 
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = 0.5*(r1+r2);
        xval[i] = 20.0 * rsum  - 10.0;
        xvalf[i] = (float) xval[i];
        err += xval[i];
        sumerr += xval[i]*xval[i];
    }
    err /= (double) num;
    sumerr /= (double) num;
    printf("time/set for %d x-values : %8.4gus\n", num, wallclock(&start)/num);
    printf("<x>: %.6g    <x**2> - <x>**2: %.15g\n", err, sumerr-(err*err));

    xscale = 1.0/(rep*num);

#define DOUBLE_ERROR(ref,res)                           \
    sumerr = 0.0;                                       \
    for (i=0; i < num; ++i) {                           \
        sumerr += fabs(((double)res[i]- (double)ref[i])/((double) ref[i])); \
    }                                                   \
    printf("avgerr  %.6g\n", sumerr/((double) num))

#define FLOAT_ERROR(ref,res)                            \
    sumerrf = 0.0f;                                     \
    for (i=0; i < num; ++i) {                           \
        sumerrf += fabsf((res[i]-ref[i])/ref[i]);       \
    }                                                   \
    printf("avgerr  %.6g\n", sumerrf/((double) num))

#define RUN_LOOP(x,result,func,type)            \
    memset(result, 0, num*sizeof(type));        \
    start = wallclock(NULL);                    \
    for (j=0; j < rep; ++j) {                   \
        for (i=0; i < num; ++i)                 \
            result[i] += func(x[i]);            \
    }                                           \
    printf("time for% 20s(): %8.4fus  ",        \
           #func, xscale*wallclock(&start))

    RUN_LOOP(xval,res0,exp2,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_exp2,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_exp2,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res0,exp,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_exp,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_exp,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_exp_alt,double);
    DOUBLE_ERROR(res0,res3);
    
    RUN_LOOP(xval,res0,exp10,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_exp10,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_exp10,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_exp10_alt,double);
    DOUBLE_ERROR(res0,res3);


    RUN_LOOP(xvalf,res0f,exp2f,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_exp2f,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_exp2f,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res0f,expf,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_expf,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_expf,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_expf_alt,float);
    DOUBLE_ERROR(res0f,res3f);
    
    RUN_LOOP(xvalf,res0f,exp10f,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_exp10f,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_exp10f,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_exp10f_alt,float);
    DOUBLE_ERROR(res0f,res3f);


    puts("-------------------------\ntesting logarithm functions");
    err = sumerr = 0.0;
    xscale = 1.0 / ((double) RAND_MAX);
    start = wallclock(NULL);
    /* random numbers distributed between 2**-10.0 and 2**10.0 */ 
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = 0.5*(r1+r2);
        xval[i] = exp2(20.0 * rsum  - 10.0);
        xvalf[i] = (float) xval[i];
        err += xval[i];
        sumerr += xval[i]*xval[i];
    }
    err /= (double) num;
    sumerr /= (double) num;
    printf("time/set for %d x-values : %8.4gus\n", num, wallclock(&start)/num);
    xscale = 1.0/(rep*num);

    RUN_LOOP(xval,res0,log2,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_log2,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_log2,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_log2_alt,double);
    DOUBLE_ERROR(res0,res3);

    RUN_LOOP(xval,res0,log,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_log,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_log,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_log_alt,double);
    DOUBLE_ERROR(res0,res3);

    RUN_LOOP(xval,res0,log10,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_log10,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,fm_log10,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_log10_alt,double);
    DOUBLE_ERROR(res0,res3);

    RUN_LOOP(xvalf,res0f,log2f,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_log2f,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_log2f,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_log2f_alt,float);
    DOUBLE_ERROR(res0f,res3f);

    RUN_LOOP(xvalf,res0f,logf,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_logf,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_logf,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_logf_alt,float);
    DOUBLE_ERROR(res0f,res3f);

    RUN_LOOP(xvalf,res0f,log10f,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_log10f,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,fm_log10f,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_log10f_alt,float);
    DOUBLE_ERROR(res0f,res3f);

    puts("-------------------------\ntesting error function and complement");
    err = sumerr = 0.0;
    xscale = 16.0 / ((double) RAND_MAX);
    start = wallclock(NULL);
    /* random numbers distributed between -6.0 and 26.0 */ 
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = (r1+r2);
        xval[i] = rsum - 6.0;
        xvalf[i] = (float) xval[i];
        err += xval[i];
        sumerr += xval[i]*xval[i];
    }
    err /= (double) num;
    sumerr /= (double) num;
    printf("time/set for %d x-values : %8.4gus\n", num, wallclock(&start)/num);
    xscale = 1.0/(rep*num);

    RUN_LOOP(xval,res0,erf,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_erf,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,(1.0) - erfc,double);
    DOUBLE_ERROR(res0,res2);
#if 0
    RUN_LOOP(xval,res3,fm_log2_alt,double);
    DOUBLE_ERROR(res0,res3);
#endif

    RUN_LOOP(xval,res0,erfc,double);
    printf("numreps %d\n", rep);

    RUN_LOOP(xval,res1,__builtin_erfc,double);
    DOUBLE_ERROR(res0,res1);

    RUN_LOOP(xval,res2,(1.0) - erf,double);
    DOUBLE_ERROR(res0,res2);

    RUN_LOOP(xval,res3,fm_erfc,double);
    DOUBLE_ERROR(res0,res3);


    RUN_LOOP(xvalf,res0f,erff,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_erff,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,1.0f-erfcf,float);
    DOUBLE_ERROR(res0f,res2f);
#if 0
    RUN_LOOP(xvalf,res3f,fm_log2f_alt,float);
    DOUBLE_ERROR(res0f,res3f);
#endif
    RUN_LOOP(xvalf,res0f,erfcf,float);
    printf("numreps %d\n", rep);

    RUN_LOOP(xvalf,res1f,__builtin_erfcf,float);
    DOUBLE_ERROR(res0f,res1f);

    RUN_LOOP(xvalf,res2f,1.0f-erff,float);
    DOUBLE_ERROR(res0f,res2f);

    RUN_LOOP(xvalf,res3f,fm_erfcf,float);
    DOUBLE_ERROR(res0f,res3f);

    
    free(xval);
    free(res0);
    free(res1);
    free(res2);
    free(res3);
    free(xvalf);
    free(res0f);
    free(res1f);
    free(res2f);
    free(res3f);

    return 0;

#if 0
    /* special cases */


    puts("testing special cases. libm exp10() / exp10f()");
    printf("exp10(0.0)-1.0 = %.15g\n", exp10(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", exp10(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", exp10(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", exp10(10.0)-1e10);

    printf("exp10f(0.0f)-1.0f = %.15g\n", exp10f(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", exp10f(1.0f)-10.0f);
    printf("exp10(-1.0f)-0.1f = %.15g\n", exp10f(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", exp10f(10.0f)-1e10f);
    printf("exp10(10.0f) = %.15g\n", exp10f(10.0f));

    puts("testing special cases. libm fm_exp10() / fm_exp10f()");
    printf("exp10(0.0)-1.0 = %.15g\n", fm_exp10(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", fm_exp10(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", fm_exp10(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", fm_exp10(10.0)-1e10);

    printf("exp10f(0.0f)-1.0f = %.15g\n", fm_exp10f(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", fm_exp10f(1.0f)-10.0f);
    printf("exp10(-1.0f)-0.1f = %.15g\n", fm_exp10f(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", fm_exp10f(10.0f)-1e10f);
    printf("exp10(10.0f) = %.15g\n", fm_exp10f(10.0f));

    puts("testing special cases. libm fm_exp10_alt() / fm_exp10f_alt()");
    printf("exp10(0.0)-1.0 = %.15g\n", fm_exp10_alt(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.15g\n", fm_exp10_alt(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.15g\n", fm_exp10_alt(-1.0)-0.1);
    printf("exp10(10.0)-1e10 = %.15g\n", fm_exp10_alt(10.0)-1e10);
    printf("exp10(10.0f) = %.15g\n", exp10f(10.0f));

    printf("exp10f(0.0f)-1.0f = %.15g\n", fm_exp10f_alt(0.0f)-1.0f);
    printf("exp10f(1.0f)-10.0f = %.15g\n", fm_exp10f_alt(1.0f)-10.0f);
    printf("exp10(-1.0)-0.1f = %.15g\n", fm_exp10f_alt(-1.0f)-0.1f);
    printf("exp10(10.0f)-1e10f = %.15g\n", fm_exp10f_alt(10.0f)-1e10f);

#endif

}

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make -C .."
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
