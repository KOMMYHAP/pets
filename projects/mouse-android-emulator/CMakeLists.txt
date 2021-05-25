project(mouse-android-emulator)

add_subdirectory(network)

if (WIN32)
	add_subdirectory(event-receiver)
endif()

add_subdirectory(event-dispatcher)
