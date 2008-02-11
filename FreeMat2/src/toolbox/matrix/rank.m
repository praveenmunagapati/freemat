%!
%@Module RANK Calculate the Rank of a Matrix
%@@Section ARRAY
%@@Usage
%Returns the rank of a matrix.  There are two ways to use
%the @|rank| function is
%@[
%   y = rank(A,tol)
%@]
%where @|tol| is the tolerance to use when computing the
%rank.  The second form is
%@[
%   y = rank(A)
%@]
%in which case the tolerance @|tol| is chosen as
%@[
%   tol = max(size(A))*max(s)*eps,
%@]
%where @|s| is the vector of singular values of @|A|.  The
%rank is computed using the singular value decomposition @|svd|.
%@@Examples
%Some examples of matrix rank calculations
%@<
%rank([1,3,2;4,5,6])
%rank([1,2,3;2,4,6])
%@>
%Here we construct an ill-conditioned matrix, and show the use 
%of the @|tol| argument.
%@<
%A = [1,0;0,eps/2]
%rank(A)
%rank(A,eps/8)
%@>
%!
% Copyright (c) 2005 Samit Basu
function y = rank(A,tol)
s = svd(A);
if (~isset('tol'))
  tol = max(size(A))*s(1)*teps(A);
end
y = sum(s>tol);
