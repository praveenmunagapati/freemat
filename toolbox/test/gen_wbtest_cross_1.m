function gen_wbtest_cross_1(verbose)
  load reference/wbinputs.mat
  error_refs = 0;
  y1 = []; y1_refs = {};
  try
    y1=cross([1,2,3],[7,5,2]);
  catch
    error_refs = 1;
  end
  if (~error_refs)
  y1_refs = {y1};
  end
  save reference/wbtest_cross_1_ref.mat error_refs  y1_refs 
end