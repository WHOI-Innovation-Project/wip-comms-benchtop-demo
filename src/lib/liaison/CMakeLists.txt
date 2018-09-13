add_library(wip_comms_liaison SHARED
  liaison_imagery.cpp
  liaison_load.cpp
  )

find_path(Wt_INCLUDE_DIR Wt/WObject)
find_library(Wt_LIBRARY NAMES wt DOC "The Wt C++ Web library")

target_link_libraries(wip_comms_liaison wip_comms_messages goby_middleware ${Wt_LIBRARY}
  wip_comms_config
  ${PROGRESSIVE_IMAGERY_PATH}/build/libprogressive_imagery.so
  goby_moos
  )

project_install_lib(wip_comms_liaison)
