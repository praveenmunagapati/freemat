/* complex16/zhseqr.f -- translated by f2c (version 20030211).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static doublecomplex c_b1 = {0.,0.};
static doublecomplex c_b2 = {1.,0.};
static integer c__1 = 1;
static integer c__4 = 4;
static integer c_n1 = -1;
static integer c__2 = 2;
static integer c__8 = 8;
static integer c__15 = 15;
static logical c_false = FALSE_;

/* Subroutine */ int zhseqr_(char *job, char *compz, integer *n, integer *ilo,
	 integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *w, 
	doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork,
	 integer *info, ftnlen job_len, ftnlen compz_len)
{
    /* System generated locals */
    address a__1[2];
    integer h_dim1, h_offset, z_dim1, z_offset, i__1, i__2, i__3, i__4[2], 
	    i__5, i__6;
    doublereal d__1, d__2, d__3, d__4;
    doublecomplex z__1;
    char ch__1[2];

    /* Builtin functions */
    double d_imag(doublecomplex *);
    void d_cnjg(doublecomplex *, doublecomplex *);
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer i__, j, k, l;
    static doublecomplex s[225]	/* was [15][15] */, v[16];
    static integer i1, i2, ii, nh, nr, ns, nv;
    static doublecomplex vv[16];
    static integer itn;
    static doublecomplex tau;
    static integer its;
    static doublereal ulp, tst1;
    static integer maxb, ierr;
    static doublereal unfl;
    static doublecomplex temp;
    static doublereal ovfl;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    extern /* Subroutine */ int zscal_(integer *, doublecomplex *, 
	    doublecomplex *, integer *);
    static integer itemp;
    static doublereal rtemp;
    extern /* Subroutine */ int zgemv_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, ftnlen);
    static logical initz, wantt, wantz;
    static doublereal rwork[1];
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *);
    extern doublereal dlapy2_(doublereal *, doublereal *);
    extern /* Subroutine */ int dlabad_(doublereal *, doublereal *);
    extern doublereal dlamch_(char *, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    extern /* Subroutine */ int zdscal_(integer *, doublereal *, 
	    doublecomplex *, integer *), zlarfg_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *);
    extern integer izamax_(integer *, doublecomplex *, integer *);
    extern doublereal zlanhs_(char *, integer *, doublecomplex *, integer *, 
	    doublereal *, ftnlen);
    extern /* Subroutine */ int zlahqr_(logical *, logical *, integer *, 
	    integer *, integer *, doublecomplex *, integer *, doublecomplex *,
	     integer *, integer *, doublecomplex *, integer *, integer *), 
	    zlacpy_(char *, integer *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, ftnlen), zlaset_(char *, integer *, 
	    integer *, doublecomplex *, doublecomplex *, doublecomplex *, 
	    integer *, ftnlen), zlarfx_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *, ftnlen);
    static doublereal smlnum;
    static logical lquery;


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZHSEQR computes the eigenvalues of a complex upper Hessenberg */
/*  matrix H, and, optionally, the matrices T and Z from the Schur */
/*  decomposition H = Z T Z**H, where T is an upper triangular matrix */
/*  (the Schur form), and Z is the unitary matrix of Schur vectors. */

/*  Optionally Z may be postmultiplied into an input unitary matrix Q, */
/*  so that this routine can give the Schur factorization of a matrix A */
/*  which has been reduced to the Hessenberg form H by the unitary */
/*  matrix Q:  A = Q*H*Q**H = (QZ)*T*(QZ)**H. */

/*  Arguments */
/*  ========= */

/*  JOB     (input) CHARACTER*1 */
/*          = 'E': compute eigenvalues only; */
/*          = 'S': compute eigenvalues and the Schur form T. */

/*  COMPZ   (input) CHARACTER*1 */
/*          = 'N': no Schur vectors are computed; */
/*          = 'I': Z is initialized to the unit matrix and the matrix Z */
/*                 of Schur vectors of H is returned; */
/*          = 'V': Z must contain an unitary matrix Q on entry, and */
/*                 the product Q*Z is returned. */

