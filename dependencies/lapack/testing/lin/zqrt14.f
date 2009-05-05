      DOUBLE PRECISION FUNCTION ZQRT14( TRANS, M, N, NRHS, A, LDA, X,
     $                 LDX, WORK, LWORK )
*
*  -- LAPACK test routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     February 29, 1992
*
*     .. Scalar Arguments ..
      CHARACTER          TRANS
      INTEGER            LDA, LDX, LWORK, M, N, NRHS
*     ..
*     .. Array Arguments ..
      COMPLEX*16         A( LDA, * ), WORK( LWORK ), X( LDX, * )
*     ..
*
*  Purpose
*  =======
*
*  ZQRT14 checks whether X is in the row space of A or A'.  It does so
*  by scaling both X and A such that their norms are in the range
*  [sqrt(eps), 1/sqrt(eps)], then computing a QR factorization of [A,X]
*  (if TRANS = 'C') or an LQ factorization of [A',X]' (if TRANS = 'N'),
*  and returning the norm of the trailing triangle, scaled by
*  MAX(M,N,NRHS)*eps.
*
*  Arguments
*  =========
*
*  TRANS   (input) CHARACTER*1
*          = 'N':  No transpose, check for X in the row space of A
*          = 'C':  Conjugate transpose, check for X in row space of A'.
*
*  M       (input) INTEGER
*          The number of rows of the matrix A.
*
*  N       (input) INTEGER
*          The number of columns of the matrix A.
*
*  NRHS    (input) INTEGER
*          The number of right hand sides, i.e., the number of columns
*          of X.
*
*  A       (input) COMPLEX*16 array, dimension (LDA,N)
*          The M-by-N matrix A.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.
*
*  X       (input) COMPLEX*16 array, dimension (LDX,NRHS)
*          If TRANS = 'N', the N-by-NRHS matrix X.
*          IF TRANS = 'C', the M-by-NRHS matrix X.
*
*  LDX     (input) INTEGER
*          The leading dimension of the array X.
*
*  WORK    (workspace) COMPLEX*16 array dimension (LWORK)
*
*  LWORK   (input) INTEGER
*          length of workspace array required
*          If TRANS = 'N', LWORK >= (M+NRHS)*(N+2);
*          if TRANS = 'C', LWORK >= (N+NRHS)*(M+2).
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            TPSD
      INTEGER            I, INFO, J, LDWORK
      DOUBLE PRECISION   ANRM, ERR, XNRM
*     ..
*     .. Local Arrays ..
      DOUBLE PRECISION   RWORK( 1 )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      DOUBLE PRECISION   DLAMCH, ZLANGE
      EXTERNAL           LSAME, DLAMCH, ZLANGE
*     ..
*     .. External Subroutines ..
      EXTERNAL           XERBLA, ZGELQ2, ZGEQR2, ZLACPY, ZLASCL
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, DBLE, DCONJG, MAX, MIN
*     ..
*     .. Executable Statements ..
*
      ZQRT14 = ZERO
      IF( LSAME( TRANS, 'N' ) ) THEN
         LDWORK = M + NRHS
         TPSD = .FALSE.
         IF( LWORK.LT.( M+NRHS )*( N+2 ) ) THEN
            CALL XERBLA( 'ZQRT14', 10 )
            RETURN
         ELSE IF( N.LE.0 .OR. NRHS.LE.0 ) THEN
            RETURN
         END IF
      ELSE IF( LSAME( TRANS, 'C' ) ) THEN
         LDWORK = M
         TPSD = .TRUE.
         IF( LWORK.LT.( N+NRHS )*( M+2 ) ) THEN
            CALL XERBLA( 'ZQRT14', 10 )
            RETURN
         ELSE IF( M.LE.0 .OR. NRHS.LE.0 ) THEN
            RETURN
         END IF
      ELSE
         CALL XERBLA( 'ZQRT14', 1 )
         RETURN
      END IF
*
*     Copy and scale A
*
      CALL ZLACPY( 'All', M, N, A, LDA, WORK, LDWORK )
      ANRM = ZLANGE( 'M', M, N, WORK, LDWORK, RWORK )
      IF( ANRM.NE.ZERO )
     $   CALL ZLASCL( 'G', 0, 0, ANRM, ONE, M, N, WORK, LDWORK, INFO )
*
*     Copy X or X' into the right place and scale it
*
      IF( TPSD ) THEN
*
*        Copy X into columns n+1:n+nrhs of work
*
         CALL ZLACPY( 'All', M, NRHS, X, LDX, WORK( N*LDWORK+1 ),
     $                LDWORK )
         XNRM = ZLANGE( 'M', M, NRHS, WORK( N*LDWORK+1 ), LDWORK,
     $          RWORK )
         IF( XNRM.NE.ZERO )
     $      CALL ZLASCL( 'G', 0, 0, XNRM, ONE, M, NRHS,
     $                   WORK( N*LDWORK+1 ), LDWORK, INFO )
         ANRM = ZLANGE( 'One-norm', M, N+NRHS, WORK, LDWORK, RWORK )
*
*        Compute QR factorization of X
*
         CALL ZGEQR2( M, N+NRHS, WORK, LDWORK,
     $                WORK( LDWORK*( N+NRHS )+1 ),
     $                WORK( LDWORK*( N+NRHS )+MIN( M, N+NRHS )+1 ),
     $                INFO )
*
*        Compute largest entry in upper triangle of
*        work(n+1:m,n+1:n+nrhs)
*
         ERR = ZERO
         DO 20 J = N + 1, N + NRHS
            DO 10 I = N + 1, MIN( M, J )
               ERR = MAX( ERR, ABS( WORK( I+( J-1 )*M ) ) )
   10       CONTINUE
   20    CONTINUE
*
      ELSE
*
*        Copy X' into rows m+1:m+nrhs of work
*
         DO 40 I = 1, N
            DO 30 J = 1, NRHS
               WORK( M+J+( I-1 )*LDWORK ) = DCONJG( X( I, J ) )
   30       CONTINUE
   40    CONTINUE
*
         XNRM = ZLANGE( 'M', NRHS, N, WORK( M+1 ), LDWORK, RWORK )
         IF( XNRM.NE.ZERO )
     $      CALL ZLASCL( 'G', 0, 0, XNRM, ONE, NRHS, N, WORK( M+1 ),
     $                   LDWORK, INFO )
*
*        Compute LQ factorization of work
*
         CALL ZGELQ2( LDWORK, N, WORK, LDWORK, WORK( LDWORK*N+1 ),
     $                WORK( LDWORK*( N+1 )+1 ), INFO )
*
*        Compute largest entry in lower triangle in
*        work(m+1:m+nrhs,m+1:n)
*
         ERR = ZERO
         DO 60 J = M + 1, N
            DO 50 I = J, LDWORK
               ERR = MAX( ERR, ABS( WORK( I+( J-1 )*LDWORK ) ) )
   50       CONTINUE
   60    CONTINUE
*
      END IF
*
      ZQRT14 = ERR / ( DBLE( MAX( M, N, NRHS ) )*DLAMCH( 'Epsilon' ) )
*
      RETURN
*
*     End of ZQRT14
*
      END
