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
          "plotting exponentiation functions:",stdout);
    xscale = 1000.0 / ((double) 2*num);
    fputs(" exp2(x)",stdout);

    fp = fopen("exp2.dat","w");
    fprintf(fp,"# %d: x, exp2(x), __builtin_exp2(x), fm_exp2(x)\n",num);
    for (i=-num; i < 0; ++i) {
        double x = (double)i * xscale;
        double y1 = exp2(x);
        double y2 = fm_exp2(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }

    for (i=0; i <= num; ++i) {
        double x = (double)i * xscale;
        double y1 = exp2(x);
        double y2 = fm_exp2(x);
        double y3 = fabs((y2-y1)/y1);

        fprintf(fp,"%24.16g %24.16g %24.16g %24.16g\n",x,y1,y2,y3);
    }
    fclose(fp);
    puts("");

    puts("testing special cases. libm exp2() vs. fm_exp2()");
    printf("exp2(0.0)-1.0 = %.20g\n", exp2(0.0)-1.0);
    printf("exp2(0.0)-1.0 = %.20g\n", fm_exp2(0.0)-1.0);
    printf("exp2(1.0)-2.0 = %.20g\n", exp2(1.0)-2.0);
    printf("exp2(1.0)-2.0 = %.20g\n", fm_exp2(1.0)-2.0);
    printf("exp2(-1.0)-0.5 = %.20g\n", exp2(-1.0)-0.5);
    printf("exp2(-1.0)-0.5 = %.20g\n", fm_exp2(-1.0)-0.5);
    printf("exp2(20.0)-1048576 = %.20g\n", exp2(20.0)-1048576);
    printf("exp2(20.0)-1048576 = %.20g\n", fm_exp2(20.0)-1048576);
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
