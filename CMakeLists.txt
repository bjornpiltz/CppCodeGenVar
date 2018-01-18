cmake_minimum_required (VERSION 3.0)
project(CppCodeGenVar C CXX)
set (CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/eigen3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ceres-solver/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ceres-solver/config)

set( HDRS 
  include/codegenvar/Number.h
  include/codegenvar/Symbol.h
  include/codegenvar/SymbolWithEigenBindings.h
  include/codegenvar/generator/NumericalGenerator.h
  include/codegenvar/generator/StringGenerator.h
  include/codegenvar/generator/CodeGenerator.h
  include/codegenvar/generator/DerivateGenerator
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

if(CMAKE_COMPILER_IS_GNUCXX)
	option(BUILD_COVERAGE "Enable code coverage generation (gcc only)" OFF)

	IF(BUILD_COVERAGE)
		message("Building coverage information.")
		set(CMAKE_BUILD_TYPE "Debug")
		add_definitions(--coverage -fprofile-arcs -ftest-coverage)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
	endif()
endif()

set(INTERNALS  
  include/codegenvar/internal/AbstractExpression.h
  include/codegenvar/internal/ConstantExpression.h
  include/codegenvar/internal/Error.h
  include/codegenvar/internal/Expression.h
  include/codegenvar/internal/VariableExpression.h
)

add_library(CppCodeGenVar ${HDRS} ${SRCS} ${INTERNALS})
target_link_libraries(CppCodeGenVar)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)
source_group(internal FILES ${INTERNALS})

add_executable (symbol_example tests/example.cpp)
target_link_libraries(symbol_example CppCodeGenVar)

add_executable (code_example tests/code_example.cpp)
target_link_libraries(code_example CppCodeGenVar)

add_executable (eigen_test tests/eigen_test.cpp)
target_link_libraries(eigen_test CppCodeGenVar)

add_executable (rotation_test tests/rotation_test.cpp tests/SnavelyReprojectionError.h)
target_link_libraries(rotation_test CppCodeGenVar)

add_executable (derivate_test tests/derivate_test.cpp tests/SnavelyReprojectionError.h)
target_link_libraries(derivate_test CppCodeGenVar)

enable_testing()
add_subdirectory(test)
