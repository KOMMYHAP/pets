project(network-writer)

set(SRCS
  Writer.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /arch:AVX /std:c++17)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML SFML protopackets)
