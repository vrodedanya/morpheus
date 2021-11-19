cmake_minimum_required(VERSION 3.0.0)

project(morpheus)

include_directories(include)

add_compile_options(-Wall -Wextra)

set(MORPH_SOURCES source/Loader.cpp include/morpheus/HasDataType.hpp)

add_library(morpheus STATIC ${MORPH_SOURCES})
