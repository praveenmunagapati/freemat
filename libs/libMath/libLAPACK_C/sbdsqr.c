/* ../../../dependencies/lapack/src/sbdsqr.f -- translated by f2c (version 20061008).
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

/* Table of constant values */

static doublereal c_b15 = -.125;
static integer c__1 = 1;
static real c_b49 = 1.f;
static real c_b72 = -1.f;

/* Subroutine */ int sbdsqr_(char *uplo, integer *n, integer *ncvt, integer *
	nru, integer *ncc, real *d__, real *e, real *vt, integer *ldvt, real *
	u, integer *ldu, real *c__, integer *ldc, real *work, integer *info, 
	ftnlen uplo_len)
{
    /* System generated locals */
    integer c_dim1, c_offset, u_dim1, u_offset, vt_dim1, vt_offset, i__1, 
	    i__2;
    real r__1, r__2, r__3, r__4;
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *), sqrt(doublereal), r_sign(real *
	    , real *);

    /* Local variables */
    static real f, g, h__;
    static integer i__, j, m;
    static real r__, cs;
    static integer ll;
    static real sn, mu;
    static integer nm1, nm12, nm13, lll;
    static real eps, sll, tol, abse;
    static integer idir;
    static real abss;
    static integer oldm;
    static real cosl;
    static integer isub, iter;
    static real unfl, sinl, cosr, smin, smax, sinr;
    extern /* Subroutine */ int srot_(integer *, real *, integer *, real *, 
	    integer *, real *, real *), slas2_(real *, real *, real *, real *,
	     real *);
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static real oldcs;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    static integer oldll;
    static real shift, sigmn, oldsn;
    static integer maxit;
    static real sminl;
    extern /* Subroutine */ int slasr_(char *, char *, char *, integer *, 
	    integer *, real *, real *, real *, integer *, ftnlen, ftnlen, 
	    ftnlen);
    static real sigmx;
    static logical lower;
    extern /* Subroutine */ int sswap_(integer *, real *, integer *, real *, 
	    integer *), slasq1_(integer *, real *, real *, real *, integer *),
	     slasv2_(real *, real *, real *, real *, real *, real *, real *, 
	    real *, real *);
    extern doublereal slamch_(char *, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static real sminoa;
    extern /* Subroutine */ int slartg_(real *, real *, real *, real *, real *
	    );
    static real thresh;
    static logical rotate;
    static real sminlo, tolmul;


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     October 31, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SBDSQR computes the singular value decomposition (SVD) of a real */
/*  N-by-N (upper or lower) bidiagonal matrix B:  B = Q * S * P' (P' */
/*  denotes the transpose of P), where S is a diagonal matrix with */
/*  non-negative diagonal elements (the singular values of B), and Q */
/*  and P are orthogonal matrices. */

/*  The routine computes S, and optionally computes U * Q, P' * VT, */
/*  or Q' * C, for given real input matrices U, VT, and C. */

/*  See "Computing  Small Singular Values of Bidiagonal Matrices With */
/*  Guaranteed High Relative Accuracy," by J. Demmel and W. Kahan, */
/*  LAPACK Working Note #3 (or SIAM J. Sci. Statist. Comput. vol. 11, */
/*  no. 5, pp. 873-912, Sept 1990) and */
/*  "Accurate singular values and differential qd algorithms," by */
/*  B. Parlett and V. Fernando, Technical Report CPAM-554, Mathematics */
/*  Department, University of California at Berkeley, July 1992 */
/*  for a detailed description of the algorithm. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  B is upper bidiagonal; */
/*          = 'L':  B is lower bidiagonal. */

/*  N       (input) INTEGER */
/*          The order of the matrix B.  N >= 0. */

/*  NCVT    (input) INTEGER */
/*          The number of columns of the matrix VT. NCVT >= 0. */

/*  NRU     (input) INTEGER */
/*          The number of rows of the matrix U. NRU >= 0. */

/*  NCC     (input) INTEGER */
/*          The number of columns of the matrix C. NCC >= 0. */

/*  D       (input/output) REAL array, dimension (N) */
/*          On entry, the n diagonal elements of the bidiagonal matrix B. */
/*          On exit, if INFO=0, the singular values of B in decreasing */
/*          order. */

/*  E       (input/output) REAL array, dimension (N) */
/*          On entry, the elements of E contain the */
/*          offdiagonal elements of the bidiagonal matrix whose SVD */
/*          is desired. On normal exit (INFO = 0), E is destroyed. */
/*          If the algorithm does not converge (INFO > 0), D and E */
/*          will contain the diagonal and superdiagonal elements of a */
/*          bidiagonal matrix orthogonally equivalent to the one given */
/*          as input. E(N) is used for workspace. */

/*  VT      (input/output) REAL array, dimension (LDVT, NCVT) */
/*          On entry, an N-by-NCVT matrix VT. */
/*          On exit, VT is overwritten by P' * VT. */
/*          VT is not referenced if NCVT = 0. */

/*  LDVT    (input) INTEGER */
/*          The leading dimension of the array VT. */
/*          LDVT >= max(1,N) if NCVT > 0; LDVT >= 1 if NCVT = 0. */

/*  U       (input/output) REAL array, dimension (LDU, N) */
/*          On entry, an NRU-by-N matrix U. */
/*          On exit, U is overwritten by U * Q. */
/*          U is not referenced if NRU = 0. */

/*  LDU     (input) INTEGER */
/*          The leading dimension of the array U.  LDU >= max(1,NRU). */

/*  C       (input/output) REAL array, dimension (LDC, NCC) */
/*          On entry, an N-by-NCC matrix C. */
/*          On exit, C is overwritten by Q' * C. */
/*          C is not referenced if NCC = 0. */

/*  LDC     (input) INTEGER */
/*          The leading dimension of the array C. */
/*          LDC >= max(1,N) if NCC > 0; LDC >=1 if NCC = 0. */

/*  WORK    (workspace) REAL array, dimension (4*N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  If INFO = -i, the i-th argument had an illegal value */
/*          > 0:  the algorithm did not converge; D and E contain the */
/*                elements of a bidiagonal matrix which is orthogonally */
/*                similar to the input matrix B;  if INFO = i, i */
/*                elements of E have not converged to zero. */

/*  Internal Parameters */
/*  =================== */

/*  TOLMUL  REAL, default = max(10,min(100,EPS**(-1/8))) */
/*          TOLMUL controls the convergence criterion of the QR loop. */
/*          If it is positive, TOLMUL*EPS is the desired relative */
/*             precision in the computed singular values. */
/*          If it is negative, abs(TOLMUL*EPS*sigma_max) is the */
/*             desired absolute accuracy in the computed singular */
/*             values (corresponds to relative accuracy */
/*             abs(TOLMUL*EPS) in the largest singular value. */
/*          abs(TOLMUL) should be between 1 and 1/EPS, and preferably */
/*             between 10 (for fast convergence) and .1/EPS */
/*             (for there to be some accuracy in the results). */
/*          Default is to lose at either one eighth or 2 of the */
/*             available decimal digits in each computed singular value */
/*             (whichever is smaller). */

/*  MAXITR  INTEGER, default = 6 */
/*          MAXITR controls the maximum number of passes of the */
/*          algorithm through its inner loop. The algorithms stops */
/*          (and so fails to converge) if the number of passes */
/*          through the inner loop exceeds MAXITR*N**2. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --d__;
    --e;
    vt_dim1 = *ldvt;
    vt_offset = 1 + vt_dim1;
    vt -= vt_offset;
    u_dim1 = *ldu;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --work;

    /* Function Body */
    *info = 0;
    lower = lsame_(uplo, "L", (ftnlen)1, (ftnlen)1);
    if (! lsame_(uplo, "U", (ftnlen)1, (ftnlen)1) && ! lower) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*ncvt < 0) {
	*info = -3;
    } else if (*nru < 0) {
	*info = -4;
    } else if (*ncc < 0) {
	*info = -5;
    } else if (*ncvt == 0 && *ldvt < 1 || *ncvt > 0 && *ldvt < max(1,*n)) {
	*info = -9;
    } else if (*ldu < max(1,*nru)) {
	*info = -11;
    } else if (*ncc == 0 && *ldc < 1 || *ncc > 0 && *ldc < max(1,*n)) {
	*info = -13;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SBDSQR", &i__1, (ftnlen)6);
	return 0;
    }
    if (*n == 0) {
	return 0;
    }
    if (*n == 1) {
	goto L160;
    }

