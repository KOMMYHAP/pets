# add_subdirectory(protoc-3.14.0-win64)

if (NOT PROTOBUF_DIR)
	set(PROTOBUF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protoc-3.14.0-win64 CACHE STRING "Protobuf common directory" FORCE)
	set(PROTOBUF_LIB_DIR ${PROTOBUF_DIR}/lib CACHE STRING "Protobuf lLibraries directory" FORCE)
endif()
