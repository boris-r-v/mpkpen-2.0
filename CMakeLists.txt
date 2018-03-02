project(mpkpen)
cmake_minimum_required(VERSION 3.5)

# set compile time options 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# set current version of project
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
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
set(ARM_RUNNER_SRC ${PROJECT_SOURCE_DIR}/run_src/mpkpen_arm.cpp)
set(KTS_RUNNER_SRC ${PROJECT_SOURCE_DIR}/run_src/mpkpen_kts.cpp)

# add public library
add_subdirectory(public_library)
include_directories ("${PROJECT_SOURCE_DIR}/public_library")
set(EXTRA_LIBS ${EXTRA_LIBS} shared)

# add target
add_executable(${PROJECT_NAME}-arm ${ARM_RUNNER_SRC})
target_link_libraries(${PROJECT_NAME}-arm ${EXTRA_LIBS} )

add_executable(${PROJECT_NAME}-kts ${KTS_RUNNER_SRC})
target_link_libraries(${PROJECT_NAME}-kts ${EXTRA_LIBS} )


