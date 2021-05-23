if (PETS_REQUIRE_IMGUI AND PETS_REQUIRE_OPENGL)
	add_subdirectory(imgui-glfw-opengl3)
endif()

add_subdirectory(core)
