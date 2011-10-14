% Regression test function (black box) for cell
% This function is autogenerated by helpgen.py
function bbtest_success = bbtest_array_cell
  bbtest_success = 1;
NumErrors = 0;
try
  cell(2,3,2)
catch
  NumErrors = NumErrors + 1;
end
try
  cell(1,3)
catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  cell([2,6])
catch
  NumErrors = NumErrors + 1;
end
try
  cell([1,3])
catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end