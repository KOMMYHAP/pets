project(mouse-handling)

set(SRCS
  main.cpp
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${IMGUI_DEMO_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /arch:AVX /std:c++17)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
