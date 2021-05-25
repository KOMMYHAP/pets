project(event-dispatcher)

set(SRCS
	EventDispatcher.cpp

	include/EventDispatcherApplication.h
	include/EventDispatcherRemoteApplication.h
	include/NativeJniBridge.h

	src/EventDispatcherApplication.cpp
	src/EventDispatcherRemoteApplication.cpp
	src/NativeJniBridge.cpp
)

if (ANDROID_PLATFORM)
	set(SRCS ${SRCS}
		src/JniImpl.cpp
	)
	add_library(${PROJECT_NAME} SHARED ${SRCS})
elseif (WIN32)
	set(SRCS ${SRCS}
		src/JniStub.cpp
	)
	add_executable(${PROJECT_NAME} ${SRCS})
else()
	message(FATAL_ERROR "Unsupported platform.")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE
	PETS::core
	network-core
	tracy
)
