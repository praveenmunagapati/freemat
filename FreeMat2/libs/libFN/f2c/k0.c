/* ../k0.f -- translated by f2c (version 20031025).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Subroutine */ int calck0_(real *arg, real *result, integer *jint)
{
    /* Initialized data */

    static real one = 1.f;
    static real pp[10] = { 113.94980557384778174f,3683.258995734026794f,
	    31075.408980684392399f,105770.68948034021957f,
	    173988.67902565686251f,150976.46353289914539f,
	    71557.062783764037541f,18321.525870183537725f,
	    2344.4738764199315021f,116.00249425076035558f };
    static real qq[10] = { 200.13443064949242491f,4432.9628889746408858f,
	    31474.655750295278825f,97418.829762268075784f,
	    151446.44673520157801f,126898.39587977598727f,
	    58824.616785857027752f,14847.228371802360957f,
	    1882.1890840982713696f,92.556599177304839811f };
    static real zero = 0.f;
    static real xsmall = 5.95e-8f;
    static real xinf = 3.4e38f;
    static real xmax = 85.337f;
    static real p[6] = { 5.85992214128261e-4f,.1316605256498957185f,
	    11.999463724910714109f,468.50901201934832188f,
	    5916.9059852270512312f,2470.8152720399552679f };
    static real q[2] = { -249.94418972832303646f,21312.71430384912038f };
    static real f[4] = { -1.64144528372990641f,-296.01657892958843866f,
	    -17733.784684952985886f,-403203.40761145482298f };
    static real g[3] = { -250.6497244587799273f,29865.713163054025489f,
	    -1612813.6304458193998f };

    /* Builtin functions */
    double log(doublereal), exp(doublereal), sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static real x, xx, temp, sumf, sumg, sump, sumq;

/* -------------------------------------------------------------------- */

/* This packet computes modified Bessel functions of the second kind */
/*   and order zero, K0(X) and EXP(X)*K0(X), for real */
/*   arguments X.  It contains two function type subprograms, BESK0 */
/*   and BESEK0, and one subroutine type subprogram, CALCK0. */
/*   the calling statements for the primary entries are */

/*                   Y=BESK0(X) */
/*   and */
/*                   Y=BESEK0(X) */

/*   where the entry points correspond to the functions K0(X) and */
/*   EXP(X)*K0(X), respectively.  The routine CALCK0 is */
/*   intended for internal packet use only, all computations within */
/*   the packet being concentrated in this routine.  The function */
/*   subprograms invoke CALCK0 with the statement */
/*          CALL CALCK0(ARG,RESULT,JINT) */
/*   where the parameter usage is as follows */

/*      Function                     Parameters for CALCK0 */
/*       Call              ARG                  RESULT          JINT */

/*     BESK0(ARG)   0 .LT. ARG .LE. XMAX       K0(ARG)           1 */
/*     BESEK0(ARG)     0 .LT. ARG           EXP(ARG)*K0(ARG)     2 */

/*   The main computation evaluates slightly modified forms of near */
/*   minimax rational approximations generated by Russon and Blair, */
/*   Chalk River (Atomic Energy of Canada Limited) Report AECL-3461, */
/*   1969.  This transportable program is patterned after the */
/*   machine-dependent FUNPACK packet NATSK0, but cannot match that */
/*   version for efficiency or accuracy.  This version uses rational */
/*   functions that theoretically approximate K-SUB-0(X) to at */
/*   least 18 significant decimal digits.  The accuracy achieved */
/*   depends on the arithmetic system, the compiler, the intrinsic */
/*   functions, and proper selection of the machine-dependent */
/*   constants. */

/* ******************************************************************* */
/* ******************************************************************* */

/* Explanation of machine-dependent constants */

/*   beta   = Radix for the floating-point system */
/*   minexp = Smallest representable power of beta */
/*   maxexp = Smallest power of beta that overflows */
/*   XSMALL = Argument below which BESK0 and BESEK0 may */
/*            each be represented by a constant and a log. */
/*            largest X such that  1.0 + X = 1.0  to machine */
/*            precision. */
/*   XINF   = Largest positive machine number; approximately */
/*            beta**maxexp */
/*   XMAX   = Largest argument acceptable to BESK0;  Solution to */
/*            equation: */
/*               W(X) * (1-1/8X+9/128X**2) = beta**minexp */
/*            where  W(X) = EXP(-X)*SQRT(PI/2X) */


/*     Approximate values for some important machines are: */


/*                           beta       minexp       maxexp */

/*  CRAY-1        (S.P.)       2        -8193         8191 */
/*  Cyber 180/185 */
/*    under NOS   (S.P.)       2         -975         1070 */
/*  IEEE (IBM/XT, */
/*    SUN, etc.)  (S.P.)       2         -126          128 */
/*  IEEE (IBM/XT, */
/*    SUN, etc.)  (D.P.)       2        -1022         1024 */
/*  IBM 3033      (D.P.)      16          -65           63 */
/*  VAX D-Format  (D.P.)       2         -128          127 */
/*  VAX G-Format  (D.P.)       2        -1024         1023 */


/*                          XSMALL       XINF         XMAX */

/* CRAY-1        (S.P.)    3.55E-15   5.45E+2465    5674.858 */
/* Cyber 180/855 */
/*   under NOS   (S.P.)    1.77E-15   1.26E+322      672.788 */
/* IEEE (IBM/XT, */
/*   SUN, etc.)  (S.P.)    5.95E-8    3.40E+38        85.337 */
/* IEEE (IBM/XT, */
/*   SUN, etc.)  (D.P.)    1.11D-16   1.79D+308      705.342 */
/* IBM 3033      (D.P.)    1.11D-16   7.23D+75       177.852 */
/* VAX D-Format  (D.P.)    6.95D-18   1.70D+38        86.715 */
/* VAX G-Format  (D.P.)    5.55D-17   8.98D+307      706.728 */

/* ******************************************************************* */
/* ******************************************************************* */

/* Error returns */

/*  The program returns the value XINF for ARG .LE. 0.0, and the */
/*  BESK0 entry returns the value 0.0 for ARG .GT. XMAX. */


/*  Intrinsic functions required are: */

/*     EXP, LOG, SQRT */

/*  Latest modification: March 19, 1990 */

/*  Authors: W. J. Cody and Laura Stoltz */
/*           Mathematics and Computer Science Division */
/*           Argonne National Laboratory */
/*           Argonne, IL 60439 */

/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/*  Mathematical constants */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/*  Machine-dependent constants */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */

/*     Coefficients for XSMALL .LE.  ARG  .LE. 1.0 */

/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */

/*     Coefficients for  1.0 .LT. ARG */

/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
    x = *arg;
    if (x > zero) {
	if (x <= one) {
/* -------------------------------------------------------------------- */
/*     0.0 .LT.  ARG  .LE. 1.0 */
/* -------------------------------------------------------------------- */
	    temp = log(x);
	    if (x < xsmall) {
/* -------------------------------------------------------------------- */
/*     Return for small ARG */
/* -------------------------------------------------------------------- */
		*result = p[5] / q[1] - temp;
	    } else {
		xx = x * x;
		sump = ((((p[0] * xx + p[1]) * xx + p[2]) * xx + p[3]) * xx + 
			p[4]) * xx + p[5];
		sumq = (xx + q[0]) * xx + q[1];
		sumf = ((f[0] * xx + f[1]) * xx + f[2]) * xx + f[3];
		sumg = ((xx + g[0]) * xx + g[1]) * xx + g[2];
		*result = sump / sumq - xx * sumf * temp / sumg - temp;
		if (*jint == 2) {
		    *result *= exp(x);
		}
	    }
	} else if (*jint == 1 && x > xmax) {
/* -------------------------------------------------------------------- */
/*     Error return for ARG .GT. XMAX */
/* -------------------------------------------------------------------- */
	    *result = zero;
	} else {
/* -------------------------------------------------------------------- */
/*     1.0 .LT. ARG */
/* -------------------------------------------------------------------- */
	    xx = one / x;
	    sump = pp[0];
	    for (i__ = 2; i__ <= 10; ++i__) {
		sump = sump * xx + pp[i__ - 1];
/* L120: */
	    }
	    sumq = xx;
	    for (i__ = 1; i__ <= 9; ++i__) {
		sumq = (sumq + qq[i__ - 1]) * xx;
/* L140: */
	    }
	    sumq += qq[9];
	    *result = sump / sumq / sqrt(x);
	    if (*jint == 1) {
		*result *= exp(-x);
	    }
	}
    } else {
/* -------------------------------------------------------------------- */
/*     Error return for ARG .LE. 0.0 */
/* -------------------------------------------------------------------- */
	*result = xinf;
    }
/* -------------------------------------------------------------------- */
/*     Update error counts, etc. */
/* -------------------------------------------------------------------- */
    return 0;
/* ---------- Last line of CALCK0 ---------- */
} /* calck0_ */

