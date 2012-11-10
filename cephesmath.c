/** selected mathematical functions from the cephes math library */

#include "cephesmath.h"

/*
  Cephes Math Library Release 2.3:  March, 1995
  Copyright 1984, 1995 by Stephen L. Moshier

  Adaptation for use in mathwrap by Axel Kohlmeyer
*/

#define CEPHES_EXPMSK 0x800f
#define CEPHES_MEXP 0x7ff
#define CEPHES_NBITS 53
#define CEPHES_SQRTH 0.70710678118654752440

static double cephes_nan=0.0;
static double cephes_inf=0.0;
static double cephes_loge2=0.0;
static double cephes_maxlog=0.0;
static double cephes_minlog=0.0;
static double cephes_maxnum=0.0;
static double cephes_negzero=0.0;

void cephes_init(void)
{
    union { double d; unsigned short s[4]; } u;

    /* initialize bitpatterns */
    u.s[0] = u.s[1] = u.s[2] = 0x0000;
    u.s[3] = 0x7ff0;
    cephes_inf = u.d;

    u.s[3] = 0x7ffc;
    cephes_nan = u.d;

    u.s[3] = 0x8000;
    cephes_negzero = u.d;

    u.s[0] = u.s[1] = u.s[2] = 0xffff;
    u.s[3] = 0x7fef;
    cephes_maxnum = u.d;

    
    u.s[0] = 0x39ef;
    u.s[1] = 0xfefa;
    u.s[2] = 0x2e42;
    u.s[3] = 0x4086;
    cephes_maxlog = u.d;

    u.s[0] = 0x3052;
    u.s[1] = 0xd52d;
    u.s[2] = 0x4910;
    u.s[3] = 0xc087;
    cephes_minlog = u.d;

    u.s[0] = 0x82fe;
    u.s[1] = 0x652b;
    u.s[2] = 0x1547;
    u.s[3] = 0x3ff7;
    cephes_loge2 = u.d;
}

/*							polevl.c
 *							p1evl.c
 *
 *	Evaluate polynomial
 *
 *
 *
 * SYNOPSIS:
 *
 * int N;
 * double x, y, coef[N+1], polevl[];
 *
 * y = polevl( x, coef, N );
 *
 *
 *
 * DESCRIPTION:
 *
 * Evaluates polynomial of degree N:
 *
 *                     2          N
 * y  =  C  + C x + C x  +...+ C x
 *        0    1     2          N
 *
 * Coefficients are stored in reverse order:
 *
 * coef[0] = C  , ..., coef[N] = C  .
 *            N                   0
 *
 *  The function p1evl() assumes that coef[N] = 1.0 and is
 * omitted from the array.  Its calling arguments are
 * otherwise the same as polevl().
 *
 *
 * SPEED:
 *
 * In the interest of speed, there are no checks for out
 * of bounds arithmetic.  This routine is used by most of
 * the functions in the library.  Depending on available
 * equipment features, the user may wish to rewrite the
 * program in microcode or assembly language.
 *
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

static double cephes_polevl(const double x, const double coef[], const int N)
{
    double ans;
    const double *p;
    int i;

    p = coef;
    ans = *p++;
    i = N;

    do { ans = ans * x  +  *p++; } while (--i);

    return ans;
}

/*							p1evl()	*/
/*                                          N
 * Evaluate polynomial when coefficient of x  is 1.0.
 * Otherwise same as polevl.
 */

static double cephes_p1evl(const double x, const double coef[], const int N)
{
    const double *p;
    double ans;
    int i;

    p = coef;
    ans = x + *p++;
    i = N-1;

    do { ans = ans * x  + *p++; } while (--i);

    return ans;
}

/* Find a multiple of 1/16 that is within 1/16 of x. */
static double cephes_reduc(const double x)
{
    double t;

    t = __builtin_ldexp(x, 4);
    t = __builtin_floor(t);
    t = __builtin_ldexp(t, -4);
    return t;
}

