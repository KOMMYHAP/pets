project(network-core)

set(SRCS
	include/Peer.h
	include/PeerConnection.h
	include/PackageManager.h
	include/ReceivedPacket.h
	include/ProtoPackagesNamespace.h
	src/Peer.cpp
	src/PeerConnection.cpp
	src/PackageManager.cpp
	src/ReceivedPacket.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_link_libraries(${PROJECT_NAME} PUBLIC PETS::core SFML protopackets)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