doublereal besk0_(real *x)
{
    /* System generated locals */
    real ret_val;

    /* Local variables */
    static integer jint;
    extern /* Subroutine */ int calck0_(real *, real *, integer *);
    static real result;

/* -------------------------------------------------------------------- */

/* This function program computes approximate values for the */
/*   modified Bessel function of the second kind of order zero */
/*   for arguments 0.0 .LT. ARG .LE. XMAX (see comments heading */
/*   CALCK0). */

/*  Authors: W. J. Cody and Laura Stoltz */

/*  Latest Modification: January 19, 1988 */

/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
    jint = 1;
    calck0_(x, &result, &jint);
    ret_val = result;
    return ret_val;
/* ---------- Last line of BESK0 ---------- */
} /* besk0_ */

doublereal besek0_(real *x)
{
    /* System generated locals */
    real ret_val;

    /* Local variables */
    static integer jint;
    extern /* Subroutine */ int calck0_(real *, real *, integer *);
    static real result;

/* -------------------------------------------------------------------- */

/* This function program computes approximate values for the */
/*   modified Bessel function of the second kind of order zero */
/*   multiplied by the Exponential function, for arguments */
/*   0.0 .LT. ARG. */

/*  Authors: W. J. Cody and Laura Stoltz */

/*  Latest Modification: January 19, 1988 */

/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
    jint = 2;
    calck0_(x, &result, &jint);
    ret_val = result;
    return ret_val;
/* ---------- Last line of BESEK0 ---------- */
} /* besek0_ */

