cmake_minimum_required(VERSION 3.0)

# Add the gtest library
include_directories(${PROJECT_SRC_DIR}/third_party/googletest/googletest/include)
include_directories(${PROJECT_SRC_DIR}/third_party/googletest/googletest)
add_library(gtest 
  ${PROJECT_SRC_DIR}/third_party/googletest/googletest/src/gtest-all.cc
  ${PROJECT_SRC_DIR}/third_party/googletest/googletest/src/gtest_main.cc
)

find_package(Threads REQUIRED)
target_link_libraries(gtest Threads::Threads)
set_target_properties(gtest PROPERTIES FOLDER "Libraries")

# https://cmake.org/Wiki/CMakeEmulateMakeCheck How to emulate GNU Autotools 'make check'
find_program(CMAKE_CTEST_COMMAND ctest ${CMAKE_HOST_PATH} ${CMAKE_TARGET_PATH})
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V -C $<CONFIGURATION>)
set_target_properties(check PROPERTIES FOLDER "CMake Targets")

macro(wrap_test test_name)
    add_executable(${test_name}_test ${test_name}_test.cpp)
    target_link_libraries(${test_name}_test gtest CppCodeGenVar)
    add_test(${test_name}_test ${test_name}_test)
    add_dependencies(check ${test_name}_test)
	set_target_properties(${test_name}_test PROPERTIES FOLDER "Tests")
endmacro()

wrap_test(symbol)
