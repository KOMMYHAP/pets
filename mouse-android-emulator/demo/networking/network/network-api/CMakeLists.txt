project(network-api)

set(SRCS
	include/NetworkInterface.h

	src/NetworkInterface.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PETS::core
		protopackets
		network-core
		jsoncpp
)
