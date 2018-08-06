protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${project_INC_DIR}/wip-comms/messages
  gps.proto
  status.proto
  )

add_library(wip_comms_messages SHARED ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(wip_comms_messages ${PROTOBUF_LIBRARIES} dccl)
project_install_lib(wip_comms_messages)
