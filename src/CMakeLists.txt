cmake_minimum_required (VERSION 3.0)

set(HDRS 
  ${PROJECT_SRC_DIR}/include/codegenvar/Number.h
  ${PROJECT_SRC_DIR}/include/codegenvar/Symbol.h
  ${PROJECT_SRC_DIR}/include/codegenvar/SymbolWithEigenBindings.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/NumericalGenerator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/StringGenerator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/CodeGenerator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/DerivateGenerator.h
)

set(SRCS 
  ${PROJECT_SRC_DIR}/src/AbstractExpression.cpp
  ${PROJECT_SRC_DIR}/src/Expression.cpp
  ${PROJECT_SRC_DIR}/src/Number.cpp 
  ${PROJECT_SRC_DIR}/src/Symbol.cpp 
  ${PROJECT_SRC_DIR}/src/generator/CodeGenerator.cpp
  ${PROJECT_SRC_DIR}/src/generator/DerivateGenerator.cpp
  ${PROJECT_SRC_DIR}/src/generator/NumericalGenerator.cpp
  ${PROJECT_SRC_DIR}/src/generator/StringGenerator.cpp
)

set(INTERNALS  
  AbstractExpression.h
  ConstantExpression.h
  Error.h
  Expression.h
  VariableExpression.h
)
source_group(internal FILES ${INTERNALS})

add_library(CppCodeGenVar STATIC ${HDRS} ${SRCS} ${INTERNALS})
target_include_directories(CppCodeGenVar 
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SRC_DIR}/include>
    $<INSTALL_INTERFACE:include>  
    
  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SRC_DIR}/third_party/eigen3
    ${PROJECT_SRC_DIR}/third_party/ceres-solver/include
    ${PROJECT_SRC_DIR}/third_party/ceres-solver/config
)
  
# Enable PIC, so shared libraries can link against it CppCodeGenVar.
set_target_properties(CppCodeGenVar PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(DIRECTORY ${PROJECT_SRC_DIR}/include/codegenvar DESTINATION include)
install(TARGETS CppCodeGenVar 
        EXPORT CppCodeGenVarExport
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib )
      