add_executable(wip_comms_gps_driver main.cpp)
target_link_libraries(wip_comms_gps_driver goby_middleware
  wip_comms_config
  wip_comms_messages
  cgsn_messages
  )
  
project_install_bin(wip_comms_gps_driver)
