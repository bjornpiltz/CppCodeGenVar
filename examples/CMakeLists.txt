cmake_minimum_required (VERSION 3.0)

# Add an artifical target that builds all examples
add_custom_target(examples)
set_target_properties(examples PROPERTIES FOLDER "CMake Targets")

macro(add_example example_name)
  add_executable(${example_name}_example ${example_name}.cpp)
  set_target_properties(${example_name}_example PROPERTIES FOLDER "Examples")
  target_link_libraries(${example_name}_example CppCodeGenVar)
  add_dependencies(examples ${example_name}_example)
  list(APPEND examples ${example_name}_example)
endmacro()

include_directories(${PROJECT_SRC_DIR}/third_party/eigen3)

add_example(derivate)
add_example(code)
add_example(conditional)
add_example(eigen)
add_example(rodrigues)
add_example(rotation)
add_example(symbol)
add_example(eigen_inverse)

set(CPPCODEGENVAR_EXAMPLES ${examples} PARENT_SCOPE)
