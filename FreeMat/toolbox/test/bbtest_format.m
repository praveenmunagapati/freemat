% Regression test function (black blox) for FreeMat v4.0
% This function is autogenerated by helpgen.
function bbtest_success = bbtest_format
  bbtest_success = 1;
NumErrors = 0;
try
  format short

catch
  NumErrors = NumErrors + 1;
end
try
  a = randn(4)

catch
  NumErrors = NumErrors + 1;
end
try
  b = float(randn(4))

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  format long

catch
  NumErrors = NumErrors + 1;
end
try
  a

catch
  NumErrors = NumErrors + 1;
end
try
  b

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  format short

catch
  NumErrors = NumErrors + 1;
end
try
  a*1e4

catch
  NumErrors = NumErrors + 1;
end
try
  a*1e-4

catch
  NumErrors = NumErrors + 1;
end
try
  b*1e4

catch
  NumErrors = NumErrors + 1;
end
try
  b*1e-4

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  format short e

catch
  NumErrors = NumErrors + 1;
end
try
  a*1e4

catch
  NumErrors = NumErrors + 1;
end
try
  a*1e-4

catch
  NumErrors = NumErrors + 1;
end
try
  b*1e4

catch
  NumErrors = NumErrors + 1;
end
try
  b*1e-4

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  format short

catch
  NumErrors = NumErrors + 1;
end
try
  t = format

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end