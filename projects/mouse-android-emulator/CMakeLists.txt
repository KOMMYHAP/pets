project(mouse-android-emulator)

set(PETS_REQUIRE_TRACY ON)
set(PETS_REQUIRE_PROTOBUF ON)

add_subdirectory(network)
add_subdirectory(event-dispatcher)
add_subdirectory(event-receiver)
