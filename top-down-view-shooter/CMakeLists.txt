project(top-down-view-shooter)

set(SRCS
	src/imgui_impl_glfw.cpp
	src/imgui_impl_opengl3.cpp
	src/main.cpp
	src/Shader.cpp

	include/imgui_impl_glfw.h
	include/imgui_impl_opengl3.h
	include/Shader.h
)

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw imgui glad)
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

set_property(TARGET top-down-view-shooter PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
