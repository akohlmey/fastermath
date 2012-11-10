
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

/** struct to collect math function call statistics */
static struct _callstats
{
    int log_hi; /**< log call counter; high 30 bits */
    int log_lo; /**< log call counter; low 30 bits  */
    int log10_hi; /**< log10 call counter; high 30 bits */
    int log10_lo; /**< log10 call counter; low 30 bits  */
} callstats;

/* function pointers for the selected math function */
static double (*my_log)(double);
static double (*my_log10)(double);

/* this makes sure, that initialization is done only once. */
static int init_done = 0;

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
    void *handle;

    /* initialization may only be done once */
    if (init_done) return;

    init_done = 1;
    fputs("\nInitializing mathwrap v0.1\n========================\n", stderr);
    memset(&callstats,0,sizeof(struct _callstats));

    handle = dlopen("libm.so.6",RTLD_NOW);
    if (handle == NULL) {
        fprintf(stderr,"initializing math library failed. %s\n", dlerror());
        exit(1);
    }

    fputs("Wrapping log()\n", stderr);
    my_log = dlsym(handle,"log");
    fputs("Wrapping log10()\n", stderr);
    my_log10 = dlsym(handle,"log10");
    
    dlclose(handle);
    fputs("========================\n", stderr);

}


/** Clean up and print stats
 *
 * This is function is added as an atexit(3) hook during init
 * and thus called during the (regular) exit of a process. */
void _mathwrap_fini(void)
{
    double numcalls;

    fputs("========================\n",stderr);
    numcalls = (double) callstats.log_lo;
    numcalls += ((double) callstats.log_hi) * ((double) (1<<30));
    fprintf(stderr,"Total calls to 'log()'  : %20.16g\n", numcalls);
    numcalls = (double) callstats.log10_lo;
    numcalls += ((double) callstats.log10_hi) * ((double) (1<<30));
    fprintf(stderr,"Total calls to 'log10()': %20.16g\n", numcalls);
    fputs("========================\n",stderr);
}

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

/** double precision decadic logarithm */
double log10(double x) 
{
    ++ callstats.log10_lo;
    if (callstats.log10_lo & 1<<30) {
        callstats.log10_lo = 0;
        ++ callstats.log10_hi;
    }

    return (*my_log10)(x);
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
