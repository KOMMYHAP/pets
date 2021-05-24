project(mouse-android-emulator)

add_subdirectory(network)

if (ANDROID_PLATFORM)
	add_subdirectory(event-dispatcher)
else()
	add_subdirectory(event-receiver)
endif()
