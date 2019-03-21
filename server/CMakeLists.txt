cmake_minimum_required(VERSION 2.8.9)
project(server)

# Add header file include directories
include_directories(./ ./header ../ ../websocketpp ../wepoll/include)

set(HEADER_FILES_PREFIX header)
set(HEADER_FILES
	singleton.h
	protocol.h
	${HEADER_FILES_PREFIX}/defines.h
    ${HEADER_FILES_PREFIX}/ServerLog.h
	${HEADER_FILES_PREFIX}/ServerConf.h
	${HEADER_FILES_PREFIX}/ThreadPool.h
	${HEADER_FILES_PREFIX}/EpollSocket.h
	${HEADER_FILES_PREFIX}/WebSocketStyle.h
	${HEADER_FILES_PREFIX}/base64.h
	${HEADER_FILES_PREFIX}/sha1.h
	${HEADER_FILES_PREFIX}/AcceptServer.h
)

set(SOURCE_FILES_PREFIX source)
set(SOURCE_FILES 
    ${SOURCE_FILES_PREFIX}/ServerLog.cpp
	${SOURCE_FILES_PREFIX}/ServerConf.cpp
	${SOURCE_FILES_PREFIX}/ThreadPool.cpp
	${SOURCE_FILES_PREFIX}/EpollSocket.cpp
	${SOURCE_FILES_PREFIX}/WebSocketStyle.cpp
	${SOURCE_FILES_PREFIX}/base64.cpp
	${SOURCE_FILES_PREFIX}/sha1.cpp
	${SOURCE_FILES_PREFIX}/AcceptServer.cpp
	main.cpp
)

source_group(header FILES ${HEADER_FILES})
source_group(source FILES ${SOURCE_FILES})

# Target
add_executable(server
    ${HEADER_FILES}
    ${SOURCE_FILES}
)
if(MSVC)
	target_link_libraries(server tinyxml wepoll hiredis)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()
target_link_libraries(server tinyxml hiredis)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()
