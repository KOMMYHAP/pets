project(network-listener)

set(SRCS
	Listener.cpp

	include/EventDispatcherApplication.h
	include/EventDispatcherRemoteApplication.h

	src/EventDispatcherApplication.cpp
	src/EventDispatcherRemoteApplication.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PETS::core
		client-core
		network-core
)
