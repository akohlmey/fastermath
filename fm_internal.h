
#ifndef FM_INTERNAL_H
#define FM_INTERNAL_H

/* internal definitions for the fastermath library */

/* unions to manipulate IEEE 754 floating point data */
typedef union 
{
    float         f;
    struct {
        unsigned int m:23;
        unsigned int e:8;
        unsigned int n:1;
    } b;
}  ufi_t;
#define FM_FLOAT_BIAS 0x7f
#define FM_INFINITYF (__builtin_inff())

#define FM_LN2F    6.9314718055994530941e-1f /* ln(2) */
#define FM_1BYLN2F 1.44269504088896340737f   /* 1/ln(2) */

typedef union 
{
    double  f;
    struct {
        unsigned int m1:32;
        unsigned int m0:20;
        unsigned int e:11;
        unsigned int n:1;
    } b;
}  udi_t;
#define FM_DOUBLE_BIAS 0x03ff
#define FM_INFINITY (__builtin_inf())

#define FM_LN2    6.9314718055994530941e-1 /* ln(2) */
#define FM_1BYLN2 1.44269504088896340737   /* 1/ln(2) */

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
