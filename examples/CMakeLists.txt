cmake_minimum_required (VERSION 3.0)

add_executable (symbol_example symbol.cpp)
target_link_libraries(symbol_example CppCodeGenVar)

add_executable (code_example code.cpp)
target_link_libraries(code_example CppCodeGenVar)

include_directories(${PROJECT_SRC_DIR}/third_party/eigen3)

add_executable (eigen_test eigen.cpp)
target_link_libraries(eigen_test CppCodeGenVar)

add_executable (rotation_test rotation.cpp SnavelyReprojectionError.h)
target_link_libraries(rotation_test CppCodeGenVar)

add_executable (derivate_test derivate.cpp SnavelyReprojectionError.h)
target_link_libraries(derivate_test CppCodeGenVar)