/*     ROTATE is true if any singular vectors desired, false otherwise */

    rotate = *ncvt > 0 || *nru > 0 || *ncc > 0;

/*     If no singular vectors desired, use qd algorithm */

    if (! rotate) {
	slasq1_(n, &d__[1], &e[1], &work[1], info);
	return 0;
    }

    nm1 = *n - 1;
    nm12 = nm1 + nm1;
    nm13 = nm12 + nm1;
    idir = 0;

/*     Get machine constants */

    eps = slamch_("Epsilon", (ftnlen)7);
    unfl = slamch_("Safe minimum", (ftnlen)12);

/*     If matrix lower bidiagonal, rotate to be upper bidiagonal */
/*     by applying Givens rotations on the left */

    if (lower) {
	i__1 = *n - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    slartg_(&d__[i__], &e[i__], &cs, &sn, &r__);
	    d__[i__] = r__;
	    e[i__] = sn * d__[i__ + 1];
	    d__[i__ + 1] = cs * d__[i__ + 1];
	    work[i__] = cs;
	    work[nm1 + i__] = sn;
/* L10: */
	}

/*        Update singular vectors if desired */

	if (*nru > 0) {
	    slasr_("R", "V", "F", nru, n, &work[1], &work[*n], &u[u_offset], 
		    ldu, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	}
	if (*ncc > 0) {
	    slasr_("L", "V", "F", n, ncc, &work[1], &work[*n], &c__[c_offset],
		     ldc, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	}
    }

/*     Compute singular values to relative accuracy TOL */
/*     (By setting TOL to be negative, algorithm will compute */
/*     singular values to absolute accuracy ABS(TOL)*norm(input matrix)) */

/* Computing MAX */
/* Computing MIN */
    d__1 = (doublereal) eps;
    r__3 = 100.f, r__4 = pow_dd(&d__1, &c_b15);
    r__1 = 10.f, r__2 = dmin(r__3,r__4);
    tolmul = dmax(r__1,r__2);
    tol = tolmul * eps;

/*     Compute approximate maximum, minimum singular values */

    smax = 0.f;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	r__2 = smax, r__3 = (r__1 = d__[i__], dabs(r__1));
	smax = dmax(r__2,r__3);
/* L20: */
    }
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	r__2 = smax, r__3 = (r__1 = e[i__], dabs(r__1));
	smax = dmax(r__2,r__3);
