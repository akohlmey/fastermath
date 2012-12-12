#ifndef FASTERMATH_H
#define FASTERMATH_H

#ifndef __GNUC__
#error "this library only works with the GNU C compilers"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes for the exported interface */
double fm_exp2(double) __attribute__ ((pure,nothrow));
float fm_exp2f(float) __attribute__ ((pure,nothrow));

#ifdef __cplusplus
}
#endif

#endif /* FASTERMATH_H */

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make"
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
