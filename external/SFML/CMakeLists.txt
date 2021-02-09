if (NOT SFML_DIR)
	set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake/SFML CACHE STRING "SFML directory" FORCE)
endif()
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML COMPONENTS REQUIRED
	system
	window
	graphics
	network
	# audio
	# main
)

add_library(SFML INTERFACE)
target_link_libraries(SFML INTERFACE
	sfml-system
	sfml-window
	sfml-graphics
	sfml-network
	# sfml-audio
	# sfml-main
)

set(SFML_DIR ${SFML_DIR} PARENT_SCOPE)
