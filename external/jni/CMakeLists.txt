if (NOT ANDROID_PLATFORM)
	return()
endif()

project(jni)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
