find_program(SPHINX_EXECUTABLE
             NAMES sphinx-build
             PATHS /usr/bin /usr/local/bin /opt/local/bin
             DOC "Sphinx documentation generator")
           
# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

# Install documentation
#TODO
#install(DIRECTORY ${SPHINX_HTML_DIR}
#        DESTINATION ...
#        COMPONENT Doc
#        PATTERN "${SPHINX_HTML_DIR}/*")

# Build examples
add_custom_target(run_examples ALL
    COMMAND symbol_example > docs/symbol_example.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
)

file(GLOB DOC_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.rst")
 
# TODO: How to access ${CMAKE_CURRENT_BINARY_DIR}/symbol_example.txt from the rst files
# foreach (DOC_FILE ${DOC_SOURCES})
#     configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${DOC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${DOC_FILE})
# endforeach()
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

add_custom_target(docs ALL
                 ${SPHINX_EXECUTABLE} 
                 -b html 
                 ${CMAKE_CURRENT_SOURCE_DIR}
                 ${SPHINX_HTML_DIR}
                 COMMENT "Building HTML documentation with Sphinx"
                 SOURCES ${DOC_SOURCES}
                 DEPENDS run_examples
)
