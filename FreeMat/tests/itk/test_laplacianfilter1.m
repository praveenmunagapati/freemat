function t = test_laplacianfilter1
  a = zeros(5);
  a(3,3) = 1;
  b = laplacianfilter(a);
  c = zeros(5);
  c(3,3) = -4;
  c(2,3) = 1;
  c(4,3) = 1;
  c(3,2) = 1;
  c(3,4) = 1;
  t = issame(c,b);
