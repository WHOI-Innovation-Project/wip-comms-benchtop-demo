protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${project_INC_DIR}/wip-comms/config
  gps_config.proto
  environmental_driver_config.proto
  liaison_config.proto
  geo_origin.proto
  classifier_config.proto 
  ifcb_database_config.proto
  )

add_library(wip_comms_config SHARED ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(wip_comms_config
  ${PROTOBUF_LIBRARIES} dccl goby_middleware wip_comms_messages
  cgsn_config
  )
project_install_lib(wip_comms_config)
