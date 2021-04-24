cmake_minimum_required(VERSION 3.9)

project(pets)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT IPO_SUPPORTED_CACHED)
	include(CheckIPOSupported) # too long check, so cache its result
	check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_CHECK_ERROR LANGUAGES CXX)
	if (NOT IPO_SUPPORTED)
		message(STATUS "IPO/LTE optimization is not supported: ${IPO_CHECK_ERROR}.")
	endif()
	set(IPO_SUPPORTED_CACHED ${IPO_SUPPORTED} CACHE BOOL "Cached result of check ipo supported" FORCE)
endif()

if (IPO_SUPPORTED_CACHED)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	message(STATUS "IPO/LTE is turned on.")
endif()

add_subdirectory(external)
add_subdirectory(libraries)

add_subdirectory(top-down-view-shooter)
add_subdirectory(mouse-android-emulator)
