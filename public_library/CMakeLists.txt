
# add lib source files
file(GLOB PUBLIC_LIB_SOURSEC ${PROJECT_SOURCE_DIR}/public_library/*.cpp)
file(GLOB PUBLIC_LIB_HEADER ${PROJECT_SOURCE_DIR}/public_library/*.h)
include_directories ("${PROJECT_SOURCE_DIR}/public_library")

# add open-library
add_library(shared SHARED ${PUBLIC_LIB_SOURSEC} )
# set property    
set_target_properties(shared PROPERTIES
			OUTPUT_NAME			${RELEASE}
			VERSION				${VERSION}
			PUBLIC_HEADER			"${PUBLIC_LIB_HEADER}")



