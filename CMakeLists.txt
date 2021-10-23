cmake_minimum_required(VERSION 3.0.0)

project(morpheus)

include_directories(include)

set(SOURCE source/main.cpp source/ValueType.cpp)

add_executable(morph ${SOURCE})