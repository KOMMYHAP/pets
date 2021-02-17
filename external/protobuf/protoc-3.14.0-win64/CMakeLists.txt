if (NOT PROTOBUF_DIR)
	set(PROTOBUF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protoc-3.14.0-win64/bin/ CACHE STRING "Proto Compiler directory" FORCE)
endif()

project(protobuf)
add_library(protobuf INTERFACE)
target_include_directories(protobuf INTERFACE include)
