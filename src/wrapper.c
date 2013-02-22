#include "fastermath.h"

double log(double x) { return fm_log_alt(x); }
double log2(double x) { return fm_log2_alt(x); }
/* double log2(double x) { return fm_log2(x); } */

double exp(double x) { return fm_exp(x); }
double exp2(double x) { return fm_exp2(x); }
double exp10(double x) { return fm_exp10(x); }

/* float logf(float x) { return fm_logf(x); } */
/* float log2f(float x) { return fm_log2f(x); } */
/* float log10f(float x) { return fm_log10f(x); } */

float expf(float x) { return fm_expf(x); }
float exp2f(float x) { return fm_exp2f(x); }
float exp10f(float x) { return fm_exp10f(x); }

