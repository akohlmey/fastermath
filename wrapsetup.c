
/** mathwrap - optimized math functions replacements
 *
 * A shared library wrapper for 32-bit and 64-bit x86 Linux systems to
 * redirect calls to math functions from the system libm to bundled SSE
 * or AVX optimized versions or from external math libraries.
 *
 * \version 0.1
 * \author Copyright &copy; 2012 Axel Kohlmeyer, akohlmey@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "cephesmath.h"

/** struct to collect math function call statistics */
static struct _callstats
{
    int log_hi;   /**< log() call counter; high 30 bits */
    int log_lo;   /**< log() call counter; low 30 bits  */
    int exp_hi;   /**< exp() call counter; high 30 bits */
    int exp_lo;   /**< exp() call counter; low 30 bits  */
    int pow_hi;   /**< pow() call counter; high 30 bits */
    int pow_lo;   /**< pow() call counter; low 30 bits  */
} callstats;

/* function pointers for the selected math function */
static double (*my_exp)(double) = NULL;
static double (*my_log)(double) = NULL;
static double (*my_pow)(double,double) = NULL;

/* this makes sure, that initialization is done only once. */
static int init_done = 0;

/* handle to optimized math library shared object */
static void *math_handle = NULL;

/* have the init and cleanup functions called automatically */
void _mathwrap_init(void) __attribute__ ((constructor(101)));
void _mathwrap_fini(void) __attribute__ ((destructor(1024)));

/** Library initialization.
 *
 * This function is called automatically, when the the package
 * is loaded as a shared library, e.g. via LD_PRELOAD.
 * That is before \b main \b is called. When  */
void _mathwrap_init(void)
{
    /* initialization may only be done once */
    if (init_done) return;
    init_done = 1;

    fputs("\nInitializing mathwrap v0.1\n========================\n", stderr);
    memset(&callstats,0,sizeof(struct _callstats));

    /* initialize local optimized math libs */
    cephes_init();

    math_handle = dlopen("libamdlibm.so",RTLD_NOW);
    if (math_handle != NULL) {
        fputs("Redirecting exp() to AMD libM version\n", stderr);
        my_exp = dlsym(math_handle,"amd_exp");
        fputs("Redirecting log() to AMD libM version\n", stderr);
        my_log = dlsym(math_handle,"amd_log");
        fputs("Redirecting pow() to AMD libM version\n", stderr);
        my_pow = dlsym(math_handle,"amd_pow");
    } else {
        math_handle = dlopen("libm.so.6",RTLD_NOW);
        fputs("Redirecting exp() to glibc version\n", stderr);
        my_exp = dlsym(math_handle,"exp");
        fputs("Redirecting log() to glibc version\n", stderr);
        my_log = dlsym(math_handle,"log");
        fputs("Redirecting pow() to glibc version\n", stderr);
        my_pow = dlsym(math_handle,"pow");
    }

    /* fall back to local cephes versions which are slowest */
    if (my_log == NULL) {
        fputs("Redirecting log() to Cephes version\n", stderr);
        my_log = &cephes_log;
    }
    if (my_pow == NULL) {
        fputs("Redirecting pow() to Cephes version\n", stderr);
        my_pow = &cephes_pow;
    }

    fputs("========================\n", stderr);
}


/** Clean up and print stats
 *
 * This is function should be called during the destructor
 * processing of executables. */
void _mathwrap_fini(void)
{
    double numcalls;

    if (math_handle != NULL) dlclose(math_handle);

    fputs("========================\n",stderr);
    numcalls = (double) callstats.exp_lo;
    numcalls += ((double) callstats.exp_hi) * ((double) (1<<30));
    fprintf(stderr,"Total calls to 'exp()'  : %20.16g\n", numcalls);
    numcalls = (double) callstats.log_lo;
    numcalls += ((double) callstats.log_hi) * ((double) (1<<30));
    fprintf(stderr,"Total calls to 'log()'  : %20.16g\n", numcalls);
    numcalls = (double) callstats.pow_lo;
    numcalls += ((double) callstats.pow_hi) * ((double) (1<<30));
    fprintf(stderr,"Total calls to 'pow()'  : %20.16g\n", numcalls);
    fputs("========================\n",stderr);
}

/** double precision exponential */
double exp(double x) 
{
    ++ callstats.exp_lo;
    if (callstats.exp_lo & 1<<30) {
        callstats.exp_lo = 0;
        ++ callstats.exp_hi;
    }

    return (*my_exp)(x);
}

/** double precision power */

/** double precision natural logarithm */
double log(double x) 
{
    ++ callstats.log_lo;
    if (callstats.log_lo & 1<<30) {
        callstats.log_lo = 0;
        ++ callstats.log_hi;
    }

    return (*my_log)(x);
}

/** double precision power */
double pow(double x, double y) 
{
    ++ callstats.pow_lo;
    if (callstats.pow_lo & 1<<30) {
        callstats.pow_lo = 0;
        ++ callstats.pow_hi;
    }

    return (*my_pow)(x,y);
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
