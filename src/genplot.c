/* 
   Copyright (c) 2012,2013,2016   Axel Kohlmeyer <akohlmey@gmail.com> 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of the <organization> nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define _GNU_SOURCE 1
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

extern double exp10(double);
extern float exp10f(float);

#include "fastermath.h"

#include "config.c"

int main(int argc, char **argv)
{
    FILE *fp;
    double xscale;
    int num, i;

    puts("\n============================="
         "\nfastermath function plotting"
         "\n=============================");
    puts(config);

    if (argc < 2) {
        printf("usage %s <num data points>\n",argv[0]);
        return -1;
    }

    num = atoi(argv[1]);

    fputs("-------------------------\n"
          "plotting exp2(x)",stdout);
    xscale = 1000.0 / ((double) 2*num);

    fp = fopen("exp2.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, exp2(x), fm_exp2(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = (double)i * xscale;
        double y1 = exp2(x);
        double y2 = fm_exp2(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = (double)i * xscale;
        double y1 = exp2(x);
        double y2 = fm_exp2(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm exp2() vs. fm_exp2()");
    printf("exp2(0.0)-1.0 = %.20g\n", exp2(0.0)-1.0);
    printf("fm_exp2(0.0)-1.0 = %.20g\n", fm_exp2(0.0)-1.0);
    printf("exp2(1.0)-2.0 = %.20g\n", exp2(1.0)-2.0);
    printf("fm_exp2(1.0)-2.0 = %.20g\n", fm_exp2(1.0)-2.0);
    printf("exp2(-1.0)-0.5 = %.20g\n", exp2(-1.0)-0.5);
    printf("fm_exp2(-1.0)-0.5 = %.20g\n", fm_exp2(-1.0)-0.5);
    printf("exp2(20.0)-1048576 = %.20g\n", exp2(20.0)-1048576);
    printf("fm_exp2(20.0)-1048576 = %.20g\n", fm_exp2(20.0)-1048576);

    fputs("-------------------------\n"
          "plotting exp(x)",stdout);
    xscale = 1400.0 / ((double) 2*num);

    fp = fopen("exp.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, exp(x), fm_exp(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = (double)i * xscale;
        double y1 = exp(x);
        double y2 = fm_exp(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = (double)i * xscale;
        double y1 = exp(x);
        double y2 = fm_exp(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm exp() vs. fm_exp()");
    printf("exp(0.0)-1.0 = %.20g\n", exp(0.0)-1.0);
    printf("fm_exp(0.0)-1.0 = %.20g\n", fm_exp(0.0)-1.0);
    printf("exp(1.0)*exp(-1.0) = %.20g\n", exp(1.0)*exp(-1.0));
    printf("fm_exp(1.0)*fm_exp(-1.0) = %.20g\n", fm_exp(1.0)*fm_exp(-1.0));

    fputs("-------------------------\n"
          "plotting exp10(x)",stdout);
    xscale = 600.0 / ((double) 2*num);

    fp = fopen("exp10.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, exp10(x), fm_exp10(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = (double)i * xscale;
        double y1 = exp10(x);
        double y2 = fm_exp10(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = (double)i * xscale;
        double y1 = exp10(x);
        double y2 = fm_exp10(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm exp10() vs. fm_exp10()");
    printf("exp10(0.0)-1.0 = %.20g\n", exp10(0.0)-1.0);
    printf("fm_exp10(0.0)-1.0 = %.20g\n", fm_exp10(0.0)-1.0);
    printf("exp10(1.0)-10.0 = %.20g\n", exp10(1.0)-10.0);
    printf("fm_exp10(1.0)-10.0 = %.20g\n", fm_exp10(1.0)-10.0);
    printf("exp10(-1.0)-0.1 = %.20g\n", exp10(-1.0)-0.1);
    printf("fm_exp10(-1.0)-0.1 = %.20g\n", fm_exp10(-1.0)-0.1);
    printf("exp10(1.0)*exp10(-1.0) = %.20g\n", exp10(1.0)*exp10(-1.0));
    printf("fm_exp1-(1.0)*fm_exp10(-1.0) = %.20g\n",
           fm_exp10(1.0)*fm_exp10(-1.0));
    printf("exp10(15.0)-1e15 = %.20g\n", exp10(15.0)-1e15-1.0);
    printf("fm_exp10(15.0)-1e15 = %.20g\n", fm_exp10(15.0)-1e15);


    fputs("-------------------------\n"
          "plotting log2(x)",stdout);
    xscale = 1000.0 / ((double) 2*num);

    fp = fopen("log2.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, log2(x), fm_log2(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log2(x);
        double y2 = fm_log2(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log2(x);
        double y2 = fm_log2(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm log2() vs. fm_log2()");
    printf("log2(0.5)+1.0 = %.20g\n", log2(0.5)+1.0);
    printf("fm_log2(0.5)+1.0 = %.20g\n", fm_log2(0.5)+1.0);
    printf("log2(1.0) = %.20g\n", log2(1.0));
    printf("fm_log2(1.0) = %.20g\n", fm_log2(1.0));
    printf("log2(2.0)-1.0 = %.20g\n", log2(2.0)-1.0);
    printf("fm_log2(2.0)-1.0 = %.20g\n", fm_log2(2.0)-1.0);
    printf("log2(1048576)-20.0 = %.20g\n", log2(1048576.0)-20.0);
    printf("fm_log2(1048576)-20.0 = %.20g\n", fm_log2(1048576.0)-20.0);

    fputs("-------------------------\n"
          "plotting log(x)",stdout);
    xscale = 1400.0 / ((double) 2*num);

    fp = fopen("log.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, log(x), fm_log(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log(x);
        double y2 = fm_log(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log(x);
        double y2 = fm_log(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm log() vs. fm_log()");
    printf("log(1.0) = %.20g\n", log(1.0));
    printf("fm_log(1.0) = %.20g\n", fm_log(1.0));
    printf("log(0.1)-log(10.0) = %.20g\n", log(0.1)+log(10.0));
    printf("fm_log(0.1)-fm_log(10.0) = %.20g\n", fm_log(0.1)+fm_log(10.0));
    printf("log(0.01)-log(100.0) = %.20g\n", log(0.01)+log(100.0));
    printf("fm_log(0.01)-fm_log(100.0) = %.20g\n", fm_log(0.01)+fm_log(100.0));
    printf("log(1/300)-log(300.0) = %.20g\n",
           log(0.003333333333333333333)+log(300.0));
    printf("fm_log(1/300)-fm_log(300.0) = %.20g\n",
           fm_log(0.003333333333333333333)+fm_log(300.0));
    
    fputs("-------------------------\n"
          "plotting log10(x)",stdout);
    xscale = 600.0 / ((double) 2*num);

    fp = fopen("log10.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, log10(x), fm_log10(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=-num; i < 0; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log10(x);
        double y2 = fm_log10(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half 0 to +oo */
    for (i=0; i <= num; ++i) {
        double x = exp2((double)i * xscale);
        double y1 = log10(x);
        double y2 = fm_log10(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm log10() vs. fm_log10()");
    printf("log10(0.0000000001)+10.0 = %.20g\n", log10(0.0000000001)+10.0);
    printf("fm_log10(0.0000000001)+10.0 = %.20g\n", fm_log10(0.0000000001)+10.0);
    printf("log10(1.0) = %.20g\n", log10(1.0));
    printf("fm_log10(1.0) = %.20g\n", fm_log10(1.0));
    printf("log10(10)-1.0 = %.20g\n", log10(10.0)-1.0);
    printf("fm_log10(10.0)-1.0 = %.20g\n", fm_log10(10.0)-1.0);
    printf("log10(1e20)-20.0 = %.20g\n", log10(1e20)-20.0);
    printf("fm_log10(1e20)-20.0 = %.20g\n", fm_log10(1e20)-20.0);

#if 0
    fputs("-------------------------\n"
          "plotting erf(x)",stdout);
    xscale = 20000.0 / ((double) 2*num);

    fp = fopen("erf.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, erf(x), fm_erf(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=1; i < num; ++i) {
        double x = -xscale/((double)i);
        double y1 = erf(x);
        double y2 = fm_erf(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half >0 to +oo */
    for (i=num; i > 0; --i) {
        double x = xscale/((double)i);
        double y1 = erf(x);
        double y2 = fm_erf(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm erf() vs. fm_erf()");
    printf("erf(0.0) - 1.0 = %.20g\n", erf(0.0)-1.0);
    printf("fm_erf(0.0) - 1.0 = %.20g\n", fm_erf(0.0)-1.0);
    printf("erf(-1.0)-erf(1.0) - 2.0 = %.20g\n",
           erf(-1.0)+erf(1.0) - 2.0);
    printf("fm_erf(-1.0)-fm_erf(1.0) - 2.0 = %.20g\n",
           fm_erf(-1.0)+fm_erf(1.0) - 2.0);
    printf("erf(-10.0)/(1.0-erf(10.0)) - 2.0 = %.20g\n",
           (erf(-10.0))/(1.0-erf(10.0)) - 2.0);
    printf("fm_erf(-10.0)/fm_erf(10.0) - 2.0 = %.20g\n",
           fm_erf(-10.0)/(1.0-fm_erf(10.0)) -2.0 );
#endif

    fputs("-------------------------\n"
          "plotting erfc(x)",stdout);
    xscale = 50.0;

    fp = fopen("erfc.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, erfc(x), fm_erfc(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=1; i < num; ++i) {
        double x = -xscale/((double)i);
        double y1 = erfc(x);
        double y2 = fm_erfc(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half >0 to +oo */
    for (i=num; i > 0; --i) {
        double x = xscale/((double)i);
        double y1 = erfc(x);
        double y2 = fm_erfc(x);
        double y3 = (y1 != 0.0) ? fabs((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");

    puts("testing special cases. libm erfc() vs. fm_erfc()");
    printf("erfc(0.0) - 1.0 = %.20g\n", erfc(0.0)-1.0);
    printf("fm_erfc(0.0) - 1.0 = %.20g\n", fm_erfc(0.0)-1.0);
    printf("erfc(-1.0)-erfc(1.0) - 2.0 = %.20g\n",
           erfc(-1.0)+erfc(1.0) - 2.0);
    printf("fm_erfc(-1.0)-fm_erfc(1.0) - 2.0 = %.20g\n",
           fm_erfc(-1.0)+fm_erfc(1.0) - 2.0);
    printf("erfc(-10.0)/(1.0-erfc(10.0)) - 2.0 = %.20g\n",
           (erfc(-10.0))/(1.0-erfc(10.0)) - 2.0);
    printf("fm_erfc(-10.0)/fm_erfc(10.0) - 2.0 = %.20g\n",
           fm_erfc(-10.0)/(1.0-fm_erfc(10.0)) -2.0 );


    fputs("-------------------------\n"
          "plotting erfcf(x)",stdout);
    xscale = 50.0;

    fp = fopen("erfcf.dat","w");
    fputc('.',stdout);
    fprintf(fp,"# %d: x, erfcf(x), fm_erfcf(x), relative error\n",num);
    fputc('.',stdout);
    /* first half -oo to 0 */
    for (i=1; i < num; ++i) {
        float x = -xscale/((double)i);
        float y1 = erfcf(x);
        float y2 = fm_erfcf(x);
        float y3 = (y1 != 0.0) ? fabsf((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);

    /* second half >0 to +oo */
    for (i=num; i > 0; --i) {
        float x = xscale/((double)i);
        float y1 = erfcf(x);
        float y2 = fm_erfcf(x);
        float y3 = (y1 != 0.0) ? fabsf((y2-y1)/y1): 0.0;

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fputc('.',stdout);
    fclose(fp);
    fputc('.',stdout);
    puts(" done");
}

/*
 * Local Variables:
 * mode: c
 * compile-command: "make -C .."
 * c-basic-offset: 4
 * fill-column: 76
 * indent-tabs-mode: nil
 * End:
 */