/*  N       (input) INTEGER */
/*          The order of the matrix H.  N >= 0. */

/*  ILO     (input) INTEGER */
/*  IHI     (input) INTEGER */
/*          It is assumed that H is already upper triangular in rows */
/*          and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally */
/*          set by a previous call to ZGEBAL, and then passed to CGEHRD */
/*          when the matrix output by ZGEBAL is reduced to Hessenberg */
/*          form. Otherwise ILO and IHI should be set to 1 and N */
/*          respectively. */
/*          1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0. */

/*  H       (input/output) COMPLEX*16 array, dimension (LDH,N) */
/*          On entry, the upper Hessenberg matrix H. */
/*          On exit, if JOB = 'S', H contains the upper triangular matrix */
/*          T from the Schur decomposition (the Schur form). If */
/*          JOB = 'E', the contents of H are unspecified on exit. */

/*  LDH     (input) INTEGER */
/*          The leading dimension of the array H. LDH >= max(1,N). */

/*  W       (output) COMPLEX*16 array, dimension (N) */
/*          The computed eigenvalues. If JOB = 'S', the eigenvalues are */
/*          stored in the same order as on the diagonal of the Schur form */
/*          returned in H, with W(i) = H(i,i). */

/*  Z       (input/output) COMPLEX*16 array, dimension (LDZ,N) */
/*          If COMPZ = 'N': Z is not referenced. */
/*          If COMPZ = 'I': on entry, Z need not be set, and on exit, Z */
/*          contains the unitary matrix Z of the Schur vectors of H. */
/*          If COMPZ = 'V': on entry Z must contain an N-by-N matrix Q, */
/*          which is assumed to be equal to the unit matrix except for */
/*          the submatrix Z(ILO:IHI,ILO:IHI); on exit Z contains Q*Z. */
/*          Normally Q is the unitary matrix generated by ZUNGHR after */
/*          the call to ZGEHRD which formed the Hessenberg matrix H. */

/*  LDZ     (input) INTEGER */
/*          The leading dimension of the array Z. */
/*          LDZ >= max(1,N) if COMPZ = 'I' or 'V'; LDZ >= 1 otherwise. */

/*  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK.  LWORK >= max(1,N). */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, ZHSEQR failed to compute all the */
/*                eigenvalues in a total of 30*(IHI-ILO+1) iterations; */
/*                elements 1:ilo-1 and i+1:n of W contain those */
/*                eigenvalues which have been successfully computed. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Decode and test the input parameters */

    /* Parameter adjustments */
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;

    /* Function Body */
    wantt = lsame_(job, "S", (ftnlen)1, (ftnlen)1);
    initz = lsame_(compz, "I", (ftnlen)1, (ftnlen)1);
    wantz = initz || lsame_(compz, "V", (ftnlen)1, (ftnlen)1);

    *info = 0;
    i__1 = max(1,*n);
    work[1].r = (doublereal) i__1, work[1].i = 0.;
    lquery = *lwork == -1;
    if (! lsame_(job, "E", (ftnlen)1, (ftnlen)1) && ! wantt) {
	*info = -1;
    } else if (! lsame_(compz, "N", (ftnlen)1, (ftnlen)1) && ! wantz) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1 || *ilo > max(1,*n)) {
	*info = -4;
    } else if (*ihi < min(*ilo,*n) || *ihi > *n) {
	*info = -5;
    } else if (*ldh < max(1,*n)) {
	*info = -7;
    } else if (*ldz < 1 || wantz && *ldz < max(1,*n)) {
	*info = -10;
    } else if (*lwork < max(1,*n) && ! lquery) {
	*info = -12;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZHSEQR", &i__1, (ftnlen)6);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Initialize Z, if necessary */

    if (initz) {
	zlaset_("Full", n, n, &c_b1, &c_b2, &z__[z_offset], ldz, (ftnlen)4);
    }

/*     Store the eigenvalues isolated by ZGEBAL. */

    i__1 = *ilo - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	i__3 = i__ + i__ * h_dim1;
	w[i__2].r = h__[i__3].r, w[i__2].i = h__[i__3].i;
/* L10: */
    }
    i__1 = *n;
    for (i__ = *ihi + 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	i__3 = i__ + i__ * h_dim1;
	w[i__2].r = h__[i__3].r, w[i__2].i = h__[i__3].i;
/* L20: */
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }
    if (*ilo == *ihi) {
	i__1 = *ilo;
	i__2 = *ilo + *ilo * h_dim1;
	w[i__1].r = h__[i__2].r, w[i__1].i = h__[i__2].i;
	return 0;
    }

