cmake_minimum_required (VERSION 3.0)
find_package(SymEngine CONFIG REQUIRED)
include_directories(${SYMENGINE_INCLUDE_DIRS})

set(HDRS 
  ${PROJECT_SRC_DIR}/include/codegenvar/BooleanEvaluator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/Eigen
  ${PROJECT_SRC_DIR}/include/codegenvar/Scalar.h
  ${PROJECT_SRC_DIR}/include/codegenvar/Symbol.h
  ${PROJECT_SRC_DIR}/include/codegenvar/SymbolWithEigenBindings.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/StringGenerator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/CodeGenerator.h
  ${PROJECT_SRC_DIR}/include/codegenvar/generator/DerivateGenerator.h
)

set(SRCS 
  ${PROJECT_SRC_DIR}/src/BooleanEvaluator.cpp
  ${PROJECT_SRC_DIR}/src/Scalar.cpp
  ${PROJECT_SRC_DIR}/src/Symbol.cpp
  ${PROJECT_SRC_DIR}/src/generator/CodeGenerator.cpp
  ${PROJECT_SRC_DIR}/src/generator/DerivateGenerator.cpp
  ${PROJECT_SRC_DIR}/src/generator/StringGenerator.cpp
)

set(INTERNALS
  BooleanEvaluatorPrivate.h
  Error.h
  SymbolPrivate.h
)
source_group(internal FILES ${INTERNALS})

add_library(CppCodeGenVar STATIC ${HDRS} ${SRCS} ${INTERNALS})
target_link_libraries(CppCodeGenVar ${SYMENGINE_LIBRARIES})
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
set_target_properties(CppCodeGenVar PROPERTIES FOLDER "Libraries")

# Enable PIC, so shared libraries can link against it CppCodeGenVar.
set_target_properties(CppCodeGenVar PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(DIRECTORY ${PROJECT_SRC_DIR}/include/codegenvar DESTINATION include)
install(TARGETS CppCodeGenVar 
        EXPORT CppCodeGenVarExport
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib )
