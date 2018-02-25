find_program(SPHINX_EXECUTABLE
             NAMES sphinx-build
             PATHS /usr/bin /usr/local/bin /opt/local/bin
             DOC "Sphinx documentation generator")
           
# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

# Install documentation
install(DIRECTORY ${SPHINX_HTML_DIR}
        DESTINATION docs
        COMPONENT Doc
        PATTERN "${SPHINX_HTML_DIR}/*")

file(GLOB DOC_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.rst")
 
add_custom_target(docs ALL
                 ${SPHINX_EXECUTABLE} 
                 -b html 
                 ${CMAKE_CURRENT_SOURCE_DIR}
                 ${SPHINX_HTML_DIR}
                 COMMENT "Building HTML documentation with Sphinx"
                 SOURCES ${DOC_SOURCES} conf.py
)
set_target_properties(docs PROPERTIES FOLDER "CMake Targets")
