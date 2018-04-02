
set (Boost_USE_STATIC_LIBS ON) 
set (Protobuf_USE_STATIC_LIBS ON)

# need library
find_package(Boost REQUIRED COMPONENTS program_options system)
find_package(Protobuf REQUIRED)

# add lib source files
file(GLOB PUBLIC_LIB_SOURSEC ${PROJECT_SOURCE_DIR}/public_library/src/*.cpp)
file(GLOB PUBLIC_LIB_HEADER ${PROJECT_SOURCE_DIR}/public_library/src/*.h)
include_directories("${PROJECT_SOURCE_DIR}/public_library/src")
include_directories(${PROTOBUF_INCLUDE_DIRS})

# add protobuf generator
include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(ProtobufProtoSources 
    ${PROJECT_SOURCE_DIR}/in_proto/Message.proto
    ${PROJECT_SOURCE_DIR}/in_proto/Order.proto
    ${PROJECT_SOURCE_DIR}/in_proto/Ticket.proto
)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtobufProtoSources} )

# add include dir for the exutables
include_directories ("${PROJECT_SOURCE_DIR}/private_library/src")

# add static open-library
add_library(static_public STATIC ${PUBLIC_LIB_SOURSEC} ${PROTO_SRCS} ${PROTO_HDRS} )
set_target_properties(static_public PROPERTIES LINK_FLAGS "-static" )

target_link_libraries(static_public static_private ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} )

# set property    
set_target_properties(static_public PROPERTIES
			OUTPUT_NAME			${PROJECT_NAME}_public-${RELEASE_SERIES}
			VERSION				${VERSION}
			PUBLIC_HEADER			"${PUBLIC_LIB_HEADER}"
		    )




