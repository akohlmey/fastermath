
#ifndef FM_INTERNAL_H
#define FM_INTERNAL_H

#ifndef _FM_INTERNAL
#error use fm_internal.h header only for building the library
#endif 

#include <stdint.h>

/* internal definitions for the fastermath library */

/* unions to manipulate IEEE 754 floating point data */
typedef union 
{
    float    f;
    uint32_t u;
    struct {
        unsigned int m:23;
        unsigned int e:8;
        unsigned int n:1;
    } b;
}  ufi_t;
#define FM_FLOAT_BIAS 0x7f

/* single precision constants */
#define FM_FLOAT_LOG2OFE 1.4426950408889634074f
#define FM_FLOAT_LOGEOF2 6.9314718055994530942e-1f

typedef union 
{
    double   f;
    uint64_t u;
    struct {
        unsigned int m1:32;
        unsigned int m0:20;
        unsigned int e:11;
        unsigned int n:1;
    } b;
}  udi_t;
#define FM_DOUBLE_BIAS 0x03ff

/* double precision constants */
#define FM_DOUBLE_LOG2OFE 1.4426950408889634074
#define FM_DOUBLE_LOGEOF2 6.9314718055994530942e-1

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
