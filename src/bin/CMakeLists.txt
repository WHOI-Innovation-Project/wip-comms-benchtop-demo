option(build_sandbox "Build sandbox prototyping code" OFF)

if(build_sandbox)
  add_subdirectory(sandbox)
endif()

add_subdirectory(gps_driver)
add_subdirectory(environmental_driver)
