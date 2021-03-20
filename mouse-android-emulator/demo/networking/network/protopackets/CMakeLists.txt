project(protopackets)

# 1. generate CMakeLists.txt with all generated files
add_custom_command(
	OUTPUT test.txt
	COMMAND python 
	ARGS
		compile.py
		"--protobuf-root" "${PROTOBUF_DIR}"
		"--protocol-root" "${CMAKE_CURRENT_SOURCE_DIR}/protocol"
		"--cpp-out" "${CMAKE_CURRENT_SOURCE_DIR}/cpp-protocol"
		"--java-out" "${CMAKE_CURRENT_SOURCE_DIR}/java-protocol"
	DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/protocol/Example.proto
	COMMENT "Discovered changes in .proto files: regenerating protocol cpp and java files..."
	VERBATIM
)

# 2. extract sources into SRCS
add_subdirectory(cpp-protocol)

# 3. create library
add_library(${PROJECT_NAME} STATIC ${SRCS})

# 4. add include directories
target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/cpp-protocol
	${PROTOBUF_DIR}/include
)

# 5. link with protobuf libraries
find_library(libprotobuf_LIBRARY_RELEASE libprotobuf ${PROTOBUF_LIB_DIR}/Release)
find_library(libprotobuf_LIBRARY_DEBUG libprotobufd ${PROTOBUF_LIB_DIR}/Debug)
set(libprotobuf_LIBRARY
	debug ${libprotobuf_LIBRARY_DEBUG}
	optimized ${libprotobuf_LIBRARY_RELEASE}
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${libprotobuf_LIBRARY})
