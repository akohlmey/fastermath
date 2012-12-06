
#define _GNU_SOURCE
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "fastermath.h"

#define FM_DATA_ALIGN 16

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
    double xscale, diff, err, sumerr, start;
    double *xval, *res0, *res1, *res2, *res3;
    int num, rep, i, j;
    unsigned int seed;

    if (argc < 3) return -1;
    num = atoi(argv[1]);
    rep = atoi(argv[2]);
    if (argc < 4) {
        seed = 1966;
    } else {
        seed = atoi(argv[2]);
    }
    srand(seed);
    xscale = 1.0 / ((double) RAND_MAX);

    posix_memalign((void **)&xval, FM_DATA_ALIGN, num*sizeof(double));
    posix_memalign((void **)&res0, FM_DATA_ALIGN, num*sizeof(double));
    posix_memalign((void **)&res1, FM_DATA_ALIGN, num*sizeof(double));
    posix_memalign((void **)&res2, FM_DATA_ALIGN, num*sizeof(double));
    posix_memalign((void **)&res3, FM_DATA_ALIGN, num*sizeof(double));

    puts("testing exp2()");

    err = sumerr = 0.0;
    start = wallclock(NULL);
    for (i=0; i < num; ++i) {
        double r1,r2,rsum;
        r1 = xscale * ((double) rand());
        r2 = xscale * ((double) rand());
        rsum = 0.5*(r1+r2);
        xval[i] = 10.0 * rsum  - 5.0;
        err += xval[i];
        sumerr += xval[i]*xval[i];
    }
    err /= (double) num;
    sumerr /= (double) num;
    printf("time/set for %d x-values: %.6g\n", num, wallclock(&start)/num);
    printf("<x>: %.6g    <x**2> - <x>**2: %.15g\n", err, sumerr-(err*err));

    /* special cases */
    puts("testing special cases. system exp2()");
    printf("exp2(0.0)-1.0 = %.15g\n", exp2(0.0)-1.0);
    printf("exp2(1.0)-2.0 = %.15g\n", exp2(1.0)-2.0);
    printf("exp2(-0.5)-1.0/sqrt(2) = %.15g\n", exp2(-0.5)-1.0/sqrt(2.0));
    printf("exp2(0.5)-sqrt(2) = %.15g\n", exp2(0.5)-sqrt(2.0));
    printf("2.0*exp2(-0.5)-exp2(0.5) = %.15g\n", 2.0*exp2(-0.5)-exp2(0.5));
    printf("2.0*exp2(0.0)-exp2(1.0) = %.15g\n", 2.0*exp2(0.0)-exp2(1.0));
    printf("exp2(-1023.0) = %.15g\n", exp2(-1023.0));
    printf("exp2( 1023.0) = %.15g\n", exp2( 1023.0));
    printf("exp2(-1024.0) = %.15g\n", exp2(-1024.0));
    printf("exp2( 1024.0) = %.15g\n", exp2( 1024.0));
    printf("exp2(-2048.0) = %.15g\n", exp2(-2048.0));
    printf("exp2( 2048.0) = %.15g\n", exp2( 2048.0));

    puts("testing special cases. fm_exp2()");
    printf("exp2(0.0)-1.0 = %.15g\n", fm_exp2(0.0)-1.0);
    printf("exp2(1.0)-2.0 = %.15g\n", fm_exp2(1.0)-2.0);
    printf("exp2(-0.5)-1.0/sqrt(2) = %.15g\n", fm_exp2(-0.5)-1.0/sqrt(2.0));
    printf("exp2(0.5)-sqrt(2) = %.15g\n", fm_exp2(0.5)-sqrt(2.0));
    printf("2.0*exp2(-0.5)-exp2(0.5) = %.15g\n", 2.0*fm_exp2(-0.5)-fm_exp2(0.5));
    printf("2.0*exp2(0.0)-exp2(1.0) = %.15g\n", 2.0*fm_exp2(0.0)-fm_exp2(1.0));
    printf("exp2(-1023.0) = %.15g\n", fm_exp2(-1023.0));
    printf("exp2( 1023.0) = %.15g\n", fm_exp2( 1023.0));
    printf("exp2(-2048.0) = %.15g\n", fm_exp2(-2048.0));
    printf("exp2( 2048.0) = %.15g\n", fm_exp2( 2048.0));

    xscale = 1.0/(rep*num);
    puts("getting reference data.");
    memset(res0, 0, num*sizeof(double));
    start = wallclock(NULL);
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res0[i] += exp2(xval[i]);
    }
    printf("time for system exp2(): %8.4fus\n", xscale*wallclock(&start));

    puts("using fm_exp2()");
    memset(res1, 0, num*sizeof(double));
    start = wallclock(NULL);
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res1[i] += fm_exp2(xval[i]);
    }
    printf("time for fm_exp2():     %8.4fus.  ", xscale*wallclock(&start));
    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res1[i]-res0[i]);
        err  = diff/fabs(res0[i]);
        sumerr += err;
    }
    printf("avgerr %.6g\n", sumerr/((double) num));

#if 0
#ifdef __x86_64__
    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res2[i] = amd_exp2(xval[i]);
    }
    printf("time for limM exp2(): %.6g\n", wallclock()-start);
#endif

    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res3[i] = exp2(xval[i]);
    }
    printf("time for default exp2(): %.6g\n", wallclock()-start);


    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res1[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr fm %.6g\n", num, sumerr / ((double) num));

#ifdef __x86_64__
    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res2[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr libM %.6g\n", num, sumerr / ((double) num));
#endif

    puts("testing exp()");

    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res1[i] = fm_exp(xval[i]);
    }
    printf("time for fm exp(): %.6g\n", wallclock()-start);

#ifdef __x86_64__
    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res2[i] = amd_exp(xval[i]);
    }
    printf("time for limM exp(): %.6g\n", wallclock()-start);
#endif

    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res3[i] = exp(xval[i]);
    }
    printf("time for default exp(): %.6g\n", wallclock()-start);

    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res1[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr fm %.6g\n", num, sumerr / ((double) num));

#ifdef __x86_64__
    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res2[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr libM %.6g\n", num, sumerr / ((double) num));
#endif

    puts("testing exp10()");

    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res1[i] = fm_exp10(xval[i]);
    }
    printf("time for fm exp10(): %.6g\n", wallclock()-start);

#ifdef __x86_64__
    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res2[i] = amd_exp10(xval[i]);
    }
    printf("time for limM exp10(): %.6g\n", wallclock()-start);
#endif

    start = wallclock();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res3[i] = exp10(xval[i]);
    }
    printf("time for default exp10(): %.6g\n", wallclock()-start);

    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res1[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr fm %.6g\n", num, sumerr / ((double) num));

#ifdef __x86_64__
    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabs(res2[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr libM %.6g\n", num, sumerr / ((double) num));
#endif

#endif
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
