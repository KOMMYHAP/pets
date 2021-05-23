set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

option(PETS_REQUIRE_OPENGL OFF)
if (PETS_REQUIRE_OPENGL)
	add_subdirectory(glad)
	add_subdirectory(glfw)
	add_subdirectory(glm)
endif()

option(PETS_REQUIRE_SFML OFF)
if (PETS_REQUIRE_SFML)
	add_subdirectory(SFML)
endif()

option(PETS_REQUIRE_SFML OFF)
if (PETS_REQUIRE_SFML)
	set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui CACHE STRING "Dear Imgui directory")

	if (PETS_REQUIRE_SFML)
		set(IMGUI_SFML_IMGUI_DEMO OFF)
		add_subdirectory(imgui-sfml)
	endif()
endif()

option(PETS_REQUIRE_PROTOBUF OFF)
if (PETS_REQUIRE_PROTOBUF)
	add_subdirectory(protobuf)
endif()

option(PETS_REQUIRE_JSON OFF)
if (PETS_REQUIRE_JSON)
	add_subdirectory(json)
endif()

option(PETS_REQUIRE_TRACY OFF)
if (PETS_REQUIRE_TRACY)
	add_subdirectory(tracy)
endif()
