% Regression test function (black blox) for FreeMat v svn
% This function is autogenerated by helpgen.
function bbtest_success = bbtest_disp
  bbtest_success = 1;
NumErrors = 0;
try
  a = 32;

catch
  NumErrors = NumErrors + 1;
end
try
  b = 1:4;

catch
  NumErrors = NumErrors + 1;
end
try
  disp(a,b,pi)

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
