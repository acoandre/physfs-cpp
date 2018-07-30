cmake_minimum_required(VERSION 3.9.0)
project(PhysFS++)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
set( PHYSFSCPP_HEADER_DIR "${CMAKE_CURRENT_LIST_DIR}/Include/PhysFS")
set( PHYSFSCPP_HEADERS    "${CMAKE_CURRENT_LIST_DIR}/Include/PhysFS/PhysFS.hpp" )

add_subdirectory(Source)
#add_subdirectory(test)