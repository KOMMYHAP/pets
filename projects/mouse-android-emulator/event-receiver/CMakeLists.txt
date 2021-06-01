cmake_minimum_required(VERSION 3.19)

include(../mouse-emulator.cmake)

project(event-receiver)

set(SRCS
	include/EventReceiverApplication.h
	include/EventReceiverRemoteApplication.h

	src/EventReceiverApplication.cpp
	src/EventReceiverRemoteApplication.cpp

	EventReceiver.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PETS::core
		network-core
		tracy
)