/* L30: */
    }
    sminl = 0.f;
    if (tol >= 0.f) {

/*        Relative accuracy desired */

	sminoa = dabs(d__[1]);
	if (sminoa == 0.f) {
	    goto L50;
	}
	mu = sminoa;
	i__1 = *n;
	for (i__ = 2; i__ <= i__1; ++i__) {
	    mu = (r__2 = d__[i__], dabs(r__2)) * (mu / (mu + (r__1 = e[i__ - 
		    1], dabs(r__1))));
	    sminoa = dmin(sminoa,mu);
	    if (sminoa == 0.f) {
		goto L50;
	    }
/* L40: */
	}
L50:
	sminoa /= sqrt((real) (*n));
/* Computing MAX */
	r__1 = tol * sminoa, r__2 = *n * 6 * *n * unfl;
	thresh = dmax(r__1,r__2);
    } else {

/*        Absolute accuracy desired */

/* Computing MAX */
	r__1 = dabs(tol) * smax, r__2 = *n * 6 * *n * unfl;
	thresh = dmax(r__1,r__2);
    }

/*     Prepare for main iteration loop for the singular values */
/*     (MAXIT is the maximum number of passes through the inner */
/*     loop permitted before nonconvergence signalled.) */

    maxit = *n * 6 * *n;
    iter = 0;
    oldll = -1;
    oldm = -1;

/*     M points to last element of unconverged part of matrix */

    m = *n;

/*     Begin main iteration loop */

L60:

/*     Check for convergence or exceeding iteration count */

    if (m <= 1) {
	goto L160;
    }
    if (iter > maxit) {
	goto L200;
    }

