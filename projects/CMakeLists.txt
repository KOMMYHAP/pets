option(PETS_PROJECT_TOP_DOWN_VIEW_SHOOTER OFF)
if (PETS_PROJECT_TOP_DOWN_VIEW_SHOOTER)
	add_subdirectory(top-down-view-shooter)
endif()

option(PETS_PROJECT_MOUSE_ANDROID_EMULATOR OFF)
if (PETS_PROJECT_MOUSE_ANDROID_EMULATOR)
	add_subdirectory(mouse-android-emulator)
endif()
