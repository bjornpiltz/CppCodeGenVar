cmake_minimum_required(VERSION 3.0)

# Add the gtest library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest)
add_library(gtest 
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest_main.cc
)

# https://cmake.org/Wiki/CMakeEmulateMakeCheck How to emulate GNU Autotools 'make check'
find_program(CMAKE_CTEST_COMMAND ctest ${CMAKE_HOST_PATH} ${CMAKE_TARGET_PATH})
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION>)

macro(wrap_test test_name)
    add_executable(${test_name}_test ${test_name}_test.cpp)
    target_link_libraries(${test_name}_test gtest CppCodeGenVar)
    add_test(${test_name}_test ${test_name}_test)
    add_dependencies(check ${test_name}_test)
endmacro()

wrap_test(symbol)