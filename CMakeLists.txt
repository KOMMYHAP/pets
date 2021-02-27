cmake_minimum_required(VERSION 3.8)

project(pets)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external)
add_subdirectory(libraries)

add_subdirectory(top-down-view-shooter)
add_subdirectory(mouse-android-emulator)
