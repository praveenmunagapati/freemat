% Regression test function (black blox) for FreeMat v svn
% This function is autogenerated by helpgen.
function bbtest_success = bbtest_strncmp
  bbtest_success = 1;
NumErrors = 0;
try
  x1 = 'astring';

catch
  NumErrors = NumErrors + 1;
end
try
  x2 = 'bstring';

catch
  NumErrors = NumErrors + 1;
end
try
  x3 = 'astring';

catch
  NumErrors = NumErrors + 1;
end
try
  strncmp(x1,x2,4)

catch
  NumErrors = NumErrors + 1;
end
try
  strncmp(x1,x3,4)

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  x = {'ast','bst',43,'astr'}

catch
  NumErrors = NumErrors + 1;
end
try
  p = strncmp(x,'ast',3)

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  strncmp({'this','is','a','pickle'},{'think','is','to','pickle'},3)

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
NumErrors = 0;
try
  strncmp({'this','is','a','pickle'},['peter ';'piper ';'hated ';'pickle'],4);

catch
  NumErrors = NumErrors + 1;
end
if (NumErrors ~= 0) bbtest_success = 0; return; end
