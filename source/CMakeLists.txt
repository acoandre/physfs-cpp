add_library(physfs++ PhysFS/physfs.cpp ${PHYSFSCPP_HEADERS})
target_include_directories(
        physfs++
        PUBLIC
        "$<BUILD_INTERFACE:${PHYSFSCPP_HEADERS}>"
        "${PHYSFSCPP_HEADERS}"
)

if (BUILD_SHARED_LIBS)
    if (UNIX)
        #target_compile_options(physfs++ PRIVATE "-fvisibility=hidden")
    endif()

    target_compile_definitions(physfs++ PUBLIC -DPHYSFS_EXPORT_LIB)
    target_link_libraries(physfs++ PUBLIC physfs)
else()
    target_compile_definitions(physfs++ PUBLIC -DPHYSFS_CPP_STATIC)
    target_link_libraries(physfs++ PUBLIC physfs-static)
endif()
