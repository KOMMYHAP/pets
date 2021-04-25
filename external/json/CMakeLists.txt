project(jsoncpp)

add_library(jsoncpp STATIC
	lib_json/json_reader.cpp
	lib_json/json_tool.h
	lib_json/json_value.cpp
	lib_json/json_valueiterator.inl
	lib_json/json_writer.cpp
)
target_include_directories(jsoncpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
