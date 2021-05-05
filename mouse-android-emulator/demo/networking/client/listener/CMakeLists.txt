project(network-listener)

set(SRCS
	Listener.cpp

	include/ListenerApplication.h
	include/RemoteApplication.h
	include/RemoteApplicationBridge.h

	src/ListenerApplication.cpp
	src/RemoteApplication.cpp
	src/RemoteApplicationBridge.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PETS::core
		network-api
)
