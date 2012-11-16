
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