/*							log.c
 *
 *	Natural logarithm
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, log();
 *
 * y = log( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base e (2.718...) logarithm of x.
 *
 * The argument is separated into its exponent and fractional
 * parts.  If the exponent is between -1 and +1, the logarithm
 * of the fraction is approximated by
 *
 *     log(1+x) = x - 0.5 x**2 + x**3 P(x)/Q(x).
 *
 * Otherwise, setting  z = 2(x-1)/x+1),
 * 
 *     log(x) = z + z**3 P(z)/Q(z).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0.5, 2.0    150000      1.44e-16    5.06e-17
 *    IEEE      +-MAXNUM    30000       1.20e-16    4.78e-17
 *    DEC       0, 10       170000      1.8e-17     6.3e-18
 *
 * In the tests over the interval [+-MAXNUM], the logarithms
 * of the random arguments were uniformly distributed over
 * [0, MAXLOG].
 *
 * ERROR MESSAGES:
 *
 * log singularity:  x = 0; returns -INFINITY
 * log domain:       x < 0; returns NAN
 */

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1995, 2000 by Stephen L. Moshier
*/

/* Coefficients for log(1+x) = x - x**2/2 + x**3 P(x)/Q(x)
 * 1/sqrt(2) <= x < sqrt(2)
 */
static unsigned short LOG_P[] = {
    0x1bb0,0x93c3,0xb4c2,0x3f1a,
    0x52f2,0x3f56,0xd6f5,0x3fdf,
    0x6911,0xed92,0xd2ba,0x4012,
    0xeb2e,0xc63e,0xff72,0x402c,
    0xc84d,0x924b,0xefd6,0x4031,
    0xdcf8,0x7d7e,0xd563,0x401e,
};

static unsigned short LOG_Q[] = {
  /*0x0000,0x0000,0x0000,0x3ff0,*/
    0xef8e,0xae97,0x9320,0x4026,
    0xc033,0x4e19,0x9d2c,0x4046,
    0xbdbd,0xa326,0xbf33,0x4054,
    0xae21,0xeb5e,0xc9e2,0x4051,
    0x25b2,0x9e1f,0x200a,0x4037,
};

/* Coefficients for log(x) = z + z**3 P(z)/Q(z),
 * where z = 2(x-1)/(x+1)
 * 1/sqrt(2) <= x < sqrt(2)
 */

static unsigned short LOG_R[12] = {
    0x0e84,0xdc6c,0x443d,0xbfe9,
    0x7b6b,0x7302,0x62fc,0x4030,
    0x2a20,0x1122,0x0906,0xc050,
};

static unsigned short LOG_S[12] = {
  /*0x0000,0x0000,0x0000,0x3ff0,*/
    0x6d0a,0x43ec,0xd60d,0xc041,
    0xe40e,0x112a,0x8180,0x4073,
    0x3f3b,0x19b3,0x0d89,0xc088,
};

