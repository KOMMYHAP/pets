cmake_minimum_required(VERSION 3.16)

include(../../../../../../mouse-emulator.cmake)

project(event-dispatcher)

add_library(${PROJECT_NAME} SHARED
	include/JavaToNativeBridge.h
	include/NativeJniBridge.h
	include/NativeToJavaBridge.h
	src/JavaToNativeBridge.cpp
	src/JniImpl.cpp
	src/NativeJniBridge.cpp
	src/NativeToJavaBridge.cpp
)

find_library(log-lib log REQUIRED)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${log-lib}
	PUBLIC
		event-dispatcher-lib
)

target_compile_options(${PROJECT_NAME} PUBLIC "-fPIC")
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} )
