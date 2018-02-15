#------------------------------------------------- 
# some argument checking: 
# test_cmd is the command to run with all its arguments 
if( NOT test_cmd ) 
  message( FATAL_ERROR "Variable test_cmd not defined" ) 
endif() 

if(output_blessed AND EXISTS "${output_blessed}")

 # output_blessed contains the name of the "ground truth" output file.
  set(output_test "${CMAKE_BINARY_DIR}/${example_name}.txt")
  execute_process(COMMAND ${test_cmd} 
	OUTPUT_FILE ${output_test} 
	RESULT_VARIABLE test_not_successful)

  execute_process( COMMAND 
	    ${CMAKE_COMMAND} -E compare_files ${output_blessed} ${output_test}
	    RESULT_VARIABLE test_not_successful 
  )
  if( test_not_successful ) 
	message( SEND_ERROR "${output_test} does not match ${output_blessed}!" ) 
  else()
    message( "${output_test} matches ${output_blessed}." )
  endif() 

else()
  # We don't have ground truth data for the output. We can still check that tha exe returns zero.
  execute_process(COMMAND ${test_cmd}
	    OUTPUT_QUIET
		RESULT_VARIABLE test_not_successful)

  if( test_not_successful ) 
	message( SEND_ERROR "${test_cmd} returned ${test_not_successful}" ) 
  endif() 
endif()
#------------------------------------------------- 