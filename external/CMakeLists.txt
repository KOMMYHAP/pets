set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

# OpenGL
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)

# SFML
add_subdirectory(SFML)

# Dear ImGui
if (NOT IMGUI_DIR)
	set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui CACHE STRING "Dear Imgui directory")
endif()

# Dear ImGui (SFML)
set(IMGUI_SFML_IMGUI_DEMO OFF)
add_subdirectory(imgui-sfml)

add_subdirectory(protobuf)
add_subdirectory(json)
add_subdirectory(tracy)
