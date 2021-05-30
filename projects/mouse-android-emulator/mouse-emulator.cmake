cmake_minimum_required(VERSION 3.9)

set(PETS_REQUIRE_PROTOBUF ON CACHE BOOL "Require protobuf library")
set(PETS_REQUIRE_TRACY ON CACHE BOOL "Require tracy library")
if (ANDROID_PLATFORM)
	set(PETS_REQUIRE_JNI ON CACHE BOOL "Require jni library for android")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../pets.cmake)

if (ANDROID_PLATFORM)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/event-dispatcher event-dispatcher-lib)
else()
	message(FATAL_ERROR "Unsupported platform")
endif()
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/network network)
