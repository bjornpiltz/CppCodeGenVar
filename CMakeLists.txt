cmake_minimum_required (VERSION 3.0)
project(symbol C CXX)
set (CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Eigen3 3.3 REQUIRED CONFIG)
add_definitions(-DHAVE_EIGEN)
include_directories( ${EIGEN3_INCLUDE_DIR} )

find_package(Ceres REQUIRED CONFIG)

set( HDRS 
  include/Number.h
  include/Symbol.h
  include/SymbolWithEigenBindings.h
  include/generator/NumericalGenerator.h
  include/generator/StringGenerator.h
  include/generator/CodeGenerator.h
  include/generator/DerivateGenerator
)
set(SRCS 
  src/AbstractExpression.cpp
  src/Expression.cpp
  src/Number.cpp 
  src/Symbol.cpp 
  src/generator/CodeGenerator.cpp
  src/generator/DerivateGenerator.cpp
  src/generator/NumericalGenerator.cpp
  src/generator/StringGenerator.cpp
)
set(INTERNALS  
  include/internal/AbstractExpression.h
  include/internal/ConstantExpression.h
  include/internal/Error.h
  include/internal/Expression.h
  include/internal/VariableExpression.h
)

add_library(symbol ${HDRS} ${SRCS} ${INTERNALS})
target_link_libraries(symbol ceres)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)
source_group(internal FILES ${INTERNALS})

add_executable (symbol_example tests/example.cpp)
target_link_libraries(symbol_example symbol)

add_executable (code_example tests/code_example.cpp)
target_link_libraries(code_example symbol)

add_executable (eigen_test tests/eigen_test.cpp)
target_link_libraries(eigen_test symbol)

add_executable (rotation_test tests/rotation_test.cpp tests/SnavelyReprojectionError.h)
target_link_libraries(rotation_test symbol)

add_executable (derivate_test tests/derivate_test.cpp tests/SnavelyReprojectionError.h)
target_link_libraries(derivate_test symbol)

enable_testing()
add_subdirectory(test)