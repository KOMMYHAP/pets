cmake_minimum_required(VERSION 3.9)
project(event-dispatcher-lib)

set(SRCS
	# EventDispatcher.cpp

	include/ApplicationInputInterface.h
	include/ApplicationOutputInterface.h
	include/EventDispatcherApplication.h
	include/EventDispatcherRemoteApplication.h
	# include/NativeJniBridge.h

	src/EventDispatcherApplication.cpp
	src/EventDispatcherRemoteApplication.cpp
	# src/NativeJniBridge.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})

# if (WIN32)
# 	set(SRCS ${SRCS}
# 		src/JniStub.cpp
# 	)
	# add_executable(event-dispatcher)
# else()
# 	message(FATAL_ERROR "Unsupported platform.")
# endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC
	PETS::core
	network-core
	tracy
	jni
)