/*     Set rows and columns ILO to IHI to zero below the first */
/*     subdiagonal. */

    i__1 = *ihi - 2;
    for (j = *ilo; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = j + 2; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * h_dim1;
	    h__[i__3].r = 0., h__[i__3].i = 0.;
/* L30: */
	}
/* L40: */
    }
    nh = *ihi - *ilo + 1;

/*     I1 and I2 are the indices of the first row and last column of H */
/*     to which transformations must be applied. If eigenvalues only are */
/*     being computed, I1 and I2 are re-set inside the main loop. */

    if (wantt) {
	i1 = 1;
	i2 = *n;
    } else {
	i1 = *ilo;
	i2 = *ihi;
    }

/*     Ensure that the subdiagonal elements are real. */

    i__1 = *ihi;
    for (i__ = *ilo + 1; i__ <= i__1; ++i__) {
	i__2 = i__ + (i__ - 1) * h_dim1;
	temp.r = h__[i__2].r, temp.i = h__[i__2].i;
	if (d_imag(&temp) != 0.) {
	    d__1 = temp.r;
	    d__2 = d_imag(&temp);
	    rtemp = dlapy2_(&d__1, &d__2);
	    i__2 = i__ + (i__ - 1) * h_dim1;
	    h__[i__2].r = rtemp, h__[i__2].i = 0.;
	    z__1.r = temp.r / rtemp, z__1.i = temp.i / rtemp;
	    temp.r = z__1.r, temp.i = z__1.i;
	    if (i2 > i__) {
		i__2 = i2 - i__;
		d_cnjg(&z__1, &temp);
		zscal_(&i__2, &z__1, &h__[i__ + (i__ + 1) * h_dim1], ldh);
	    }
	    i__2 = i__ - i1;
	    zscal_(&i__2, &temp, &h__[i1 + i__ * h_dim1], &c__1);
	    if (i__ < *ihi) {
		i__2 = i__ + 1 + i__ * h_dim1;
		i__3 = i__ + 1 + i__ * h_dim1;
		z__1.r = temp.r * h__[i__3].r - temp.i * h__[i__3].i, z__1.i =
			 temp.r * h__[i__3].i + temp.i * h__[i__3].r;
		h__[i__2].r = z__1.r, h__[i__2].i = z__1.i;
	    }
	    if (wantz) {
		zscal_(&nh, &temp, &z__[*ilo + i__ * z_dim1], &c__1);
	    }
	}
/* L50: */
    }

/*     Determine the order of the multi-shift QR algorithm to be used. */

/* Writing concatenation */
    i__4[0] = 1, a__1[0] = job;
    i__4[1] = 1, a__1[1] = compz;
    s_cat(ch__1, a__1, i__4, &c__2, (ftnlen)2);
    ns = ilaenv_(&c__4, "ZHSEQR", ch__1, n, ilo, ihi, &c_n1, (ftnlen)6, (
	    ftnlen)2);
