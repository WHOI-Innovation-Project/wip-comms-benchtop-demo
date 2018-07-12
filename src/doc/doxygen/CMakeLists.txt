file(GLOB FIGURES RELATIVE ${project_SRC_DIR}/doc/figures ${project_SRC_DIR}/doc/figures/*)
foreach(F ${FIGURES})
  configure_file(${project_SRC_DIR}/doc/figures/${F} ${CMAKE_BINARY_DIR}/src/doc/figures/${F} COPYONLY)
endforeach()

configure_file(wip-comms.doxy.in ${CMAKE_BINARY_DIR}/src/doc/wip-comms.doxy @ONLY)

file(GLOB MD_FILES ${project_SRC_DIR}/doc/markdown/*.md)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/src/doc/latex/refman.tex ${CMAKE_BINARY_DIR}/src/doc/html/index.html
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/doc/markdown
  COMMAND ${DOXYGEN_EXECUTABLE}
  ARGS ${CMAKE_BINARY_DIR}/src/doc/wip-comms.doxy
  DEPENDS ${CMAKE_BINARY_DIR}/src/doc/wip-comms.doxy
  ${MD_FILES}
  wip_comms_config
  wip_comms_messages
  )

# run pdflatex twice (second pass correctly sets table of contents)
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/src/doc/latex/refman.pdf
  COMMAND ${PDFLATEX_COMPILER}
  ARGS  -file-line-error -halt-on-error refman.tex
  COMMAND ${PDFLATEX_COMPILER}
  ARGS  -file-line-error -halt-on-error refman.tex
  DEPENDS ${CMAKE_BINARY_DIR}/src/doc/latex/refman.tex
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/doc/latex
  VERBATIM )

add_custom_target(wip_comms_doxygen ALL
  DEPENDS ${CMAKE_BINARY_DIR}/src/doc/html/index.html ${CMAKE_BINARY_DIR}/src/doc/latex/refman.pdf wip_comms_graphviz
  COMMENT "Running doxygen to generate documentation output to ${CMAKE_BINARY_DIR}/src/doc/latex/refman.pdf and ${CMAKE_BINARY_DIR}/src/doc/html/index.html"
  VERBATIM )

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/src/doc/html" "${CMAKE_BINARY_DIR}/src/doc/latex")
