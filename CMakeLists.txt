project(wip-comms C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdocumentation -Wall")

cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

# require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.58.0 COMPONENTS date_time)

find_package(ProtobufLocal REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

find_package(DCCL REQUIRED)
get_target_property(DCCL_INCLUDE_DIR dccl INTERFACE_INCLUDE_DIRECTORIES)

include_directories("${DCCL_INCLUDE_DIR}")
protobuf_include_dirs("${DCCL_INCLUDE_DIR}")

find_package(GOBY REQUIRED)
get_target_property(GOBY_INCLUDE_DIR goby_middleware INTERFACE_INCLUDE_DIRECTORIES)

message("Using Goby in ${GOBY_INCLUDE_DIR}")
include_directories("${GOBY_INCLUDE_DIR}")
protobuf_include_dirs("${GOBY_INCLUDE_DIR}")

# todo: improve with findpackage

set(PROGRESSIVE_IMAGERY_PATH /home/gobysoft/progressive-imagery)
include_directories(${PROGRESSIVE_IMAGERY_PATH}/include)


# enable testing
option(enable_testing "Enable building of tests using CTest (if set to ON, you can run tests with 'make test')" ON)
if(enable_testing)
  enable_testing()
  include(CTest)
endif()

include(ProjectVersion)

# create variables for various directories
get_filename_component(project_SRC_DIR src ABSOLUTE)
get_filename_component(project_BUILD_DIR ${CMAKE_BINARY_DIR} ABSOLUTE)
get_filename_component(project_SCRIPTS_DIR scripts ABSOLUTE)

# fetch all the local directories for generated code
get_filename_component(project_LIB_DIR ${project_BUILD_DIR}/lib ABSOLUTE)
get_filename_component(project_BIN_DIR ${project_BUILD_DIR}/bin ABSOLUTE)
get_filename_component(project_INC_DIR ${project_BUILD_DIR}/include ABSOLUTE)
get_filename_component(project_SHARE_DIR ${project_BUILD_DIR}/share ABSOLUTE)

## set the cmake defaults for libraries and binaries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${project_LIB_DIR} CACHE PATH 
  "Output directory for the dynamic libraries" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${project_BIN_DIR} CACHE PATH
  "Output directory for the binaries" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${project_LIB_DIR} CACHE PATH 
  "Output directory for the static libraries (archives)" )

include_directories(${project_INC_DIR})
protobuf_include_dirs(${project_INC_DIR})


## configure to build/include
file(GLOB_RECURSE INCLUDE_FILES RELATIVE ${project_SRC_DIR}/lib
  src/lib/*.proto
  src/lib/*.h
  )
foreach(I ${INCLUDE_FILES})
  configure_file(${project_SRC_DIR}/lib/${I} ${project_INC_DIR}/wip-comms/${I} @ONLY)
endforeach()

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/include")

## set instructions for `make install`

# include
install(DIRECTORY ${project_INC_DIR}/ DESTINATION include
  FILES_MATCHING REGEX .*\\.h|.*\\.proto)

# bin
macro(project_install_bin target_bin)
  install(TARGETS ${target_bin} DESTINATION bin)
endmacro()

# lib
macro(project_install_lib target_lib)
  set_property(TARGET ${target_lib} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${project_INC_DIR}>
    $<INSTALL_INTERFACE:include>  # <prefix>/include
    )

  set_target_properties(${target_lib} PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_SOVERSION}")
  
  install(TARGETS ${target_lib} EXPORT wip-comms-config
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib) 
  list(APPEND PROJECT_LIBRARY_LIST ${target_lib})
  set(PROJECT_LIBRARY_LIST ${PROJECT_LIBRARY_LIST} CACHE INTERNAL "project_library_list")
endmacro()

unset(PROJECT_LIBRARY_LIST CACHE)

add_subdirectory(src)

# configuration export - installed version
install(EXPORT wip-comms-config DESTINATION share/wip-comms/cmake)
# configuration export - local build version
export(TARGETS ${PROJECT_LIBRARY_LIST} FILE ${CMAKE_BINARY_DIR}/wip-comms-config.cmake)

