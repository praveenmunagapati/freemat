% Regression test function (black blox) for FreeMat v4.0
% This function is autogenerated by helpgen.
function bbtest_success = bbtest_ones
  bbtest_success = 1;
NumErrors = 0;
try
  ones(2,3,2)

catch
  NumErrors = NumErrors + 1;
end
try
  ones(1,3)

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  ones([2,6])

catch
  NumErrors = NumErrors + 1;
end
try
  ones([1,3])

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  uint16(ones(3))

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