/*     Find diagonal block of matrix to work on */

    if (tol < 0.f && (r__1 = d__[m], dabs(r__1)) <= thresh) {
	d__[m] = 0.f;
    }
    smax = (r__1 = d__[m], dabs(r__1));
    smin = smax;
    i__1 = m - 1;
    for (lll = 1; lll <= i__1; ++lll) {
	ll = m - lll;
	abss = (r__1 = d__[ll], dabs(r__1));
	abse = (r__1 = e[ll], dabs(r__1));
	if (tol < 0.f && abss <= thresh) {
	    d__[ll] = 0.f;
	}
	if (abse <= thresh) {
	    goto L80;
	}
	smin = dmin(smin,abss);
/* Computing MAX */
	r__1 = max(smax,abss);
	smax = dmax(r__1,abse);
/* L70: */
    }
    ll = 0;
    goto L90;
L80:
    e[ll] = 0.f;

/*     Matrix splits since E(LL) = 0 */

    if (ll == m - 1) {

/*        Convergence of bottom singular value, return to top of loop */

	--m;
	goto L60;
    }
L90:
    ++ll;

/*     E(LL) through E(M-1) are nonzero, E(LL-1) is zero */

    if (ll == m - 1) {

/*        2 by 2 block, handle separately */

	slasv2_(&d__[m - 1], &e[m - 1], &d__[m], &sigmn, &sigmx, &sinr, &cosr,
		 &sinl, &cosl);
	d__[m - 1] = sigmx;
	e[m - 1] = 0.f;
	d__[m] = sigmn;

/*        Compute singular vectors, if desired */

	if (*ncvt > 0) {
	    srot_(ncvt, &vt[m - 1 + vt_dim1], ldvt, &vt[m + vt_dim1], ldvt, &
		    cosr, &sinr);
	}
	if (*nru > 0) {
	    srot_(nru, &u[(m - 1) * u_dim1 + 1], &c__1, &u[m * u_dim1 + 1], &
		    c__1, &cosl, &sinl);
	}
	if (*ncc > 0) {
	    srot_(ncc, &c__[m - 1 + c_dim1], ldc, &c__[m + c_dim1], ldc, &
		    cosl, &sinl);
	}
	m += -2;
	goto L60;
    }

/*     If working on new submatrix, choose shift direction */
/*     (from larger end diagonal element towards smaller) */

    if (ll > oldm || m < oldll) {
	if ((r__1 = d__[ll], dabs(r__1)) >= (r__2 = d__[m], dabs(r__2))) {

/*           Chase bulge from top (big end) to bottom (small end) */

	    idir = 1;
	} else {

/*           Chase bulge from bottom (big end) to top (small end) */

	    idir = 2;
	}
    }

/*     Apply convergence tests */

    if (idir == 1) {

/*        Run convergence test in forward direction */
/*        First apply standard test to bottom of matrix */

	if ((r__2 = e[m - 1], dabs(r__2)) <= dabs(tol) * (r__1 = d__[m], dabs(
		r__1)) || tol < 0.f && (r__3 = e[m - 1], dabs(r__3)) <= 
		thresh) {
	    e[m - 1] = 0.f;
	    goto L60;
	}

	if (tol >= 0.f) {

/*           If relative accuracy desired, */
/*           apply convergence criterion forward */

	    mu = (r__1 = d__[ll], dabs(r__1));
	    sminl = mu;
	    i__1 = m - 1;
	    for (lll = ll; lll <= i__1; ++lll) {
		if ((r__1 = e[lll], dabs(r__1)) <= tol * mu) {
		    e[lll] = 0.f;
		    goto L60;
		}
		sminlo = sminl;
		mu = (r__2 = d__[lll + 1], dabs(r__2)) * (mu / (mu + (r__1 = 
			e[lll], dabs(r__1))));
		sminl = dmin(sminl,mu);
/* L100: */
	    }
	}

    } else {

/*        Run convergence test in backward direction */
/*        First apply standard test to top of matrix */

	if ((r__2 = e[ll], dabs(r__2)) <= dabs(tol) * (r__1 = d__[ll], dabs(
		r__1)) || tol < 0.f && (r__3 = e[ll], dabs(r__3)) <= thresh) {
	    e[ll] = 0.f;
	    goto L60;
	}

	if (tol >= 0.f) {

/*           If relative accuracy desired, */
/*           apply convergence criterion backward */

	    mu = (r__1 = d__[m], dabs(r__1));
	    sminl = mu;
	    i__1 = ll;
	    for (lll = m - 1; lll >= i__1; --lll) {
		if ((r__1 = e[lll], dabs(r__1)) <= tol * mu) {
		    e[lll] = 0.f;
		    goto L60;
		}
		sminlo = sminl;
		mu = (r__2 = d__[lll], dabs(r__2)) * (mu / (mu + (r__1 = e[
			lll], dabs(r__1))));
		sminl = dmin(sminl,mu);
/* L110: */
	    }
	}
    }
    oldll = ll;
    oldm = m;

