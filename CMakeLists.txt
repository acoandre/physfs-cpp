cmake_minimum_required(VERSION 3.9.0)

project(PhysFS++)

set(PHYSFSCPP_VERSION 0.1.0)
set(PHYSFSCPP_SOVERSION 1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

find_package(PhysFS REQUIRED)


# Don't use -rpath.
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
endif()

option(PHYSFS_BUILD_STATIC "Build static library" TRUE)
if(PHYSFS_BUILD_STATIC)
    # static libraries use a different target
    set(PHYSFS_LIB_TARGET physfscpp-static )
    add_library(${PHYSFS_LIB_TARGET} STATIC
		source/PhysFS/PhysFS.cpp
		include/PhysFS/PhysFS.hpp
	)
    target_compile_definitions(${PHYSFS_LIB_TARGET} PRIVATE PHYSFS_CPP_STATIC)
    if(NOT MSVC)
        set_target_properties(${PHYSFS_LIB_TARGET} PROPERTIES OUTPUT_NAME "physfscpp")
    endif()

    # where to store all these lovely build artefacts
    set(PHYSFS_INSTALL_TARGETS ${PHYSFS_INSTALL_TARGETS} ";${PHYSFS_LIB_TARGET}")

    # assume we want to ink against the static version of physfs
    target_link_libraries(${PHYSFS_LIB_TARGET} PUBLIC physfs-static)
endif()

option(PHYSFS_BUILD_SHARED "Build shared library" FALSE)
if(PHYSFS_BUILD_SHARED)
    set(PHYSFS_LIB_TARGET physfscpp )
    add_library(${PHYSFS_LIB_TARGET} SHARED
		source/PhysFS/PhysFS.cpp
		include/PhysFS/PhysFS.hpp
	)
    target_compile_definitions(${PHYSFS_LIB_TARGET} PRIVATE PHYSFS_CPP_EXPORT)
    set_target_properties(${PHYSFS_LIB_TARGET} PROPERTIES MACOSX_RPATH 1)
    set_target_properties(${PHYSFS_LIB_TARGET} PROPERTIES VERSION ${PHYSFSCPP_VERSION})
    set_target_properties(${PHYSFS_LIB_TARGET} PROPERTIES SOVERSION ${PHYSFSCPP_SOVERSION})

    # where to store all these lovely build artefacts
    set(PHYSFS_INSTALL_TARGETS ${PHYSFS_INSTALL_TARGETS} ";physfscpp")

    # assume we want to link against the shared version of physfs
    target_link_libraries(${PHYSFS_LIB_TARGET} PUBLIC physfs)
endif()


if(NOT PHYSFS_BUILD_SHARED AND NOT PHYSFS_BUILD_STATIC)
    message(FATAL "Both shared and static libraries are disabled!")
endif()

target_include_directories(${PHYSFS_LIB_TARGET}
	PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(PHYSFS_BUILD_SHARED AND PHYSFS_BUILD_STATIC AND NOT WINDOWS)
    set_target_properties(physfscpp PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    set_target_properties(physfscpp-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
endif()

install(TARGETS ${PHYSFS_INSTALL_TARGETS} EXPORT PhysFScpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
		INCLUDES DESTINATION include)
install(DIRECTORY ./include/ DESTINATION include)

install(EXPORT PhysFScpp
		NAMESPACE physfscpp::
        DESTINATION "lib${LIB_SUFFIX}/cmake/physfscpp"
        FILE physfscpp-config.cmake
)

macro(message_bool_option _NAME _VALUE)
    if(${_VALUE})
        message(STATUS "  ${_NAME}: enabled")
    else()
        message(STATUS "  ${_NAME}: disabled")
    endif()
endmacro()

message(STATUS "PhysicsFS++ will build with the following options:")
message_bool_option("Build static library" PHYSFS_BUILD_STATIC)
message_bool_option("Build shared library" PHYSFS_BUILD_SHARED)


#add_subdirectory(test)