% Regression test function (black blox) for FreeMat v svn
% This function is autogenerated by helpgen.
function bbtest_success = bbtest_acoth
  bbtest_success = 1;
NumErrors = 0;
try
  x = linspace(1,pi);

catch
  NumErrors = NumErrors + 1;
end
try
  plot(x,acoth(x)); grid('on');

catch
  NumErrors = NumErrors + 1;
end
try
  mprint('acothplot');

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
