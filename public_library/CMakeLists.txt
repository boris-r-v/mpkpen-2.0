if(BUILD_STATIC)
    set (CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})		# -- !MOST IMPORTANT LINE! This line set archive linkage without shared library
    set (Boost_USE_STATIC_LIBS ON) 
    set (Boost_USE_STATIC_RUNTIME ON)
    set (Protobuf_USE_STATIC_LIBS ON)
    set (Boost_USE_MULTITHREADED OFF)
endif(BUILD_STATIC)

# need library
find_package(Boost COMPONENTS program_options system REQUIRED )
find_package(Protobuf REQUIRED)

# add lib source files
file(GLOB PUBLIC_LIB_SOURSEC ${PROJECT_SOURCE_DIR}/public_library/src/*.cpp)
file(GLOB PUBLIC_LIB_HEADER ${PROJECT_SOURCE_DIR}/public_library/src/*.h)
include_directories("${PROJECT_SOURCE_DIR}/public_library/src")
include_directories(${PROTOBUF_INCLUDE_DIRS})

# add protobuf generator
include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(ProtobufProtoSources 
#    ${PROJECT_SOURCE_DIR}/in_proto/Message.proto
    ${PROJECT_SOURCE_DIR}/in_proto/Order.proto
#    ${PROJECT_SOURCE_DIR}/in_proto/Ticket.proto
)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtobufProtoSources} )

# add include dir for the exutables
include_directories ("${PROJECT_SOURCE_DIR}/private_library/src")

# choose linkage type
if(BUILD_STATIC)
    add_library(public STATIC ${PUBLIC_LIB_SOURSEC} ${PROTO_SRCS} ${PROTO_HDRS} )
else(BUILD_STATIC)
    add_library(public SHARED ${PUBLIC_LIB_SOURSEC} ${PROTO_SRCS} ${PROTO_HDRS} )
endif(BUILD_STATIC)

#link 
target_link_libraries(public private ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES} )
# set property    
set_target_properties(public PROPERTIES
	    OUTPUT_NAME			${PROJECT_NAME}_public-${RELEASE_SERIES}
	    VERSION			${VERSION}
	    PUBLIC_HEADER		"${PUBLIC_LIB_HEADER}"
	    )


















#/usr/lib/x86_64-linux-gnu/libboost_program_options.a /usr/lib/x86_64-linux-gnu/libboost_system.a
#/usr/lib/x86_64-linux-gnu/libprotobuf.a  /usr/lib/x86_64-linux-gnu/libc.a /usr/lib/x86_64-linux-gnu/libpthread.a
