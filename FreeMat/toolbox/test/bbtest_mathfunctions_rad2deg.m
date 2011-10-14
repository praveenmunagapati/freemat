% Regression test function (black box) for rad2deg
% This function is autogenerated by helpgen.py
function bbtest_success = bbtest_mathfunctions_rad2deg
  bbtest_success = 1;
NumErrors = 0;
try
  rad2deg(1) % one radian is about 57 degrees
catch
  NumErrors = NumErrors + 1;
end
try
  rad2deg(pi/4) % should be 45 degrees
catch
  NumErrors = NumErrors + 1;
end
try
  rad2deg(2*pi) % Note that this is 360 not 0 degrees
catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end