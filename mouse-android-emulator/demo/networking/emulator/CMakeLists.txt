project(emulator)

set(SRCS
	include/Emulator.h
	include/EmulatorDelegate.h

	src/Emulator.cpp
	src/EmulatorDelegate.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		PETS::core
	PRIVATE
		network-api
)
