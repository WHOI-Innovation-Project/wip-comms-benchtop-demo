add_library(wip_comms_liaison SHARED
  liaison_imagery.cpp
  liaison_load.cpp
  )

target_link_libraries(wip_comms_liaison wip_comms_messages goby_middleware)

project_install_lib(wip_comms_liaison)
