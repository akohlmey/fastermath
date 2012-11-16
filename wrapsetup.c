
/** fastermath - optimized math functions replacements
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

/* this makes sure, that initialization is done only once. */
static int init_done = 0;

/* have the init and cleanup functions called automatically */
void _fastermath_init(void) __attribute__ ((constructor(101)));
void _fastermath_fini(void) __attribute__ ((destructor(1024)));

/** Library initialization.
 *
 * This function is called automatically, when the the package
 * is loaded as a shared library, e.g. via LD_PRELOAD.
 * That is before \b main \b is called. When  */
void _fastermath_init(void)
{
    /* initialization may only be done once */
    if (init_done) return;
    init_done = 1;

    fputs("\nInitializing fastermath v0.1\n========================\n", stderr);

    fputs("========================\n", stderr);
}


/** Clean up and print stats
 *
 * This is function should be called during the destructor
 * processing of executables. */
void _fastermath_fini(void)
{

    fputs("========================\n",stderr);
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
