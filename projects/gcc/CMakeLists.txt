cmake_minimum_required(VERSION 3.5)
project (playground)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/../../include")

file(GLOB SOURCES "src/*.cpp")
add_executable(playground ${SOURCES})

