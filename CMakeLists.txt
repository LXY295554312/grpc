cmake_minimum_required(VERSION 3.10)
project(grpc_calculator)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Protobuf和gRPC包
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 打印找到的版本信息
message(STATUS "Using Protobuf: ${Protobuf_VERSION}")
message(STATUS "Using gRPC: ${gRPC_VERSION}")

# 定义目录路径
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# 指定.proto文件
set(PROTO_FILE ${PROTO_DIR}/calculator.proto)

# 生成Protobuf和gRPC的C++代码
get_filename_component(PROTO_FILE_NAME ${PROTO_FILE} NAME_WE)

# 生成的Protobuf文件
set(PROTO_HDRS ${BUILD_DIR}/${PROTO_FILE_NAME}.pb.h)
set(PROTO_SRCS ${BUILD_DIR}/${PROTO_FILE_NAME}.pb.cc)

# 生成的gRPC文件
set(GRPC_HDRS ${BUILD_DIR}/${PROTO_FILE_NAME}.grpc.pb.h)
set(GRPC_SRCS ${BUILD_DIR}/${PROTO_FILE_NAME}.grpc.pb.cc)

# 添加自定义命令生成代码
add_custom_command(
  OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
  COMMAND protobuf::protoc
  ARGS --grpc_out=${BUILD_DIR}
       --cpp_out=${BUILD_DIR}
       -I${PROTO_DIR}
       --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
       ${PROTO_FILE}
  DEPENDS ${PROTO_FILE}
  COMMENT "生成gRPC和Protobuf代码"
)

# 创建一个库来包含生成的代码
add_library(grpc_proto_lib
  ${PROTO_SRCS} ${PROTO_HDRS}
  ${GRPC_SRCS} ${GRPC_HDRS}
)

# 链接库
target_link_libraries(grpc_proto_lib
  gRPC::grpc++ gRPC::grpc++_reflection
  protobuf::libprotobuf
)

# 包含头文件目录
target_include_directories(grpc_proto_lib PUBLIC ${BUILD_DIR})

# 添加服务器可执行文件
add_executable(calculator_server
  ${SRC_DIR}/calculator_server.cc
)

# 添加客户端可执行文件
add_executable(calculator_client
  ${SRC_DIR}/calculator_client.cc
)

# 链接可执行文件
target_link_libraries(calculator_server
  grpc_proto_lib
  gRPC::grpc++
  protobuf::libprotobuf
)

target_link_libraries(calculator_client
  grpc_proto_lib
  gRPC::grpc++
  protobuf::libprotobuf
)

# 让客户端和服务器能找到生成的头文件
target_include_directories(calculator_server PUBLIC ${BUILD_DIR})
target_include_directories(calculator_client PUBLIC ${BUILD_DIR})
    