cmake_minimum_required(VERSION 3.0.0)

project(morpheus)

include_directories(include)

set(MORPH_SOURCES source/ValueType.cpp)

add_library(morph STATIC ${MORPH_SOURCES})