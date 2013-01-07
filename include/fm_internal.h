
#ifndef FM_INTERNAL_H
#define FM_INTERNAL_H

#ifndef _FM_INTERNAL
#error use fm_internal.h header only for building the library
#endif 

#include <stdint.h>

/* internal definitions for the fastermath library */

/* IEEE 754 single precision floating point data manipulation */
typedef union 
{
    float    f;
    uint32_t u;
    int32_t  i;
}  ufi_t;
#define FM_FLOAT_BIAS 127

/* generate 2**num in floating point by bitshifting */
#define FM_FLOAT_INIT_EXP(var,num)              \
    var.i = (((int) num) + FM_FLOAT_BIAS) << 23

/* IEEE 754 double precision floating point data manipulation */
typedef union 
{
    double   f;
    uint64_t u;
    int32_t  i[2];
}  udi_t;
#define FM_DOUBLE_BIAS 1023
#define FM_DOUBLE_EMASK 2146435072
#define FM_DOUBLE_EBITS 20

/* generate 2**num in floating point by bitshifting */
#define FM_DOUBLE_INIT_EXP(var,num)                 \
    var.i[0] = 0;                                   \
    var.i[1] = (((int) num) + FM_DOUBLE_BIAS) << 20

/* single precision constants */
#define FM_FLOAT_LOG2OFE  1.4426950408889634074f
#define FM_FLOAT_LOGEOF2  6.9314718055994530942e-1f
#define FM_FLOAT_LOG2OF10 3.32192809488736234789f
#define FM_FLOAT_LOG10OF2 3.0102999566398119521f
#define FM_FLOAT_SQRT2    1.41421356237309504880f
#define FM_FLOAT_SQRTH    0.70710678118654752440f

/* double precision constants */
#define FM_DOUBLE_LOG2OFE  1.4426950408889634074
#define FM_DOUBLE_LOGEOF2  6.9314718055994530942e-1
#define FM_DOUBLE_LOG2OF10 3.32192809488736234789
#define FM_DOUBLE_LOG10OF2 3.0102999566398119521
#define FM_DOUBLE_SQRT2    1.41421356237309504880
#define FM_DOUBLE_SQRTH    0.70710678118654752440

#endif /* FM_INTERNAL_H */

/* 
 * Local Variables:
 * mode: c
 * compile-command: "make"
 * c-basic-offset: 4
 * fill-column: 76 
 * indent-tabs-mode: nil 
 * End: 
 */
