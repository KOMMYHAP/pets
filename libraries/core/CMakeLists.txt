project(core)

set(SRCS
	include/application/ApplicationBase.h
	include/application/ApplicationEvent.h

	src/application/ApplicationBase.cpp
	src/application/ApplicationEvent.cpp

	# include/files/FileManager.h

	# src/files/FileManager.cpp

	include/network/IpAddress.hpp
	include/network/Packet.hpp
	include/network/Socket.hpp
	include/network/UdpSocket.hpp
	include/network/SocketHandle.hpp

	src/network/IpAddress.cpp
	src/network/Packet.cpp
	src/network/Socket.cpp
	src/network/SocketImpl.hpp
	src/network/UdpSocket.cpp

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
	# include/tools/JsonConfig.h
	include/tools/TemplateUtils.h
	include/tools/Timer.h
	include/tools/TimeState.h
	include/tools/Utils.h

	src/tools/ParsedCommandLine.cpp
	# src/tools/JsonConfig.cpp
	src/tools/Timer.cpp
	src/tools/TimeState.cpp
	src/tools/Utils.cpp

	include/stddefs.h
)

if (WIN32)
	set(SRCS ${SRCS}
		src/network/Win32/SocketImpl.cpp
		src/network/Win32/SocketImpl.hpp)
else()
	set(SRCS ${SRCS}
		src/network/Unix/SocketImpl.cpp
		src/network/Unix/SocketImpl.hpp)
endif()

add_library(${PROJECT_NAME} STATIC ${SRCS})
add_library(PETS::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
	PRIVATE
		src/network)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		tracy)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# target_precompile_headers(${PROJECT_NAME} PRIVATE include/stddefs.h)
