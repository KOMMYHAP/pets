project(event-receiver)

set(SRCS
	include/EventReceiverApplication.h
	include/EventReceiverRemoteApplication.h

	src/EventReceiverApplication.cpp
	src/EventReceiverRemoteApplication.cpp

	EventReceiver.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
#add_executable(${PROJECT_NAME} WIN32 ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PETS::core
		network-core
		tracy
)