/*     Compute shift.  First, test if shifting would ruin relative */
/*     accuracy, and if so set the shift to zero. */

/* Computing MAX */
    r__1 = eps, r__2 = tol * .01f;
    if (tol >= 0.f && *n * tol * (sminl / smax) <= dmax(r__1,r__2)) {

/*        Use a zero shift to avoid loss of relative accuracy */

	shift = 0.f;
    } else {

/*        Compute the shift from 2-by-2 block at end of matrix */

	if (idir == 1) {
	    sll = (r__1 = d__[ll], dabs(r__1));
	    slas2_(&d__[m - 1], &e[m - 1], &d__[m], &shift, &r__);
	} else {
	    sll = (r__1 = d__[m], dabs(r__1));
	    slas2_(&d__[ll], &e[ll], &d__[ll + 1], &shift, &r__);
	}

/*        Test if shift negligible, and if so set to zero */

	if (sll > 0.f) {
/* Computing 2nd power */
	    r__1 = shift / sll;
	    if (r__1 * r__1 < eps) {
		shift = 0.f;
	    }
	}
    }

/*     Increment iteration count */

    iter = iter + m - ll;

/*     If SHIFT = 0, do simplified QR iteration */

    if (shift == 0.f) {
	if (idir == 1) {

/*           Chase bulge from top to bottom */
/*           Save cosines and sines for later singular vector updates */

	    cs = 1.f;
	    oldcs = 1.f;
	    i__1 = m - 1;
	    for (i__ = ll; i__ <= i__1; ++i__) {
		r__1 = d__[i__] * cs;
		slartg_(&r__1, &e[i__], &cs, &sn, &r__);
		if (i__ > ll) {
		    e[i__ - 1] = oldsn * r__;
		}
		r__1 = oldcs * r__;
		r__2 = d__[i__ + 1] * sn;
		slartg_(&r__1, &r__2, &oldcs, &oldsn, &d__[i__]);
		work[i__ - ll + 1] = cs;
		work[i__ - ll + 1 + nm1] = sn;
		work[i__ - ll + 1 + nm12] = oldcs;
		work[i__ - ll + 1 + nm13] = oldsn;
/* L120: */
	    }
	    h__ = d__[m] * cs;
	    d__[m] = h__ * oldcs;
	    e[m - 1] = h__ * oldsn;

/*           Update singular vectors */

	    if (*ncvt > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "F", &i__1, ncvt, &work[1], &work[*n], &vt[
			ll + vt_dim1], ldvt, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	    if (*nru > 0) {
		i__1 = m - ll + 1;
		slasr_("R", "V", "F", nru, &i__1, &work[nm12 + 1], &work[nm13 
			+ 1], &u[ll * u_dim1 + 1], ldu, (ftnlen)1, (ftnlen)1, 
			(ftnlen)1);
	    }
	    if (*ncc > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "F", &i__1, ncc, &work[nm12 + 1], &work[nm13 
			+ 1], &c__[ll + c_dim1], ldc, (ftnlen)1, (ftnlen)1, (
			ftnlen)1);
	    }

/*           Test convergence */

	    if ((r__1 = e[m - 1], dabs(r__1)) <= thresh) {
		e[m - 1] = 0.f;
	    }

	} else {

/*           Chase bulge from bottom to top */
/*           Save cosines and sines for later singular vector updates */

	    cs = 1.f;
	    oldcs = 1.f;
	    i__1 = ll + 1;
	    for (i__ = m; i__ >= i__1; --i__) {
		r__1 = d__[i__] * cs;
		slartg_(&r__1, &e[i__ - 1], &cs, &sn, &r__);
		if (i__ < m) {
		    e[i__] = oldsn * r__;
		}
		r__1 = oldcs * r__;
		r__2 = d__[i__ - 1] * sn;
		slartg_(&r__1, &r__2, &oldcs, &oldsn, &d__[i__]);
		work[i__ - ll] = cs;
		work[i__ - ll + nm1] = -sn;
		work[i__ - ll + nm12] = oldcs;
		work[i__ - ll + nm13] = -oldsn;
/* L130: */
	    }
	    h__ = d__[ll] * cs;
	    d__[ll] = h__ * oldcs;
	    e[ll] = h__ * oldsn;

/*           Update singular vectors */

	    if (*ncvt > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "B", &i__1, ncvt, &work[nm12 + 1], &work[
			nm13 + 1], &vt[ll + vt_dim1], ldvt, (ftnlen)1, (
			ftnlen)1, (ftnlen)1);
	    }
	    if (*nru > 0) {
		i__1 = m - ll + 1;
		slasr_("R", "V", "B", nru, &i__1, &work[1], &work[*n], &u[ll *
			 u_dim1 + 1], ldu, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	    if (*ncc > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "B", &i__1, ncc, &work[1], &work[*n], &c__[
			ll + c_dim1], ldc, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }

/*           Test convergence */

	    if ((r__1 = e[ll], dabs(r__1)) <= thresh) {
		e[ll] = 0.f;
	    }
	}
    } else {

/*        Use nonzero shift */

	if (idir == 1) {

/*           Chase bulge from top to bottom */
/*           Save cosines and sines for later singular vector updates */

	    f = ((r__1 = d__[ll], dabs(r__1)) - shift) * (r_sign(&c_b49, &d__[
		    ll]) + shift / d__[ll]);
	    g = e[ll];
	    i__1 = m - 1;
	    for (i__ = ll; i__ <= i__1; ++i__) {
		slartg_(&f, &g, &cosr, &sinr, &r__);
		if (i__ > ll) {
		    e[i__ - 1] = r__;
		}
		f = cosr * d__[i__] + sinr * e[i__];
		e[i__] = cosr * e[i__] - sinr * d__[i__];
		g = sinr * d__[i__ + 1];
		d__[i__ + 1] = cosr * d__[i__ + 1];
		slartg_(&f, &g, &cosl, &sinl, &r__);
		d__[i__] = r__;
		f = cosl * e[i__] + sinl * d__[i__ + 1];
		d__[i__ + 1] = cosl * d__[i__ + 1] - sinl * e[i__];
		if (i__ < m - 1) {
		    g = sinl * e[i__ + 1];
		    e[i__ + 1] = cosl * e[i__ + 1];
		}
		work[i__ - ll + 1] = cosr;
		work[i__ - ll + 1 + nm1] = sinr;
		work[i__ - ll + 1 + nm12] = cosl;
		work[i__ - ll + 1 + nm13] = sinl;
/* L140: */
	    }
	    e[m - 1] = f;

/*           Update singular vectors */

	    if (*ncvt > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "F", &i__1, ncvt, &work[1], &work[*n], &vt[
			ll + vt_dim1], ldvt, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	    if (*nru > 0) {
		i__1 = m - ll + 1;
		slasr_("R", "V", "F", nru, &i__1, &work[nm12 + 1], &work[nm13 
			+ 1], &u[ll * u_dim1 + 1], ldu, (ftnlen)1, (ftnlen)1, 
			(ftnlen)1);
	    }
	    if (*ncc > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "F", &i__1, ncc, &work[nm12 + 1], &work[nm13 
			+ 1], &c__[ll + c_dim1], ldc, (ftnlen)1, (ftnlen)1, (
			ftnlen)1);
	    }

/*           Test convergence */

	    if ((r__1 = e[m - 1], dabs(r__1)) <= thresh) {
		e[m - 1] = 0.f;
	    }

	} else {

/*           Chase bulge from bottom to top */
/*           Save cosines and sines for later singular vector updates */

	    f = ((r__1 = d__[m], dabs(r__1)) - shift) * (r_sign(&c_b49, &d__[
		    m]) + shift / d__[m]);
	    g = e[m - 1];
	    i__1 = ll + 1;
	    for (i__ = m; i__ >= i__1; --i__) {
		slartg_(&f, &g, &cosr, &sinr, &r__);
		if (i__ < m) {
		    e[i__] = r__;
		}
		f = cosr * d__[i__] + sinr * e[i__ - 1];
		e[i__ - 1] = cosr * e[i__ - 1] - sinr * d__[i__];
		g = sinr * d__[i__ - 1];
		d__[i__ - 1] = cosr * d__[i__ - 1];
		slartg_(&f, &g, &cosl, &sinl, &r__);
		d__[i__] = r__;
		f = cosl * e[i__ - 1] + sinl * d__[i__ - 1];
		d__[i__ - 1] = cosl * d__[i__ - 1] - sinl * e[i__ - 1];
		if (i__ > ll + 1) {
		    g = sinl * e[i__ - 2];
		    e[i__ - 2] = cosl * e[i__ - 2];
		}
		work[i__ - ll] = cosr;
		work[i__ - ll + nm1] = -sinr;
		work[i__ - ll + nm12] = cosl;
		work[i__ - ll + nm13] = -sinl;
/* L150: */
	    }
	    e[ll] = f;

/*           Test convergence */

	    if ((r__1 = e[ll], dabs(r__1)) <= thresh) {
		e[ll] = 0.f;
	    }

/*           Update singular vectors if desired */

	    if (*ncvt > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "B", &i__1, ncvt, &work[nm12 + 1], &work[
			nm13 + 1], &vt[ll + vt_dim1], ldvt, (ftnlen)1, (
			ftnlen)1, (ftnlen)1);
	    }
	    if (*nru > 0) {
		i__1 = m - ll + 1;
		slasr_("R", "V", "B", nru, &i__1, &work[1], &work[*n], &u[ll *
			 u_dim1 + 1], ldu, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	    if (*ncc > 0) {
		i__1 = m - ll + 1;
		slasr_("L", "V", "B", &i__1, ncc, &work[1], &work[*n], &c__[
			ll + c_dim1], ldc, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	}
    }

/*     QR iteration finished, go back and check convergence */

    goto L60;

/*     All singular values converged, so make them positive */

L160:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (d__[i__] < 0.f) {
	    d__[i__] = -d__[i__];

/*           Change sign of singular vectors, if desired */

	    if (*ncvt > 0) {
		sscal_(ncvt, &c_b72, &vt[i__ + vt_dim1], ldvt);
	    }
	}
/* L170: */
    }

/*     Sort the singular values into decreasing order (insertion sort on */
/*     singular values, but only one transposition per singular vector) */

    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {

/*        Scan for smallest D(I) */

	isub = 1;
	smin = d__[1];
	i__2 = *n + 1 - i__;
	for (j = 2; j <= i__2; ++j) {
	    if (d__[j] <= smin) {
		isub = j;
		smin = d__[j];
	    }
/* L180: */
	}
	if (isub != *n + 1 - i__) {

/*           Swap singular values and vectors */

	    d__[isub] = d__[*n + 1 - i__];
	    d__[*n + 1 - i__] = smin;
	    if (*ncvt > 0) {
		sswap_(ncvt, &vt[isub + vt_dim1], ldvt, &vt[*n + 1 - i__ + 
			vt_dim1], ldvt);
	    }
	    if (*nru > 0) {
		sswap_(nru, &u[isub * u_dim1 + 1], &c__1, &u[(*n + 1 - i__) * 
			u_dim1 + 1], &c__1);
	    }
	    if (*ncc > 0) {
		sswap_(ncc, &c__[isub + c_dim1], ldc, &c__[*n + 1 - i__ + 
			c_dim1], ldc);
	    }
	}
/* L190: */
    }
    goto L220;

/*     Maximum number of iterations exceeded, failure to converge */

L200:
    *info = 0;
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (e[i__] != 0.f) {
	    ++(*info);
	}
/* L210: */
    }
L220:
    return 0;

/*     End of SBDSQR */

} /* sbdsqr_ */
