
#ifndef FM_INTERNAL_H
#define FM_INTERNAL_H

#include <time.h>
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
#define FM_FLOAT_BIAS 0x7fU
#define FM_INFINITYF (__builtin_inff())

#define FM_LN2F 6.9314718055994530941e-1f /* ln(2) = 1/ld(e) */
#define FM_LDEF 1.44269504088896340737f   /* ld(e) = 1/ln(2) */
#define FM_LOG10_2F 3.01029995663981195214e-1f /* log_10(2) = 1/log_10(2) */
#define FM_LOG2_10F 3.32192809488736234787e0f  /* log_2(10) = 1/log_2(10) */

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
#define FM_DOUBLE_BIAS 0x03ffU
#define FM_INFINITY (__builtin_inf())

#define FM_LN2 6.9314718055994530941e-1 /* ln(2) = 1/ld(e) */
#define FM_LDE 1.44269504088896340737   /* ld(e) = 1/ln(2) */
#define FM_LOG10_2 3.01029995663981195214e-1 /* log_10(2) = 1/log_2(10) */
#define FM_LOG2_10 3.32192809488736234787e0  /* log_2(10) = 1/log_10(2) */

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