double cephes_log(double x)
{
    int e;
    double y, z;

    if (__builtin_isnan(x))
        return x;

    if (!__builtin_isfinite(x))
        return x;

    /* Test for domain */
    if (x <= 0.0) {
        if (x == 0.0) {
            return -cephes_inf;
        } else {
            return cephes_nan;
        }
    }

    /* separate mantissa from exponent */

    x = __builtin_frexp(x, &e);
    /*
      q = (short *)&x;
      q += 3;
      e = *q;
      e = ((e >> 4) & 0x0fff) - 0x3fe;
      *q &= 0x0f;
      *q |= 0x3fe0;
      */

    /* logarithm using log(x) = z + z**3 P(z)/Q(z),
     * where z = 2(x-1)/x+1)
     */

    if ( (e > 2) || (e < -2) ) {
        if (x < CEPHES_SQRTH) {
            /* 2( 2x-1 )/( 2x+1 ) */
            e -= 1;
            z = x - 0.5;
            y = 0.5 * z + 0.5;
        } else {
            /*  2 (x-1)/(x+1)   */
            z = x - 0.5;
            z -= 0.5;
            y = 0.5 * x  + 0.5;
        }

        x = z / y;

        /* rational form */
        z = x*x;
        z = x * ( z * cephes_polevl( z, (const double *)LOG_R, 2)
                  / cephes_p1evl( z, (const double *)LOG_S, 3 ) );
        y = e;
        z = z - y * 2.121944400546905827679e-4;
        z = z + x;
        z = z + e * 0.693359375;
        goto ldone;
    }

    /* logarithm using log(1+x) = x - .5x**2 + x**3 P(x)/Q(x) */

    if (x < CEPHES_SQRTH) {
        e -= 1;
        x = __builtin_ldexp(x, 1) - 1.0; /*  2x - 1  */
	} else {
        x = x - 1.0;
	}

    /* rational form */
    z = x*x;
    y = x * ( z * cephes_polevl(x, (const double *)LOG_P, 5)
              / cephes_p1evl(x, (const double *)LOG_Q, 5) );

    if (e)
        y = y - e * 2.121944400546905827679e-4;

    y = y - __builtin_ldexp(z, -1);   /*  y - 0.5 * z  */
    z = x + y;

    if (e)
        z = z + e * 0.693359375;

  ldone:

    return z;
}

/*							pow.c
 *
 *	Power function
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, z, pow();
 *
 * z = pow( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes x raised to the yth power.  Analytically,
 *
 *      x**y  =  exp( y log(x) ).
 *
 * Following Cody and Waite, this program uses a lookup table
 * of 2**-i/16 and pseudo extended precision arithmetic to
 * obtain an extra three bits of accuracy in both the logarithm
 * and the exponential.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -26,26       30000      4.2e-16      7.7e-17
 *    DEC      -26,26       60000      4.8e-17      9.1e-18
 * 1/26 < x < 26, with log(x) uniformly distributed.
 * -26 < y < 26, y uniformly distributed.
 *    IEEE     0,8700       30000      1.5e-14      2.1e-15
 * 0.99 < x < 1.01, 0 < y < 8700, uniformly distributed.
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * pow overflow     x**y > MAXNUM      INFINITY
 * pow underflow   x**y < 1/MAXNUM       0.0
 * pow domain      x<0 and y noninteger  0.0
 *
 */

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1995, 2000 by Stephen L. Moshier
*/

static unsigned short POW_P[] = {
    0x5cf0,0x7f5b,0xdb99,0x3fdf,
    0xdf15,0xea9e,0xddef,0x400d,
    0xeb6f,0x7f78,0xccbd,0x401e,
    0x9b74,0xb65c,0xaa83,0x4012,
};

static unsigned short POW_Q[] = {
  /*0x0000,0x0000,0x0000,0x3ff0,*/
    0x914e,0x9b20,0xaab4,0x4022,
    0xc9f5,0x41c1,0xffff,0x403b,
    0x6402,0x1b17,0xccbc,0x4040,
    0xe92e,0x918a,0xffc5,0x402b,
};

static unsigned short POW_A[] = {
    0x0000,0x0000,0x0000,0x3ff0,
    0x90da,0xa2a4,0xa4af,0x3fee,
    0xa487,0xdcfb,0x5818,0x3fed,
    0x529c,0xdd85,0x199b,0x3fec,
    0xd3ad,0x995a,0xe89f,0x3fea,
    0xf090,0x82a3,0xc491,0x3fe9,
    0xa0db,0x422a,0xace5,0x3fe8,
    0x0187,0x73eb,0xa114,0x3fe7,
    0x3bcd,0x667f,0xa09e,0x3fe6,
    0x5429,0xdd48,0xab07,0x3fe5,
    0x2a27,0xd536,0xbfda,0x3fe4,
    0x3422,0x4c12,0xdea6,0x3fe3,
    0xb715,0x0a31,0x06fe,0x3fe3,
    0x6238,0x6e75,0x387a,0x3fe2,
    0x517b,0x3c7d,0x72b8,0x3fe1,
    0x890f,0x6cf9,0xb558,0x3fe0,
    0x0000,0x0000,0x0000,0x3fe0
};

