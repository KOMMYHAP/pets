project(protopackets)

set(SRCS
	packets-cpp/packets/Example.pb.cc
	packets-cpp/packets/Example.pb.h
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC packets-cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf)
