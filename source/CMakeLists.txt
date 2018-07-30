set(PHYSFSCPP_VERSION 0.1.0)
set(PHYSFSCPP_SOVERSION 1)

option(PHYSFS_BUILD_STATIC "Build static library" TRUE)
if(PHYSFS_BUILD_STATIC)
    add_library(physfscpp-static STATIC PhysFS/physfs.cpp ${PHYSFSCPP_HEADERS})
    if(NOT_MSVC)
        set_target_properties(physfscpp-static PROPERTIES OUTPUT_NAME "physfscpp")
    endif()
    set(PHYSFS_LIB_TARGET physfscpp-static )
    set(PHYSFS_INSTALL_TARGETS ${PHYSFS_INSTALL_TARGETS} ";physfscpp-static")
    target_compile_definitions(${PHYSFS_LIB_TARGET} PUBLIC -DPHYSFS_CPP_STATIC)
    target_link_libraries(${PHYSFS_LIB_TARGET} PUBLIC physfs-static)
endif()

option(PHYSFS_BUILD_SHARED "Build static library" TRUE)
if(PHYSFS_BUILD_SHARED)
    add_library(physfscpp SHARED PhysFS/physfs.cpp ${PHYSFSCPP_HEADERS})
    target_compile_definitions(physfscpp PUBLIC -DPHYSFS_EXPORT_LIB)
    set_target_properties(physfscpp PROPERTIES MACOSX_RPATH 1)
    set_target_properties(physfscpp PROPERTIES VERSION ${PHYSFSCPP_VERSION})
    set_target_properties(physfscpp PROPERTIES SOVERSION ${PHYSFSCPP_SOVERSION})
    target_link_libraries(${PHYSFS_LIB_TARGET} PUBLIC physfs)
    set(PHYSFS_LIB_TARGET physfscpp)
    set(PHYSFS_INSTALL_TARGETS ${PHYSFS_INSTALL_TARGETS} ";physfscpp")
endif()

if(NOT PHYSFS_BUILD_SHARED AND NOT PHYSFS_BUILD_STATIC)
    message(FATAL "Both shared and static libraries are disabled!")
endif()

target_include_directories(
        ${PHYSFS_LIB_TARGET}
        PUBLIC
        "$<BUILD_INTERFACE:${PHYSFSCPP_HEADER_DIR}>"
        "${PHYSFSCPP_HEADERS}"
)

if(PHYSFS_BUILD_SHARED AND PHYSFS_BUILD_STATIC AND NOT WINDOWS)
    set_target_properties(physfscpp PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    set_target_properties(physfscpp-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
endif()

install(TARGETS ${PHYSFS_INSTALL_TARGETS}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX})
install(FILES "${PHYSFSCPP_HEADERS}" DESTINATION include)

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