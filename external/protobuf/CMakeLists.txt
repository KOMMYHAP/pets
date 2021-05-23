if (NOT PROTOBUF_DIR)
	set(PROTOBUF_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Protobuf common directory" FORCE)
	set(PROTOBUF_COMPILIER_DIR ${PROTOBUF_DIR}/protoc CACHE STRING "Protobuf compiler directory" FORCE)

	if (WIN32)
		set(PROTOBUF_LIB_DIR ${PROTOBUF_DIR}/lib/windows CACHE STRING "Protobuf lLibraries directory" FORCE)
	elseif (ANDROID_PLATFORM)
		set(PROTOBUF_LIB_DIR ${PROTOBUF_DIR}/lib/android CACHE STRING "Protobuf lLibraries directory" FORCE)
	else()
		message(FATAL_ERROR "This platform is not supported.")
	endif()

endif()
