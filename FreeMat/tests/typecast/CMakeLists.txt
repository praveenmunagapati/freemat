ENABLE_TESTING()
ADD_TEST(wbtest_cast_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''cell'')','wbtest_cast_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_2 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''struct'')','wbtest_cast_2','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_3 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''uint8'')','wbtest_cast_3','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_4 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''int8'')','wbtest_cast_4','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_5 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''uint16'')','wbtest_cast_5','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_6 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''int16'')','wbtest_cast_6','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_7 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''uint32'')','wbtest_cast_7','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_8 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''int32'')','wbtest_cast_8','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_9 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''uint64'')','wbtest_cast_9','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_10 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''int64'')','wbtest_cast_10','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_11 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''single'')','wbtest_cast_11','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_12 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''double'')','wbtest_cast_12','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_cast_13 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=cast(x1,''char'')','wbtest_cast_13','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_char_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=char(x1,x2)#(any(loopi==[4,21]))','wbtest_char_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_bin2dec_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=bin2dec(x1)','wbtest_bin2dec_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_dec2bin_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=dec2bin(x1)#(any(loopi==[6,11,15,49:53,58]))','wbtest_dec2bin_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_logical_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=logical(x1)','wbtest_logical_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_string_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=string(x1)','wbtest_string_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_uint8_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=uint8(x1)','wbtest_uint8_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_uint16_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=uint16(x1)','wbtest_uint16_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_uint32_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=uint32(x1)','wbtest_uint32_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_uint64_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=uint64(x1)','wbtest_uint64_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(test_uint64_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wrap_test('test_uint64_1')")
ADD_TEST(wbtest_int8_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=int8(x1)','wbtest_int8_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_int16_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=int16(x1)','wbtest_int16_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_int32_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=int32(x1)','wbtest_int32_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_int64_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=int64(x1)','wbtest_int64_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_float_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=float(x1)','wbtest_float_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(test_sparse58 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wrap_test('test_sparse58')")
ADD_TEST(wbtest_double_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=double(x1)','wbtest_double_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(wbtest_complex_1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wb_test('@$exact#y1=complex(x1,x2)','wbtest_complex_1','${CMAKE_SOURCE_DIR}/tests/reference')")
ADD_TEST(test_dcomplex1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wrap_test('test_dcomplex1')")
ADD_TEST(test_int2bin1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wrap_test('test_int2bin1')")
ADD_TEST(test_bin2int1 ${FreeMat_Loc} "-e" "-nogui" "-t" "-nogreet" "-p" "${CMAKE_SOURCE_DIR}/tests/typecast" "-f" "wrap_test('test_bin2int1')")
