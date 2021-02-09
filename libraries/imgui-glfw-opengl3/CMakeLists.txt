# dear imgui
set(IMGUI_SOURCE_FILES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

set(IMGUI_HEADER_FILES
  ${IMGUI_DIR}/imconfig.h
  ${IMGUI_DIR}/imgui.h
  ${IMGUI_DIR}/imgui_internal.h
  ${IMGUI_DIR}/imstb_rectpack.h
  ${IMGUI_DIR}/imstb_textedit.h
  ${IMGUI_DIR}/imstb_truetype.h)

add_library(imgui-glfw-opengl3 STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
target_link_libraries(imgui-glfw-opengl3 PUBLIC glfw glad)
target_include_directories(imgui-glfw-opengl3 PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_compile_definitions(imgui-glfw-opengl3 PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
