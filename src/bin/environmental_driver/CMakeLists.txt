add_executable(wip_comms_environmental_driver main.cpp)
target_link_libraries(wip_comms_environmental_driver goby_middleware
  wip_comms_config
  wip_comms_messages
  )
  
project_install_bin(wip_comms_environmental_driver)
