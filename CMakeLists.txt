cmake_minimum_required (VERSION 3.0)
project(CppCodeGenVar C CXX)

set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CPPCODEGENVAR_VERSION_MAJOR 0)
set(CPPCODEGENVAR_VERSION_MINOR 7)
set(CPPCODEGENVAR_VERSION ${CPPCODEGENVAR_VERSION_MAJOR}.${CPPCODEGENVAR_VERSION_MINOR} )

set (CMAKE_CXX_STANDARD 11)

option(BUILD_DOCUMENTATION "Build html documentation with Sphinx." OFF)
option(BUILD_TESTING "Build the tests" ON)
option(BUILD_EXAMPLES "Build the examples" ON)

set(CMAKE_DEBUG_POSTFIX "d")
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_COMPILER_IS_GNUCXX)
  option(BUILD_COVERAGE "Enable code coverage generation (gcc only)" OFF)

  if (BUILD_COVERAGE)
    message("Building coverage information.")
    set(CMAKE_BUILD_TYPE "Debug")
    add_definitions(--coverage -fprofile-arcs -ftest-coverage)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  endif()
endif()

add_subdirectory(src)

if (BUILD_DOCUMENTATION)
  add_subdirectory(docs)
endif()

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

# Install targets:
configure_file(cmake/CppCodeGenVarConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CppCodeGenVarConfig.cmake @ONLY )
configure_file(cmake/CppCodeGenVarConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CppCodeGenVarConfigVersion.cmake @ONLY )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/CppCodeGenVarConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/CppCodeGenVarConfigVersion.cmake
        DESTINATION cmake )
      
install(EXPORT CppCodeGenVarExport DESTINATION cmake FILE CppCodeGenVarTargets.cmake )
