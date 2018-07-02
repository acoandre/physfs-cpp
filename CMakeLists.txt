cmake_minimum_required(VERSION 3.9.0)
project(PhysFS++)
enable_testing()

set( PHYSFSCPP_HEADERS "${CMAKE_CURRENT_LIST_DIR}/Include/PhysFS" )
add_subdirectory(Source)
#add_subdirectory(test)