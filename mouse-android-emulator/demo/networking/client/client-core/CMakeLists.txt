project(client-core)

set(SRCS
	include/RemoteApplicationBase.h
	include/RemoteApplicationBridge.h

	src/RemoteApplicationBase.cpp
	src/RemoteApplicationBridge.cpp
)

add_library(${PROJECT_NAME} ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		PETS::core
		network-core
)
