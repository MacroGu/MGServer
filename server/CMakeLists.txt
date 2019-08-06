cmake_minimum_required(VERSION 2.8.9)
project(server)

# Add header file include directories
include_directories(./ ./header ../libuv/include ../include/mysql)
link_directories(../libs/mysql_win)

set(HEADER_FILES_PREFIX header)
set(HEADER_FILES
	singleton.h
	protocol.h
	${HEADER_FILES_PREFIX}/defines.h
    ${HEADER_FILES_PREFIX}/ServerLog.h
	${HEADER_FILES_PREFIX}/ServerConf.h
	${HEADER_FILES_PREFIX}/RedisHandle.h
	${HEADER_FILES_PREFIX}/DBConnector.h
)

set(SOURCE_FILES_PREFIX source)
set(SOURCE_FILES 
    ${SOURCE_FILES_PREFIX}/ServerLog.cpp
	${SOURCE_FILES_PREFIX}/ServerConf.cpp
	${SOURCE_FILES_PREFIX}/RedisHandle.cpp
	${SOURCE_FILES_PREFIX}/DBConnector.cpp
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
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SPDLOG_ACTIVE_LEVEL)
	target_link_libraries(server libmysql)
else()

endif()
target_link_libraries(server tinyxml hiredis libuv)

set(CMAKE_LIBRARY_PATH "../libs")

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()
