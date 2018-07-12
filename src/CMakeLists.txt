protobuf_include_dirs(${project_INC_DIR}/wip-comms/messages)
include_directories(${project_INC_DIR}/wip-comms/messages)

add_subdirectory(lib)
add_subdirectory(bin)

if(enable_testing)
add_subdirectory(test)
endif()

option(build_doc ${LATEX_DOC_STRING} OFF)
if(build_doc)
  find_package(LATEX REQUIRED)
  find_package(Doxygen REQUIRED dot)
  add_subdirectory(doc)

  # doc install
  install(DIRECTORY ${project_BUILD_DIR}/src/doc/html DESTINATION share/doc/wip-comms-doc)
  install(DIRECTORY ${project_BUILD_DIR}/src/doc/figures DESTINATION share/doc/wip-comms-doc)
  install(DIRECTORY ${project_BUILD_DIR}/src/doc/graphviz DESTINATION share/doc/wip-comms-doc FILES_MATCHING PATTERN "*.png")

  install(DIRECTORY DESTINATION share/doc/wip-comms-doc FILES_MATCHING REGEX .*\\.png)
  install(FILES ${project_BUILD_DIR}/src/doc/latex/refman.pdf DESTINATION share/doc/wip-comms-doc RENAME wip-comms.pdf)

endif()