static unsigned short POW_B[] = {
    0x0000,0x0000,0x0000,0x0000,
    0x3707,0xd75b,0xed02,0x3c72,
    0xcc81,0x345d,0xa1cd,0x3c87,
    0x4b27,0x5686,0xe9f1,0x3c86,
    0x6456,0x13b2,0xdd34,0xbc8b,
    0x42e2,0xafec,0x4397,0x3c6d,
    0x82e4,0xd231,0xf46a,0x3c76,
    0x8a76,0xb9d7,0x9041,0xbc71,
    0x0000,0x0000,0x0000,0x0000
};

static unsigned short POW_R[] = {
    0x937f,0xd7f2,0x6307,0x3eef,
    0x9259,0x60fc,0x2fbe,0x3f24,
    0xef1d,0xc84a,0xd87e,0x3f55,
    0x33b7,0x6ef1,0xb2ab,0x3f83,
    0x1a92,0xd704,0x6b08,0x3fac,
    0xc56d,0xff82,0xbfbd,0x3fce,
    0x39ef,0xfefa,0x2e42,0x3fe6
};

#define douba(k) (*(double *)&POW_A[(k)<<2])
#define doubb(k) (*(double *)&POW_B[(k)<<2])
#define POW_MEXP   16383.0
#define POW_MNEXP -17183.0

/* log2(e) - 1 */
#define LOG2EA 0.44269504088896340736

#define F W
#define Fa Wa
#define Fb Wb
#define G W
#define Ga Wa
#define Gb u
#define H W
#define Ha Wb
#define Hb Wb

