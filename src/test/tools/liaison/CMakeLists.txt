protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} config.proto)

add_executable(wip_comms_liaison_test main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(wip_comms_liaison_test goby_middleware wip_comms_messages)
