cmake_minimum_required(VERSION 3.9)
project(HackFM)

set (CMAKE_CXX_FLAGS "-O2")

include_directories(HackFM "src/")
include_directories(HackFM "C:/Program Files/PothosSDR/include/libhackrf/")

file(GLOB SRC "src/*.cpp")

link_directories(HackFM "C:/Program Files/PothosSDR/lib/")

link_libraries(hackrf)

#link_libraries(pthread)

add_executable(HackFM ${SRC})