double cephes_pow(double x, double y)
{
    double w, z, W, Wa, Wb, ya, yb, u;
    double aw, ay, wy;
    int e, i, nflg, iyflg, yoddint;

    if (y == 0.0)
        return 1.0;

    if (__builtin_isnan(x))
        return x;

    if (__builtin_isnan(y))
        return y;

    if (y == 1.0)
        return x;

    if (!__builtin_isfinite(y) && (x == 1.0 || x == -1.0))
        return cephes_nan;

    if (x == 1.0)
        return 1.0;

    if (y >= cephes_maxnum) {
        if (x > 1.0)
            return cephes_inf;
        if (x > 0.0 && x < 1.0)
            return 0.0;
        if( x < -1.0)
            return cephes_inf;
        if( x > -1.0 && x < 0.0)
            return 0.0;
	}

    if (y <= -cephes_maxnum) {
        if (x > 1.0)
            return 0.0;
        if (x > 0.0 && x < 1.0)
            return cephes_inf;
        if (x < -1.0)
            return 0.0;
        if (x > -1.0 && x < 0.0)
            return cephes_inf;
	}

    if (x >= cephes_maxnum) {
        if (y > 0.0)
            return cephes_inf;
        return 0.0;
	}

    /* Set iyflg to 1 if y is an integer.  */
    iyflg = 0;
    w = __builtin_floor(y);
    if (w == y)
        iyflg = 1;

    /* Test for odd integer y.  */
    yoddint = 0;
    if (iyflg) {
        ya = __builtin_fabs(y);
        ya = __builtin_floor(0.5 * ya);
        yb = 0.5 * __builtin_fabs(w);
        if (ya != yb)
            yoddint = 1;
	}

    if (x <= -cephes_maxnum) {
        if (y > 0.0) {
            if (yoddint)
                return  -cephes_inf;

            return cephes_inf;
		}

        if (y < 0.0) {
            if (yoddint)
                return cephes_negzero;
            return 0.0;
		}
 	}

    nflg = 0;	/* flag = 1 if x<0 raised to integer power */
    if (x <= 0.0) {
        if (x == 0.0) {
            if (y < 0.0) {
                if (__builtin_signbit(x) && yoddint)
                    return -cephes_inf;

                return cephes_inf;
			}

            if (y > 0.0) {
                if (__builtin_signbit(x) && yoddint)
                    return cephes_negzero;

                return 0.0;
			}
            return 1.0;
		} else {
            if (iyflg == 0)
                /* noninteger power of negative number */
                return cephes_nan;
            
            nflg = 1;
        }
    }
    

    /* Integer power of an integer.  */

    if (iyflg) {
        i = w;
        w = __builtin_floor(x);
        if ((w == x) && (__builtin_fabs(y) < 32768.0)) {
            w = cephes_powi( x, (int) y);
            return w;
        }
    }

    if (nflg)
        x = __builtin_fabs(x);

    /* For results close to 1, use a series expansion.  */
    w = x - 1.0;
    aw = __builtin_fabs(w);
    ay = __builtin_fabs(y);
    wy = w * y;
    ya = __builtin_fabs(wy);
    if((aw <= 1.0e-3 && ay <= 1.0)
       || (ya <= 1.0e-3 && ay >= 1.0)) {
        z = (((((w*(y-5.)/720. + 1./120.)*w*(y-4.) + 1./24.)*w*(y-3.)
               + 1./6.)*w*(y-2.) + 0.5)*w*(y-1.) )*wy + wy + 1.;
        goto done;
    }

    /* separate significand from exponent */
    x = __builtin_frexp(x, &e);

    /* Find significand of x in antilog table A[]. */
    i = 1;
    if (x <= douba(9))
        i = 9;
    if (x <= douba(i+4))
        i += 4;
    if (x <= douba(i+2))
        i += 2;
    if (x >= douba(1))
        i = -1;
    i += 1;

    /* Find (x - A[i])/A[i]
     * in order to compute log(x/A[i]):
     *
     * log(x) = log( a x/a ) = log(a) + log(x/a)
     *
     * log(x/a) = log(1+v),  v = x/a - 1 = (x-a)/a
     */
    x -= douba(i);
    x -= doubb(i/2);
    x /= douba(i);

    /* rational approximation for log(1+v):
     *
     * log(1+v)  =  v  -  v**2/2  +  v**3 P(v) / Q(v)
     */
    z = x*x;
    w = x * ( z * cephes_polevl(x, (const double *)POW_P, 3) 
              / cephes_p1evl(x, (const double *)POW_Q, 4) );
    w = w - __builtin_ldexp(z, -1);   /*  w - 0.5 * z  */

    /* Convert to base 2 logarithm:
     * multiply by log2(e) */
    w = w + LOG2EA * w;
    /* Note x was not yet added in
     * to above rational approximation,
     * so do it now, while multiplying
     * by log2(e). */
    z = w + LOG2EA * x;
    z = z + x;

    /* Compute exponent term of the base 2 logarithm. */
    w = -i;
    w = __builtin_ldexp(w, -4);	/* divide by 16 */
    w += e;
    /* Now base 2 log of x is w + z. */

    /* Multiply base 2 log by y, in extended precision. */

    /* separate y into large part ya
     * and small part yb less than 1/16 */
    ya = cephes_reduc(y);
    yb = y - ya;

    F = z * y  +  w * yb;
    Fa = cephes_reduc(F);
    Fb = F - Fa;

    G = Fa + w * ya;
    Ga = cephes_reduc(G);
    Gb = G - Ga;

    H = Fb + Gb;
    Ha = cephes_reduc(H);
    w = __builtin_ldexp(Ga+Ha, 4);

    /* Test the power of 2 for overflow */
    if (w > POW_MEXP) {
        if (nflg && yoddint)
            return -cephes_inf;
        return cephes_inf;
    }

    if (w < (POW_MNEXP - 1)) {
        if (nflg && yoddint)
            return cephes_negzero;

        return 0.0;
    }

    e = w;
    Hb = H - Ha;

    if (Hb > 0.0) {
        e += 1;
        Hb -= 0.0625;
    }

    /* Now the product y * log2(x)  =  Hb + e/16.0.
     *
     * Compute base 2 exponential of Hb,
     * where -0.0625 <= Hb <= 0. */
    z = Hb * cephes_polevl( Hb, (const double *)POW_R, 6);  /*    z  =  2**Hb - 1    */

    /* Express e/16 as an integer plus a negative number of 16ths.
     * Find lookup table entry for the fractional power of 2. */
    if (e < 0)
        i = 0;
    else
        i = 1;

    i = e/16 + i;
    e = 16*i - e;
    w = douba(e);
    z = w + w * z;      /*    2**-e * ( 1 + (2**Hb-1))    */
    z = __builtin_ldexp(z, i);  /* multiply by integer power of 2 */

  done:

    /* Negate if odd integer power of negative number */
    if (nflg && yoddint) {
        if (z == 0.0)
            z = cephes_negzero;
        else
            z = -z;
    }
    return z;
}

