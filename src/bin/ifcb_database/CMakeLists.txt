find_library(PQXX_LIBRARY pqxx)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} config.proto)

add_executable(wip_comms_ifcb_database_interface main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(wip_comms_ifcb_database_interface goby_middleware wip_comms_config wip_comms_messages pqxx   ${PROGRESSIVE_IMAGERY_PATH}/build/libprogressive_imagery.so)

project_install_bin(wip_comms_ifcb_database_interface)
