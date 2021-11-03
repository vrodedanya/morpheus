cmake_minimum_required(VERSION 3.0.0)

project(morpheusLibrary)

include_directories(include)

set(MORPH_SOURCES source/HasDataType.cpp)

add_library(morpheus STATIC ${MORPH_SOURCES})