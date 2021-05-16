project(protopackets)

add_subdirectory(cpp-protocol)

add_library(${PROJECT_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/cpp-protocol
	${PROTOBUF_DIR}/include
)

find_library(libprotobuf_LIBRARY_RELEASE libprotobuf ${PROTOBUF_LIB_DIR}/Release)
find_library(libprotobuf_LIBRARY_DEBUG libprotobufd ${PROTOBUF_LIB_DIR}/Debug)
set(libprotobuf_LIBRARY
	debug ${libprotobuf_LIBRARY_DEBUG}
	optimized ${libprotobuf_LIBRARY_RELEASE}
)
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		tracy
		${libprotobuf_LIBRARY}
)
