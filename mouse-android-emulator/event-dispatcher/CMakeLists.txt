project(event-dispatcher)

set(SRCS
	EventDispatcher.cpp

	# include/EventDispatcherApplication.h
	# include/EventDispatcherRemoteApplication.h
	#
	# src/EventDispatcherApplication.cpp
	# src/EventDispatcherRemoteApplication.cpp
)

# add_executable(${PROJECT_NAME} ${SRCS})
add_library(${PROJECT_NAME} SHARED ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE
	# PETS::core
	# network-core
	# tracy
)