/* Writing concatenation */
    i__4[0] = 1, a__1[0] = job;
    i__4[1] = 1, a__1[1] = compz;
    s_cat(ch__1, a__1, i__4, &c__2, (ftnlen)2);
    maxb = ilaenv_(&c__8, "ZHSEQR", ch__1, n, ilo, ihi, &c_n1, (ftnlen)6, (
	    ftnlen)2);
    if (ns <= 1 || ns > nh || maxb >= nh) {

/*        Use the standard double-shift algorithm */

	zlahqr_(&wantt, &wantz, n, ilo, ihi, &h__[h_offset], ldh, &w[1], ilo, 
		ihi, &z__[z_offset], ldz, info);
	return 0;
    }
    maxb = max(2,maxb);
/* Computing MIN */
    i__1 = min(ns,maxb);
    ns = min(i__1,15);

/*     Now 1 < NS <= MAXB < NH. */

/*     Set machine-dependent constants for the stopping criterion. */
/*     If norm(H) <= sqrt(OVFL), overflow should not occur. */

    unfl = dlamch_("Safe minimum", (ftnlen)12);
    ovfl = 1. / unfl;
    dlabad_(&unfl, &ovfl);
    ulp = dlamch_("Precision", (ftnlen)9);
    smlnum = unfl * (nh / ulp);

/*     ITN is the total number of multiple-shift QR iterations allowed. */

    itn = nh * 30;

/*     The main loop begins here. I is the loop index and decreases from */
/*     IHI to ILO in steps of at most MAXB. Each iteration of the loop */
/*     works with the active submatrix in rows and columns L to I. */
/*     Eigenvalues I+1 to IHI have already converged. Either L = ILO, or */
/*     H(L,L-1) is negligible so that the matrix splits. */

    i__ = *ihi;
L60:
    if (i__ < *ilo) {
	goto L180;
    }

