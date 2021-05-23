project(network-core)

set(SRCS
	include/NetworkErrorConstants.h
	include/NetworkInterface.h
	include/PackageManager.h
	include/PacketHandlerOperation.h
	include/Peer.h
	include/PeerConnection.h
	include/ProtoPackagesNamespace.h
	include/ReceivedPacket.h

	src/NetworkInterface.cpp
	src/PackageManager.cpp
	src/PacketHandlerOperation.cpp
	src/Peer.cpp
	src/PeerConnection.cpp
	src/ReceivedPacket.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		PETS::core
		SFML
		protopackets
		tracy
)
