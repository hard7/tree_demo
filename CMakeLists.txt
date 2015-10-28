cmake_minimum_required(VERSION 3.3)
project(tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp) #tree_search.h rb_tree_search.h)
add_executable(tree ${SOURCE_FILES})