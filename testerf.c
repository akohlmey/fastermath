
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "fastermath.h"

#define FM_DATA_ALIGN 32

static double walltime(void) 
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (double) ts.tv_nsec * 1.0e-9 + (double) ts.tv_sec;
}
    
int main(int argc, char **argv)
{
    double start, diff, err, sumerr;
    float xscale;
    float *xnums, *res1, *res2, *res3;
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
    xscale = 1.0f / ((float) RAND_MAX);

    posix_memalign((void **)&xnums, FM_DATA_ALIGN, num*sizeof(float));
    posix_memalign((void **)&res1 , FM_DATA_ALIGN, num*sizeof(float));
    posix_memalign((void **)&res2 , FM_DATA_ALIGN, num*sizeof(float));
    posix_memalign((void **)&res3 , FM_DATA_ALIGN, num*sizeof(float));

    puts("testing exp2()");

    start = walltime();
    for (i=0; i < num; ++i) {
        float r1,r2, rsq;
        r1 = xscale * ((float) rand());
        r2 = xscale * ((float) rand());
        rsq = sqrtf(r1*r1 + r2*r2);
        xnums[i] = 126.0f * rsq  - 63.0f;
    }
    printf("time for %d x values: %.6g\n", num, walltime()-start);

    start = walltime();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res1[i] = fm_exp2f(xnums[i]);
    }
    printf("time for fm exp2f(): %.6g\n", walltime()-start);

#ifdef __x86_64__
    start = walltime();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res2[i] = amd_exp2f(xnums[i]);
    }
    printf("time for limM exp2f(): %.6g\n", walltime()-start);
#endif

    start = walltime();
    for (j=0; j < rep; ++j) {
        for (i=0; i < num; ++i)
            res3[i] = exp2f(xnums[i]);
    }
    printf("time for default exp2(): %.6g\n", walltime()-start);

    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabsf(res1[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr fm %.6g\n", num, sumerr / ((double) num));

#ifdef __x86_64__
    sumerr = 0.0;
    for (i=0; i < num; ++i) {
        diff = fabsf(res2[i]-res3[i]);
        err  = diff/fabs(res3[i]);
        sumerr += err;
    }
    printf("%d tests | avgerr libM %.6g\n", num, sumerr / ((double) num));
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
