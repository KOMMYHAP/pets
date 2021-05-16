project(core)

set(SRCS
	include/application/ApplicationBase.h

	src/application/ApplicationBase.cpp

	include/files/FileManager.h

	src/files/FileManager.cpp

	include/operations/Operation.h
	include/operations/OperationDispatcher.h
	include/operations/OperationMainThread.h
	include/operations/OperationManager.h
	include/operations/OperationThread.h
	include/operations/OperationThreadBase.h
	include/operations/OperationThreadIds.h
	include/operations/SimpleOperation.h

	src/operations/Operation.cpp
	src/operations/OperationDispatcher.cpp
	src/operations/OperationMainThread.cpp
	src/operations/OperationManager.cpp
	src/operations/OperationThread.cpp
	src/operations/SimpleOperation.cpp

	include/tools/Callback.h
	include/tools/ParsedCommandLine.h
	include/tools/JsonConfig.h
	include/tools/TemplateUtils.h
	include/tools/Timer.h
	include/tools/TimeState.h
	include/tools/Utils.h

	src/tools/ParsedCommandLine.cpp
	src/tools/JsonConfig.cpp
	src/tools/Timer.cpp
	src/tools/TimeState.cpp
	src/tools/Utils.cpp

	include/stddefs.h
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
add_library(PETS::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /std:c++17)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		SFML
		tracy
	PRIVATE
		jsoncpp)

target_precompile_headers(${PROJECT_NAME} PRIVATE include/stddefs.h)
