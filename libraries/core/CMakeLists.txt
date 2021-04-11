project(core)

set(SRCS
	include/Callback.h
	include/TemplateUtils.h
	include/Utils.h
	src/Utils.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
add_library(PETS::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)
