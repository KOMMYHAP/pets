project(top-down-view-shooter)

set(SRCS
	src/main.cpp
	src/Shader.cpp

	include/Shader.h
	include/stb_image.h
)

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include)
target_link_libraries(${PROJECT_NAME} PUBLIC
	glfw
	imgui-glfw-opengl3
	glad
	glm::glm_static
)
target_compile_options(${PROJECT_NAME} PUBLIC /permissive- /arch:AVX /std:c++17)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

set_property(TARGET top-down-view-shooter PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
