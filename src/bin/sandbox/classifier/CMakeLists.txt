add_executable(wip_comms_classifier main.cpp)
target_link_libraries(wip_comms_classifier goby_middleware
  wip_comms_config
  wip_comms_messages
  ${PROGRESSIVE_IMAGERY_PATH}/build/libprogressive_imagery.so
  )
  
project_install_bin(wip_comms_classifier)
