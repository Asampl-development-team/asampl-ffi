cmake_minimum_required(VERSION 3.0.0)

project(asampl-ffi)

file(GLOB SRCS src/*.c)

add_library(asampl-ffi STATIC ${SRCS})
target_include_directories(asampl-ffi PUBLIC include)

install(TARGETS asampl-ffi LIBRARY)
