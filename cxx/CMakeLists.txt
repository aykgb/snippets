#.1 cmake最低版本要求
cmake_minimum_required(VERSION 2.8.6)

#.2 工程名
project(cc_snippets_test)

#.3 设置编译变量和常量
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

#.4 设置第三方依赖头文件/库
set(INCLUDE_DIRECTORIES
    /usr/include
    /usr/local/include
    ${PROJECT_SOURCE_DIR}/proto
    ${PROJECT_SOURCE_DIR}/../third_party/rocksdb/include)
set(LINK_DIRECTORIES
  /usr/lib
  /usr/local/lib
  ${PROJECT_SOURCE_DIR}/../third_party/rocksdb)
include_directories(${INCLUDE_DIRECTORIES})
link_directories(${LINK_DIRECTORIES})

#.4.1 添加子目录（有独立CMakeLists.txt 文件，并编译为链接库）

#.5 扫描源文件
aux_source_directory("${PROJECT_SOURCE_DIR}" SOURCES)
aux_source_directory("${PROJECT_SOURCE_DIR}/proto" PROTO_SOURCES)

#.6 添加可执行文件
add_executable(cc_snippets_test ${SOURCES} ${PROTO_SOURCES})

#.7 链接第三方库或子目录编译的链接的链接库
target_link_libraries(cc_snippets_test
          glog gtest gflags protobuf
          rocksdb lz4 zstd snappy bz2 z
          brpc
          pthread)
