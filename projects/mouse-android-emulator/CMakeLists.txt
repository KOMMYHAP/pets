project(mouse-android-emulator)

set(PETS_REQUIRE_TRACY ON)
set(PETS_REQUIRE_PROTOBUF ON)

add_subdirectory(network)

if (ANDROID_PLATFORM)
	add_subdirectory(event-dispatcher)
else()
	add_subdirectory(event-receiver)
endif()
