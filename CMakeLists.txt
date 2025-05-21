cmake_minimum_required(VERSION 3.5)
project(muduochat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cc
    src/chatserver.cc
    src/chatservice.cc
    src/db.cc
    src/usermodel.cc
)

add_executable(muduochat ${SOURCES})

target_link_libraries(muduochat
  muduo_net
  muduo_base
  pthread
  mysqlclient
)