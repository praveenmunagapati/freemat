function x = test_sparse14
ai = int32(10*rand(8));
ar = int32(10*rand(8));
ai(ai<7) = 0;
ar(ar<7) = 0;
a = dcomplex(ar+i*ai);
A = sparse(a);
b = [5;2;3;3;1];
c = [1,4,6,8];
A = sparse(a);
C = A(b,c);
c = a(b,c);
x = testeq(c,C);
