cmake_minimum_required(VERSION 3.8)

project(pets)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/imgui)
add_subdirectory(top-down-view-shooter)

add_compile_options(-msse2)
