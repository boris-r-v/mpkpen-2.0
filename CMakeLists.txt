project(mpkpen)
cmake_minimum_required(VERSION 3.5)

option(BUILD_STATIC "Build the static runner" ON)
message(${CMAKE_SYSTEM_PROCESSOR})
# set compile time options 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g ")


if(BUILD_STATIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
endif(BUILD_STATIC)

# set current version of project
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 1)
set(RELEASE_SERIES 2.0)
set(RELEASE ${PROJECT_NAME}-${RELEASE_SERIES})
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(RELEASE_VERSION ${RELEASE_NAME}-v.${VERSION})

# add the curretn version number into project
configure_file(
    "${PROJECT_SOURCE_DIR}/in_src/version.h.in"
    "${PROJECT_BINARY_DIR}/version.h"
    )
# add the binary tree to the search path for include files
include_directories(${PROJECT_BINARY_DIR})

# add runner src file
set(RUNNER_SRC ${PROJECT_SOURCE_DIR}/run_src/mpkpen.cpp)

# add private library
add_subdirectory(private_library)

# add public library
add_subdirectory(public_library)

# add include dir for the exutables
include_directories ("${PROJECT_SOURCE_DIR}/public_library/src")

# add target
add_executable(${PROJECT_NAME} ${RUNNER_SRC})
target_link_libraries(${PROJECT_NAME} public )

