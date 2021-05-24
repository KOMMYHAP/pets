project(protopackets)

add_subdirectory(cpp-protocol)

add_library(${PROJECT_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/cpp-protocol
	${PROTOBUF_DIR}/include
)

if (WIN32)
	find_library(libprotobuf_LIBRARY_RELEASE libprotobuf
		PATHS ${PROTOBUF_LIB_DIR}
		REQUIRED
	)
	find_library(libprotobuf_LIBRARY_DEBUG libprotobufd
		PATHS ${PROTOBUF_LIB_DIR}
		REQUIRED
	)

	set(libprotobuf_LIBRARY
		debug ${libprotobuf_LIBRARY_DEBUG}
		optimized ${libprotobuf_LIBRARY_RELEASE}
	)
else()
	find_library(libprotobuf_LIBRARY protobuf
		PATHS ${PROTOBUF_LIB_DIR}
		REQUIRED
		NO_CMAKE_FIND_ROOT_PATH)
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE ${libprotobuf_LIBRARY}
)