/*							powi.c
 *
 *	Real raised to integer power
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, powi();
 * int n;
 *
 * y = powi( x, n );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns argument x raised to the nth power.
 * The routine efficiently decomposes n as a sum of powers of
 * two. The desired power is a product of two-to-the-kth
 * powers of x.  Thus to compute the 32767 power of x requires
 * 28 multiplications instead of 32767 multiplications.
 *
 *
 *
 * ACCURACY:
 *
 *
 *                      Relative error:
 * arithmetic   x domain   n domain  # trials      peak         rms
 *    DEC       .04,26     -26,26    100000       2.7e-16     4.3e-17
 *    IEEE      .04,26     -26,26     50000       2.0e-15     3.8e-16
 *    IEEE        1,2    -1022,1023   50000       8.6e-14     1.6e-14
 *
 * Returns MAXNUM on overflow, zero on underflow.
 *
 */

/*							powi.c	*/

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1995, 2000 by Stephen L. Moshier
*/

double cephes_powi(double x, int nn)
{
    int n, e, sign, asign, lx;
    double w, y, s;

    /* See pow.c for these tests.  */
    if (x == 0.0) {
        if (nn == 0)
            return 1.0;
        else if (nn < 0)
            return cephes_inf;
        else {
            if (nn & 1)
                return x;
            else
                return 0.0;
        }
	}

    if (nn == 0)
        return 1.0;

    if (nn == -1)
        return 1.0/x;

    if (x < 0.0) {
        asign = -1;
        x = -x;
	} else
        asign = 0;

    if (nn < 0)	{
        sign = -1;
        n = -nn;
	} else {
        sign = 1;
        n = nn;
	}

    /* Even power will be positive. */
    if ((n & 1) == 0)
        asign = 0;

    /* Overflow detection */

    /* Calculate approximate logarithm of answer */
    s = __builtin_frexp( x, &lx);
    e = (lx - 1)*n;
    if ((e == 0) || (e > 64) || (e < -64)) {
        s = (s - 7.0710678118654752e-1) / (s +  7.0710678118654752e-1);
        s = (2.9142135623730950 * s - 0.5 + lx) * nn * cephes_loge2;
	} else {
        s = cephes_loge2 * e;
	}

    if (s > cephes_maxlog) {
        y = cephes_inf;
        goto done;
	}

    if (s < cephes_minlog) {
        y = 0.0;
        goto done;
	}

    /* Handle tiny denormal answer, but with less accuracy
     * since roundoff error in 1.0/x will be amplified.
     * The precise demarcation should be the gradual underflow threshold. */
    if ((s < (-cephes_maxlog+2.0)) && (sign < 0)) {
        x = 1.0/x;
        sign = -sign;
	}


    /* First bit of the power */
    if (n & 1)
        y = x;
    else
        y = 1.0;

    w = x;
    n >>= 1;
    while(n) {
        w = w * w;	/* arg to the 2-to-the-kth power */
        if (n & 1)	/* if that bit is set, then include in product */
            y *= w;
        n >>= 1;
	}

    if (sign < 0)
        y = 1.0/y;

  done:

    if (asign) {
        /* odd power of negative number */
        if (y == 0.0)
            y = cephes_negzero;
        else
            y = -y;
	}
    return y;
}
