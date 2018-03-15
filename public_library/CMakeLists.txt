
# need library
find_package(Boost REQUIRED COMPONENTS program_options )

# add lib source files
file(GLOB PUBLIC_LIB_SOURSEC ${PROJECT_SOURCE_DIR}/public_library/src/*.cpp)
file(GLOB PUBLIC_LIB_HEADER ${PROJECT_SOURCE_DIR}/public_library/src/*.h)
include_directories ("${PROJECT_SOURCE_DIR}/public_library/src")

# add open-library
add_library(shared_public SHARED ${PUBLIC_LIB_SOURSEC} )
target_link_libraries(shared_public shared_private ${Boost_LIBRARIES})

# set property    
set_target_properties(shared_public PROPERTIES
			OUTPUT_NAME			${PROJECT_NAME}_public-${RELEASE_SERIES}
			VERSION				${VERSION}
			PUBLIC_HEADER			"${PUBLIC_LIB_HEADER}")



