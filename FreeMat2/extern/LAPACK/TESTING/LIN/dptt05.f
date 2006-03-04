      SUBROUTINE DPTT05( N, NRHS, D, E, B, LDB, X, LDX, XACT, LDXACT,
     $                   FERR, BERR, RESLTS )
*
*  -- LAPACK test routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     February 29, 1992
*
*     .. Scalar Arguments ..
      INTEGER            LDB, LDX, LDXACT, N, NRHS
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   B( LDB, * ), BERR( * ), D( * ), E( * ),
     $                   FERR( * ), RESLTS( * ), X( LDX, * ),
     $                   XACT( LDXACT, * )
*     ..
*
*  Purpose
*  =======
*
*  DPTT05 tests the error bounds from iterative refinement for the
*  computed solution to a system of equations A*X = B, where A is a
*  symmetric tridiagonal matrix of order n.
*
*  RESLTS(1) = test of the error bound
*            = norm(X - XACT) / ( norm(X) * FERR )
*
*  A large value is returned if this ratio is not less than one.
*
*  RESLTS(2) = residual from the iterative refinement routine
*            = the maximum of BERR / ( NZ*EPS + (*) ), where
*              (*) = NZ*UNFL / (min_i (abs(A)*abs(X) +abs(b))_i )
*              and NZ = max. number of nonzeros in any row of A, plus 1
*
*  Arguments
*  =========
*
*  N       (input) INTEGER
*          The number of rows of the matrices X, B, and XACT, and the
*          order of the matrix A.  N >= 0.
*
*  NRHS    (input) INTEGER
*          The number of columns of the matrices X, B, and XACT.
*          NRHS >= 0.
*
*  D       (input) DOUBLE PRECISION array, dimension (N)
*          The n diagonal elements of the tridiagonal matrix A.
*
*  E       (input) DOUBLE PRECISION array, dimension (N-1)
*          The (n-1) subdiagonal elements of the tridiagonal matrix A.
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB,NRHS)
*          The right hand side vectors for the system of linear
*          equations.
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B.  LDB >= max(1,N).
*
*  X       (input) DOUBLE PRECISION array, dimension (LDX,NRHS)
*          The computed solution vectors.  Each vector is stored as a
*          column of the matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X.  LDX >= max(1,N).
*
*  XACT    (input) DOUBLE PRECISION array, dimension (LDX,NRHS)
*          The exact solution vectors.  Each vector is stored as a
*          column of the matrix XACT.
*
*  LDXACT  (input) INTEGER
*          The leading dimension of the array XACT.  LDXACT >= max(1,N).
*
*  FERR    (input) DOUBLE PRECISION array, dimension (NRHS)
*          The estimated forward error bounds for each solution vector
*          X.  If XTRUE is the true solution, FERR bounds the magnitude
*          of the largest entry in (X - XTRUE) divided by the magnitude
*          of the largest entry in X.
*
*  BERR    (input) DOUBLE PRECISION array, dimension (NRHS)
*          The componentwise relative backward error of each solution
*          vector (i.e., the smallest relative change in any entry of A
*          or B that makes X an exact solution).
*
*  RESLTS  (output) DOUBLE PRECISION array, dimension (2)
*          The maximum over the NRHS solution vectors of the ratios:
*          RESLTS(1) = norm(X - XACT) / ( norm(X) * FERR )
*          RESLTS(2) = BERR / ( NZ*EPS + (*) )
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, IMAX, J, K, NZ
      DOUBLE PRECISION   AXBI, DIFF, EPS, ERRBND, OVFL, TMP, UNFL, XNORM
*     ..
*     .. External Functions ..
      INTEGER            IDAMAX
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           IDAMAX, DLAMCH
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX, MIN
*     ..
*     .. Executable Statements ..
*
*     Quick exit if N = 0 or NRHS = 0.
*
      IF( N.LE.0 .OR. NRHS.LE.0 ) THEN
         RESLTS( 1 ) = ZERO
         RESLTS( 2 ) = ZERO
         RETURN
      END IF
*
      EPS = DLAMCH( 'Epsilon' )
      UNFL = DLAMCH( 'Safe minimum' )
      OVFL = ONE / UNFL
      NZ = 4
*
*     Test 1:  Compute the maximum of
*        norm(X - XACT) / ( norm(X) * FERR )
*     over all the vectors X and XACT using the infinity-norm.
*
      ERRBND = ZERO
      DO 30 J = 1, NRHS
         IMAX = IDAMAX( N, X( 1, J ), 1 )
         XNORM = MAX( ABS( X( IMAX, J ) ), UNFL )
         DIFF = ZERO
         DO 10 I = 1, N
            DIFF = MAX( DIFF, ABS( X( I, J )-XACT( I, J ) ) )
   10    CONTINUE
*
         IF( XNORM.GT.ONE ) THEN
            GO TO 20
         ELSE IF( DIFF.LE.OVFL*XNORM ) THEN
            GO TO 20
         ELSE
            ERRBND = ONE / EPS
            GO TO 30
         END IF
*
   20    CONTINUE
         IF( DIFF / XNORM.LE.FERR( J ) ) THEN
            ERRBND = MAX( ERRBND, ( DIFF / XNORM ) / FERR( J ) )
         ELSE
            ERRBND = ONE / EPS
         END IF
   30 CONTINUE
      RESLTS( 1 ) = ERRBND
*
*     Test 2:  Compute the maximum of BERR / ( NZ*EPS + (*) ), where
*     (*) = NZ*UNFL / (min_i (abs(A)*abs(X) +abs(b))_i )
*
      DO 50 K = 1, NRHS
         IF( N.EQ.1 ) THEN
            AXBI = ABS( B( 1, K ) ) + ABS( D( 1 )*X( 1, K ) )
         ELSE
            AXBI = ABS( B( 1, K ) ) + ABS( D( 1 )*X( 1, K ) ) +
     $             ABS( E( 1 )*X( 2, K ) )
            DO 40 I = 2, N - 1
               TMP = ABS( B( I, K ) ) + ABS( E( I-1 )*X( I-1, K ) ) +
     $               ABS( D( I )*X( I, K ) ) + ABS( E( I )*X( I+1, K ) )
               AXBI = MIN( AXBI, TMP )
   40       CONTINUE
            TMP = ABS( B( N, K ) ) + ABS( E( N-1 )*X( N-1, K ) ) +
     $            ABS( D( N )*X( N, K ) )
            AXBI = MIN( AXBI, TMP )
         END IF
         TMP = BERR( K ) / ( NZ*EPS+NZ*UNFL / MAX( AXBI, NZ*UNFL ) )
         IF( K.EQ.1 ) THEN
            RESLTS( 2 ) = TMP
         ELSE
            RESLTS( 2 ) = MAX( RESLTS( 2 ), TMP )
         END IF
   50 CONTINUE
*
      RETURN
*
*     End of DPTT05
*
      END