/*     Perform multiple-shift QR iterations on rows and columns ILO to I */
/*     until a submatrix of order at most MAXB splits off at the bottom */
/*     because a subdiagonal element has become negligible. */

    l = *ilo;
    i__1 = itn;
    for (its = 0; its <= i__1; ++its) {

/*        Look for a single small subdiagonal element. */

	i__2 = l + 1;
	for (k = i__; k >= i__2; --k) {
	    i__3 = k - 1 + (k - 1) * h_dim1;
	    i__5 = k + k * h_dim1;
	    tst1 = (d__1 = h__[i__3].r, abs(d__1)) + (d__2 = d_imag(&h__[k - 
		    1 + (k - 1) * h_dim1]), abs(d__2)) + ((d__3 = h__[i__5].r,
		     abs(d__3)) + (d__4 = d_imag(&h__[k + k * h_dim1]), abs(
		    d__4)));
	    if (tst1 == 0.) {
		i__3 = i__ - l + 1;
		tst1 = zlanhs_("1", &i__3, &h__[l + l * h_dim1], ldh, rwork, (
			ftnlen)1);
	    }
	    i__3 = k + (k - 1) * h_dim1;
/* Computing MAX */
	    d__2 = ulp * tst1;
	    if ((d__1 = h__[i__3].r, abs(d__1)) <= max(d__2,smlnum)) {
		goto L80;
	    }
/* L70: */
	}
L80:
	l = k;
	if (l > *ilo) {

/*           H(L,L-1) is negligible. */

	    i__2 = l + (l - 1) * h_dim1;
	    h__[i__2].r = 0., h__[i__2].i = 0.;
	}

/*        Exit from loop if a submatrix of order <= MAXB has split off. */

	if (l >= i__ - maxb + 1) {
	    goto L170;
	}

/*        Now the active submatrix is in rows and columns L to I. If */
/*        eigenvalues only are being computed, only the active submatrix */
/*        need be transformed. */

	if (! wantt) {
	    i1 = l;
	    i2 = i__;
	}

	if (its == 20 || its == 30) {

/*           Exceptional shifts. */

	    i__2 = i__;
	    for (ii = i__ - ns + 1; ii <= i__2; ++ii) {
		i__3 = ii;
		i__5 = ii + (ii - 1) * h_dim1;
		i__6 = ii + ii * h_dim1;
		d__3 = ((d__1 = h__[i__5].r, abs(d__1)) + (d__2 = h__[i__6].r,
			 abs(d__2))) * 1.5;
		w[i__3].r = d__3, w[i__3].i = 0.;
/* L90: */
	    }
	} else {

/*           Use eigenvalues of trailing submatrix of order NS as shifts. */

	    zlacpy_("Full", &ns, &ns, &h__[i__ - ns + 1 + (i__ - ns + 1) * 
		    h_dim1], ldh, s, &c__15, (ftnlen)4);
	    zlahqr_(&c_false, &c_false, &ns, &c__1, &ns, s, &c__15, &w[i__ - 
		    ns + 1], &c__1, &ns, &z__[z_offset], ldz, &ierr);
	    if (ierr > 0) {

/*              If ZLAHQR failed to compute all NS eigenvalues, use the */
/*              unconverged diagonal elements as the remaining shifts. */

		i__2 = ierr;
		for (ii = 1; ii <= i__2; ++ii) {
		    i__3 = i__ - ns + ii;
		    i__5 = ii + ii * 15 - 16;
		    w[i__3].r = s[i__5].r, w[i__3].i = s[i__5].i;
/* L100: */
		}
	    }
	}

/*        Form the first column of (G-w(1)) (G-w(2)) . . . (G-w(ns)) */
/*        where G is the Hessenberg submatrix H(L:I,L:I) and w is */
/*        the vector of shifts (stored in W). The result is */
/*        stored in the local array V. */

	v[0].r = 1., v[0].i = 0.;
	i__2 = ns + 1;
	for (ii = 2; ii <= i__2; ++ii) {
	    i__3 = ii - 1;
	    v[i__3].r = 0., v[i__3].i = 0.;
/* L110: */
	}
	nv = 1;
	i__2 = i__;
	for (j = i__ - ns + 1; j <= i__2; ++j) {
	    i__3 = nv + 1;
	    zcopy_(&i__3, v, &c__1, vv, &c__1);
	    i__3 = nv + 1;
	    i__5 = j;
	    z__1.r = -w[i__5].r, z__1.i = -w[i__5].i;
	    zgemv_("No transpose", &i__3, &nv, &c_b2, &h__[l + l * h_dim1], 
		    ldh, vv, &c__1, &z__1, v, &c__1, (ftnlen)12);
	    ++nv;

/*           Scale V(1:NV) so that max(abs(V(i))) = 1. If V is zero, */
/*           reset it to the unit vector. */

	    itemp = izamax_(&nv, v, &c__1);
	    i__3 = itemp - 1;
	    rtemp = (d__1 = v[i__3].r, abs(d__1)) + (d__2 = d_imag(&v[itemp - 
		    1]), abs(d__2));
	    if (rtemp == 0.) {
		v[0].r = 1., v[0].i = 0.;
		i__3 = nv;
		for (ii = 2; ii <= i__3; ++ii) {
		    i__5 = ii - 1;
		    v[i__5].r = 0., v[i__5].i = 0.;
/* L120: */
		}
	    } else {
		rtemp = max(rtemp,smlnum);
		d__1 = 1. / rtemp;
		zdscal_(&nv, &d__1, v, &c__1);
	    }
/* L130: */
	}

/*        Multiple-shift QR step */

	i__2 = i__ - 1;
	for (k = l; k <= i__2; ++k) {

/*           The first iteration of this loop determines a reflection G */
/*           from the vector V and applies it from left and right to H, */
/*           thus creating a nonzero bulge below the subdiagonal. */

/*           Each subsequent iteration determines a reflection G to */
/*           restore the Hessenberg form in the (K-1)th column, and thus */
/*           chases the bulge one step toward the bottom of the active */
/*           submatrix. NR is the order of G. */

/* Computing MIN */
	    i__3 = ns + 1, i__5 = i__ - k + 1;
	    nr = min(i__3,i__5);
	    if (k > l) {
		zcopy_(&nr, &h__[k + (k - 1) * h_dim1], &c__1, v, &c__1);
	    }
	    zlarfg_(&nr, v, &v[1], &c__1, &tau);
	    if (k > l) {
		i__3 = k + (k - 1) * h_dim1;
		h__[i__3].r = v[0].r, h__[i__3].i = v[0].i;
		i__3 = i__;
		for (ii = k + 1; ii <= i__3; ++ii) {
		    i__5 = ii + (k - 1) * h_dim1;
		    h__[i__5].r = 0., h__[i__5].i = 0.;
/* L140: */
		}
	    }
	    v[0].r = 1., v[0].i = 0.;

/*           Apply G' from the left to transform the rows of the matrix */
/*           in columns K to I2. */

	    i__3 = i2 - k + 1;
	    d_cnjg(&z__1, &tau);
	    zlarfx_("Left", &nr, &i__3, v, &z__1, &h__[k + k * h_dim1], ldh, &
		    work[1], (ftnlen)4);

/*           Apply G from the right to transform the columns of the */
/*           matrix in rows I1 to min(K+NR,I). */

/* Computing MIN */
	    i__5 = k + nr;
	    i__3 = min(i__5,i__) - i1 + 1;
	    zlarfx_("Right", &i__3, &nr, v, &tau, &h__[i1 + k * h_dim1], ldh, 
		    &work[1], (ftnlen)5);

	    if (wantz) {

/*              Accumulate transformations in the matrix Z */

		zlarfx_("Right", &nh, &nr, v, &tau, &z__[*ilo + k * z_dim1], 
			ldz, &work[1], (ftnlen)5);
	    }
/* L150: */
	}

/*        Ensure that H(I,I-1) is real. */

	i__2 = i__ + (i__ - 1) * h_dim1;
	temp.r = h__[i__2].r, temp.i = h__[i__2].i;
	if (d_imag(&temp) != 0.) {
	    d__1 = temp.r;
	    d__2 = d_imag(&temp);
	    rtemp = dlapy2_(&d__1, &d__2);
	    i__2 = i__ + (i__ - 1) * h_dim1;
	    h__[i__2].r = rtemp, h__[i__2].i = 0.;
	    z__1.r = temp.r / rtemp, z__1.i = temp.i / rtemp;
	    temp.r = z__1.r, temp.i = z__1.i;
	    if (i2 > i__) {
		i__2 = i2 - i__;
		d_cnjg(&z__1, &temp);
		zscal_(&i__2, &z__1, &h__[i__ + (i__ + 1) * h_dim1], ldh);
	    }
	    i__2 = i__ - i1;
	    zscal_(&i__2, &temp, &h__[i1 + i__ * h_dim1], &c__1);
	    if (wantz) {
		zscal_(&nh, &temp, &z__[*ilo + i__ * z_dim1], &c__1);
	    }
	}

/* L160: */
    }

/*     Failure to converge in remaining number of iterations */

    *info = i__;
    return 0;

L170:

/*     A submatrix of order <= MAXB in rows and columns L to I has split */
/*     off. Use the double-shift QR algorithm to handle it. */

    zlahqr_(&wantt, &wantz, n, &l, &i__, &h__[h_offset], ldh, &w[1], ilo, ihi,
	     &z__[z_offset], ldz, info);
    if (*info > 0) {
	return 0;
    }

/*     Decrement number of remaining iterations, and return to start of */
/*     the main loop with a new value of I. */

    itn -= its;
    i__ = l - 1;
    goto L60;

L180:
    i__1 = max(1,*n);
    work[1].r = (doublereal) i__1, work[1].i = 0.;
    return 0;

/*     End of ZHSEQR */

} /* zhseqr_ */

