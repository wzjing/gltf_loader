cmake_minimum_required(VERSION 3.10)
project(gltf_loader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -D CMAKE_C_COMPILER=Clang++")

set(SOURCE_FILES main.cpp gltf_loader.h gltf_loader.cpp lib/cJson/cJSON.c lib/cJson/cJSON.h)
add_executable(gltf_loader ${SOURCE_FILES})