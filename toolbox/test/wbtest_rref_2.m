function test_val = wbtest_rref_2(verbose)
  myloc=fileparts(which('wbtest_rref_2'));
  load([myloc,dirsep,'reference',dirsep,'wbinputs.mat'])
  load([myloc,dirsep,'reference',dirsep,'wbtest_rref_2_ref.mat'])
  fail_count = 0;
  for loopi=1:numel(wbinputs)
    x1 = wbinputs{loopi};
    error_flag = 0;
  y1 = [];
  y2 = [];
    try
      [y1,y2]=rref(x1);
    catch
      error_flag = 1;
    end
    if (error_flag && ~error_refs(loopi) && ~(false))
       printf('Mismatch Errors: input %d [y1,y2]=rref(x1)\n',loopi);
       fail_count = fail_count + 1;
    elseif (~error_flag && ~error_refs(loopi) && ~wbtest_near(y1,y1_refs{loopi}) && ~(false))
      printf('Mismatch (near): input %d output 1 [y1,y2]=rref(x1)\n',loopi);
      fail_count = fail_count + 1;
    elseif (~error_flag && ~error_refs(loopi) && ~wbtest_near(y2,y2_refs{loopi}) && ~(false))
      printf('Mismatch (near): input %d output 2 [y1,y2]=rref(x1)\n',loopi);
      fail_count = fail_count + 1;
    end
  test_val = (fail_count == 0);
end