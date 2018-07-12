# generate the PNGs to the CMAKE_SOURCE_DIR and to CMAKE_BINARY_DIR
# so that we can commit them to be viewable
# by the standalone Bitbucket MarkDown viewer.
function(GRAPHVIZ_GENERATE_PNG FILES)
  if(NOT ARGN)
    message(SEND_ERROR "Error: GRAPHVIZ_GENERATE_PNG() called without any dot files")
    return()
  endif()

  
  set(${FILES})
  
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} ${ABS_FIL})
    get_filename_component(REL_DIR ${REL_PATH} DIRECTORY)
    
    list(APPEND ${FILES} "${CMAKE_SOURCE_DIR}/${REL_DIR}/${FIL_WE}.png")
    list(APPEND ${FILES} "${CMAKE_BINARY_DIR}/${REL_DIR}/${FIL_WE}.png")
    
    add_custom_command(
      OUTPUT "${CMAKE_SOURCE_DIR}/${REL_DIR}/${FIL_WE}.png"
      OUTPUT "${CMAKE_BINARY_DIR}/${REL_DIR}/${FIL_WE}.png"
      COMMAND dot
      ARGS -Tpng  ${ABS_FIL} > ${CMAKE_SOURCE_DIR}/${REL_DIR}/${FIL_WE}.png
      DEPENDS ${ABS_FIL}
      COMMAND ${CMAKE_COMMAND} -E copy
      ARGS ${CMAKE_SOURCE_DIR}/${REL_DIR}/${FIL_WE}.png ${CMAKE_BINARY_DIR}/${REL_DIR}/${FIL_WE}.png
      COMMENT "Running Graphviz Dot generator on ${FIL} to create ${FIL_WE}.png"
      VERBATIM )

    
  endforeach()

  set_source_files_properties(${${FILES}} PROPERTIES GENERATED TRUE)
  set(${FILES} ${${FILES}} PARENT_SCOPE)

endfunction()

